
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"
#include "dialogue.h"
#include "validate.h"

// Variables for beginOptsMode and error handling.
// Stored as a struct to make error handling cheaper.
struct optsModeData
{
	dia_OptList *pDol; // DOL node.
	uint32_t *puOptAddrs; // List of option addresses.
	dia_OptNode **ppOpt; // List of OPT nodes.
	bool *pbSelectable; // Selectable options.
	int cOptsLoaded; // Count of loaded OPT nodes.
};

// Local procs:

// Basic cleanup proc for optsModeData.
void killOptsData (struct optsModeData *pOptsData);

// Enters the loop where the engine is asking for user input based on dialogue
// options.
int beginOptsMode (const uint32_t uDolAddr);

// Grabs an unsigned integer from the user.
int promptUserForOpt (unsigned int *puUserInput);

int beginDialogue (const uint32_t uDiaAddr) {

	// Load DIA node.
	talk_DiaNode *pDia;
	pDia = loadNode(g_pGameState->fpStory, uDiaAddr, NT_DIA);
	if (pDia == NULL) return 1;

	// Validate DIA node.
	if (validDia(pDia) == false) {
		fprintf(stderr, "ERROR: %s: Invalid DIA node @0x%X.\n", __func__, uDiaAddr);
		return 1;
	}

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

	// Cleanup and return.
	free(pDia);
	return 0;
}

void killOptsData (struct optsModeData *pOptsData) {
#ifdef _DEBUG
	puts("DEBUG: Called killOptsData.");
#endif
	if (pOptsData->pDol) free(pOptsData->pDol);
	if (pOptsData->puOptAddrs) free(pOptsData->puOptAddrs);
	if (pOptsData->pbSelectable) free(pOptsData->pbSelectable);
	if (pOptsData->ppOpt) {
		for (int iOption = 0; iOption < pOptsData->cOptsLoaded; iOption++)
			if (pOptsData->ppOpt[iOption]) free(pOptsData->ppOpt[iOption]);
		free(pOptsData->ppOpt);
	}
}

int beginOptsMode(const uint32_t uDolAddr) {

	// BUG: There is a memory leak somewhere in this routine, I believe. Someday
	// I will profile it out... Maybe.

	struct optsModeData optsData;

	// Load DOL node.
	optsData.pDol = loadNode(g_pGameState->fpStory, uDolAddr, NT_DOL);
	if (optsData.pDol == NULL) return 1;

	// Validate DOL node.
	if (validDol(optsData.pDol) == false) {
		fprintf(stderr, "ERROR: %s: Invalid DOL node @0x%X.\n", __func__, uDolAddr);
		return 1;
	}

	// If no options, return without error.
	// NOTE: This technically means the story file is invalid, but there is no
	// reason to treat this as a fatal error.
	if (optsData.pDol->cOpts == 0) {
		fprintf(stderr, "WARN: %s: Ignoring DOL node with no entries.\n", __func__);
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
	printf("DEBUG: %s: Beginning reading addresses from 0x%lX.\n", __func__, ftell(g_pGameState->fpStory));
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
	if ((optsData.pbSelectable = calloc(optsData.pDol->cOpts, sizeof(bool))) == NULL) {
		killOptsData(&optsData);
		return 1;
	}

	// Load each opt node and print out strings.
	optsData.cOptsLoaded = 0;
	for (int iOpt = 0; iOpt < optsData.pDol->cOpts; iOpt++) {
#ifdef _DEBUG
		printf("DEBUG: %s: Loading option #%d from 0x%X.\n", __func__, iOpt, optsData.puOptAddrs[iOpt]);
#endif
		optsData.ppOpt[iOpt] = loadNode(g_pGameState->fpStory, optsData.puOptAddrs[iOpt], NT_OPT);
		if (optsData.ppOpt[iOpt] == NULL) {
			killOptsData(&optsData);
			return 1;
		}
		optsData.cOptsLoaded++;

		// Validate OPT node.
		if (validOpt(optsData.ppOpt[iOpt]) == false) {
			fprintf(stderr, "ERROR: %s: Invalid OPT node @0x%X.\n", __func__, optsData.puOptAddrs[iOpt]);
			return 1;
		}

		// Print out each string.
		if (((optsData.ppOpt[iOpt]->fReqStory == 0) || (optsData.ppOpt[iOpt]->fReqStory & g_pGameState->fStory)) &&
			((optsData.ppOpt[iOpt]->fReqItems == 0) || (optsData.ppOpt[iOpt]->fReqItems & g_pGameState->fItem))) {

			// Print a leading number like "0:"
			printf("\t%d: ", iOpt);

			// Append the actual string.
			if (printStrFromStory(g_pGameState->fpStory, optsData.ppOpt[iOpt]->uTextAddr)) {
				killOptsData(&optsData);
				return 1;
			}

			// Turn the option selectable bool on.
			optsData.pbSelectable[iOpt] = true;
		}
	}

	unsigned int uUserInput; // Value gotten from `promptUserForOpt`.
	const char *pszInvalidSel = "Invalid selection."; // Constant string for
													  // invalid selections for
													  // selections are
													  // invalid. :D
	while (1) {

		// Get user input.
		if (promptUserForOpt(&uUserInput)) {
			killOptsData(&optsData);
			return 1;
		}

#ifdef _DEBUG
		printf("DEBUG: %s: Selected %d.\n", __func__, uUserInput);
#endif

		// Try again if invalid selection.
		if ((uUserInput > (optsData.pDol->cOpts - 1)) || (uUserInput < 0) || (optsData.pbSelectable[uUserInput] == false)) {
			puts(pszInvalidSel);
			continue;
		}

		// NOTE: These two blocks (the one above and the one below) are not
		// grouped together because the value of uUserInput is checked above,
		// and used to index ppOpt below.

		// Try again if requirements not met.
		if (!((optsData.ppOpt[uUserInput]->fReqStory == 0) || (optsData.ppOpt[uUserInput]->fReqStory & g_pGameState->fStory)) &&
			!((optsData.ppOpt[uUserInput]->fReqItems == 0) || (optsData.ppOpt[uUserInput]->fReqItems & g_pGameState->fItem))) {
			puts(pszInvalidSel);
			continue;
		}

		break;
	}

	// Act on desired opt node by processing for win or death and then checking
	// game state using logical OR operators.
	if (procDeath(optsData.ppOpt[uUserInput]->uDiaAddr) || procWin(optsData.ppOpt[uUserInput]->uDiaAddr) || (g_pGameState->nWonLost != GS_NORMAL)) {
		killOptsData(&optsData);
		if (g_pGameState->nWonLost == GS_LOST) return 0;
		return 1;
	}

	// Set flags based on selected OPT node.
	g_pGameState->fStory |= optsData.ppOpt[uUserInput]->fStory;
	g_pGameState->fItem |= optsData.ppOpt[uUserInput]->fItem;

	// Recurse into next level of options.
	if (beginDialogue(optsData.ppOpt[uUserInput]->uDiaAddr)) {
		killOptsData(&optsData);
		return 1;
	}

	// Change scenes if requested.
	if (optsData.ppOpt[uUserInput]->uMoveAddr != 0) {
		g_pGameState->uCurSceneAddr = optsData.ppOpt[uUserInput]->uMoveAddr;
		if (reloadScene()) {
			killOptsData(&optsData);
			return 1;
		}
	}

	// Cleanup and return.
	killOptsData(&optsData);
	return 0;
}