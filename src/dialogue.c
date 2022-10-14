
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "gamestate.h"
#include "dialogue.h"

// Variables for beginOptsMode and error handling.
// Stored as a struct to make error handling cheaper.
struct optsModeData
{
	dia_OptList *pDol;
	uint32_t *puOptAddrs;
	//size_t cAddrsRead;
	dia_OptNode **ppOpt;
	int cOptsLoaded;
};

// Local procs:
void killOptsData (struct optsModeData *pOptsData);
int beginOptsMode(const uint32_t uDolAddr);
int promptUserForOpt (int *pnUserInput);


int beginDialogue (const uint32_t uDiaAddr) {

	// Load DIA node.
	talk_DiaNode *pDia;
	pDia = loadNode(g_pGameState->fpStory, uDiaAddr, NT_DIA);
	if (pDia == NULL) return 1;

	// Select string to print.
	uint32_t uStrAddr;
	if (((pDia->fReqStory == 0) || (pDia->fReqStory & g_pGameState->fStory)) &&
		((pDia->fReqItems == 0) || (pDia->fReqItems & g_pGameState->fItem)) &&
		(pDia->uAltTextAddr != 0)) {

		// Select alt string & use alt flags.
		uStrAddr = pDia->uAltTextAddr;
		g_pGameState->fStory |= pDia->fAltStory;
		g_pGameState->fItem |= pDia->fAltItem;
	} else {
		// Select standard string & use standard flags.
		uStrAddr = pDia->uTextAddr;
		g_pGameState->fStory |= pDia->fStory;
		g_pGameState->fItem |= pDia->fItem;
	}

	// Print string.
	if (printStrFromStory(g_pGameState->fpStory, uStrAddr)) {
		free(pDia);
		return 1;
	}

	// Branch to OPT node processor.
	if (pDia->uOptList != 0) {
		if (beginOptsMode(pDia->uOptList)) {
			free(pDia);
			return 1;
		}
	}

	free(pDia);
	return 0;

}

// Destroy an optsModeData struct.
void killOptsData (struct optsModeData *pOptsData) {
	if (pOptsData->pDol) free(pOptsData->pDol);
	if (pOptsData->puOptAddrs) free(pOptsData->puOptAddrs);
	if (pOptsData->cOptsLoaded > 0) {
		while (pOptsData->cOptsLoaded > 0) {
			free(pOptsData->ppOpt[pOptsData->cOptsLoaded]);
			--pOptsData->cOptsLoaded;
		}
	}
}

int beginOptsMode(const uint32_t uDolAddr) {

	// TODO: Error handling is mad expensive here on space. Consider moving
	// things to another function that performs necessary deallocation on the
	// variables used here, and moving the variables to some kind of struct,
	// shared or not.

	struct optsModeData optsData;

	// Load DOL node.
	optsData.pDol = loadNode(g_pGameState->fpStory, uDolAddr, NT_DOL);
	if (optsData.pDol == NULL) return 1;
#ifdef _DEBUG
	printf("DEBUG: Loaded DOL node.\n");
#endif

	// If no options, return without error.
	// NOTE: This technically means the story file is invalid, but there is no
	// reason to treat this as a fatal error.
	if (optsData.pDol->cOpts == 0) {
		fprintf(stderr, "WARN: Ignoring DOL node with no entries.\n");
		killOptsData(&optsData);
		return 0;
	}

	// Allocate an array of OPT node addresses.
	if ((optsData.puOptAddrs = calloc(optsData.pDol->cOpts, sizeof(uint32_t))) == NULL) {
		killOptsData(&optsData);
		return 1;
	}

	// Read in OPT node addresses.
	// TODO: Debug this. puOptAddrs is filled with erroneous values.
	// The ones it does fill are closeby the real values, so it shouldn't be too hard.
	// Actual problem may be in story file as well.
	size_t cAddrsRead;
	cAddrsRead = fread(optsData.puOptAddrs, sizeof(uint32_t), optsData.pDol->cOpts, g_pGameState->fpStory);
	if (cAddrsRead < optsData.pDol->cOpts) {
		killOptsData(&optsData);
		fprintf(stderr, "ERROR: Failed to read OPT nodes from DOL node: ");
		if (ferror(g_pGameState->fpStory)) fprintf(stderr, "File I/O.\n");
		if (feof(g_pGameState->fpStory)) fprintf(stderr, "Premature EOF.\n");
		return 1;
	}

	// Allocate space for the list of opt nodes.
	if ((optsData.ppOpt = calloc(optsData.pDol->cOpts, sizeof(uint32_t))) == NULL) {
		killOptsData(&optsData);
		return 1;
	}

	// Load each opt node and print out strings.
	optsData.cOptsLoaded = 0;
	for (int iOpt = 0; iOpt < optsData.pDol->cOpts; iOpt++) {
#ifdef _DEBUG
		printf("DEBUG: Loading option #%d from 0x%X.\n", iOpt, optsData.puOptAddrs[iOpt]);
#endif
		optsData.ppOpt[iOpt] = loadNode(g_pGameState->fpStory, optsData.puOptAddrs[iOpt], NT_OPT);
		if (optsData.ppOpt[iOpt] == NULL) {
			killOptsData(&optsData);
			return 1;
		}
		optsData.cOptsLoaded++;

		// Print out each string.
		if (((optsData.ppOpt[iOpt]->fReqStory == 0) || (optsData.ppOpt[iOpt]->fReqStory & g_pGameState->fStory)) &&
			((optsData.ppOpt[iOpt]->fReqItems == 0) || (optsData.ppOpt[iOpt]->fReqItems & g_pGameState->fItem))) {
			printf("%d: ", iOpt);
			if (printStrFromStory(g_pGameState->fpStory, optsData.ppOpt[iOpt]->uTextAddr)) {
				killOptsData(&optsData);
				return 1;
			}
		}
	}

	// Get user input.
	int nUserInput;
	if (promptUserForOpt(&nUserInput)) {
		killOptsData(&optsData);
		return 1;
	}
	printf("Selected %d\n", nUserInput);

	return 0;

}

int promptUserForOpt (int *pnUserInput) {

	while (1) {

		printf("Select an option: ");

		errno = 0;

		if (scanf("%i", pnUserInput) != 1) {
			if (errno != 0) {
				perror("scanf failed to convert user input");
				return 1;
			} else {
				printf("Invalid selection.\n");
				continue;
			}
		}

		break;

	}

	return 0;

}
