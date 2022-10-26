
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "gamestate.h"
#include "dialogue.h"
#include "wingetline.h"

// Variables for beginOptsMode and error handling.
// Stored as a struct to make error handling cheaper.
struct optsModeData
{
	dia_OptList *pDol; // DOL node.
	uint32_t *puOptAddrs; // List of option addresses.
	dia_OptNode **ppOpt; // List of OPT nodes.
	int cOptsLoaded; // Count of loaded OPT nodes.
};

// Local procs:
void killOptsData (struct optsModeData *pOptsData);
int beginOptsMode (const uint32_t uDolAddr);
int promptUserForOpt (unsigned int *puUserInput);


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
#ifdef _DEBUG
	puts("DEBUG: Called killOptsData.");
#endif
	if (pOptsData->pDol) free(pOptsData->pDol);
	if (pOptsData->puOptAddrs) free(pOptsData->puOptAddrs);
	if (pOptsData->cOptsLoaded > 0) {
		while (pOptsData->cOptsLoaded > 0) {
			free(pOptsData->ppOpt[pOptsData->cOptsLoaded]);
			--pOptsData->cOptsLoaded;
		}
		free(pOptsData->ppOpt);
	}
}

// Enter opts mode.
int beginOptsMode(const uint32_t uDolAddr) {

	struct optsModeData optsData;

	// Load DOL node.
	optsData.pDol = loadNode(g_pGameState->fpStory, uDolAddr, NT_DOL);
	if (optsData.pDol == NULL) return 1;

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
#ifdef _DEBUG
	printf("DEBUG: Beginning reading addresses from 0x%lX.\n", ftell(g_pGameState->fpStory));
#endif
	size_t cAddrsRead;
	cAddrsRead = fread(&optsData.puOptAddrs[0], sizeof(uint32_t), optsData.pDol->cOpts, g_pGameState->fpStory);
	if (cAddrsRead < optsData.pDol->cOpts) {
		killOptsData(&optsData);
		fprintf(stderr, "ERROR: Failed to read OPT nodes from DOL node: ");
		if (ferror(g_pGameState->fpStory)) fprintf(stderr, "File I/O.\n");
		if (feof(g_pGameState->fpStory)) fprintf(stderr, "Premature EOF.\n");
		return 1;
	}

	// Allocate space for the list of opt nodes.
	if ((optsData.ppOpt = calloc(optsData.pDol->cOpts, sizeof(dia_OptNode *))) == NULL) {
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
			printf("\t%d: ", iOpt);
			if (printStrFromStory(g_pGameState->fpStory, optsData.ppOpt[iOpt]->uTextAddr)) {
				killOptsData(&optsData);
				return 1;
			}
		}
	}

	unsigned int uUserInput;
	while (1) {

		// Get user input.
		if (promptUserForOpt(&uUserInput)) {
			killOptsData(&optsData);
			return 1;
		}

#ifdef _DEBUG
		printf("DEBUG: Selected %d.\n", uUserInput);
#endif

		// Try again if invalid selection.
		if ((uUserInput > (optsData.pDol->cOpts - 1)) || (uUserInput < 0)) {
			printf("Invalid selection.\n");
			continue;
		}

		// NOTE: These two blocks (the one above and the one below) are not
		// grouped together because the value of uUserInput is checked above,
		// and used to index ppOpt below.

		// Try again if requirements not met.
		if (!((optsData.ppOpt[uUserInput]->fReqStory == 0) || (optsData.ppOpt[uUserInput]->fReqStory & g_pGameState->fStory)) &&
			!((optsData.ppOpt[uUserInput]->fReqItems == 0) || (optsData.ppOpt[uUserInput]->fReqItems & g_pGameState->fItem))) {
			printf("Invalid selection.\n");
			continue;
		}

		break;
	}

	// Act on desired opt node.
	g_pGameState->fStory |= optsData.ppOpt[uUserInput]->fStory;
	g_pGameState->fItem |= optsData.ppOpt[uUserInput]->fItem;
	if (beginDialogue(optsData.ppOpt[uUserInput]->uDiaAddr)) {
		killOptsData(&optsData);
		return 1;
	}

	killOptsData(&optsData);
	return 0;

}

// Prompt for user's choice as an unsigned int.
int promptUserForOpt (unsigned int *puUserInput) {

	char *pszUserInput;
	size_t cchUserInput;
	ssize_t cbBytesRead;
	char *endptr; // End pointer used by strtoul.

	while (1) {
		pszUserInput = NULL;
		cchUserInput = 0;

		// Get input from user.
		printf("Select an option: ");
		cbBytesRead = wingetline(&pszUserInput, &cchUserInput, stdin);
		if (cbBytesRead == -1) {
			if (feof(stdin)) {
				fprintf(stderr, "ERROR: Encountered EOF.\n");
				free(pszUserInput);
				return 1;
			}
			if (ferror(stdin)) {
				fprintf(stderr, "ERROR: Standard input error. Try again.\n");
				free(pszUserInput);
				clearerr(stdin);
				continue;
			}
		}

#ifdef _DEBUG
		printf("DEBUG: Allocated %zu chars and read %zu bytes.\n", cchUserInput, cbBytesRead);
#endif

		// Convert to uint.
		errno = 0;
		*puUserInput = strtoul(pszUserInput, &endptr, 10);
		if (errno != 0) {
			perror("Unable to convert user input.");
			free(pszUserInput);
			return 1;
		}

		// If no input, try again.
		if (endptr == pszUserInput) {
			printf("Invalid input.\n");
			free(pszUserInput);
			continue;
		}

		free(pszUserInput);
		return 0;

	}

}
