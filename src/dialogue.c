
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "gamestate.h"

// Variables for beginOptsMode and error handling.
// Stored as a struct to make error handling cheaper.
struct optsModeData
{
	dia_OptList *pDol;
	uint32_t *puOptAddrs;
	size_t cAddrsRead;
	dia_OptNode **ppOpt;
	int cNodesLoaded;
};

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

int beginOptsMode(const uint32_t uDolAddr) {

	// TODO: Error handling is mad expensive here on space. Consider moving
	// things to another function that performs necessary deallocation on the
	// variables used here, and moving the variables to some kind of struct,
	// shared or not.

	struct optsModeData optsData;

	// Load DOL node.
	dia_OptList *pDol;
	pDol = loadNode(g_pGameState->fpStory, uDolAddr, NT_DOL);
	if (pDol == NULL) return 1;

	// If no options, return without error.
	// NOTE: This technically means the story file is invalid, but there is no
	// reason to treat this as a fatal error.
	if (pDol->cOpts == 0) {
		fprintf(stderr, "WARN: Ignoring DOL node with no entries.\n");
		free(pDol);
		return 0;
	}

	// Allocate an array of OPT node addresses.
	uint32_t *puOptAddrs;
	if ((puOptAddrs = calloc(pDol->cOpts, sizeof(uint32_t))) == NULL) {
		free(pDol);
		return 1;
	}

	// Read in OPT nodes.
	size_t cAddrsRead;
	cAddrsRead = fread(puOutAddrs, sizeof(uint32_t), pDol->cOpts, g_pGameState->fpStory);
	if (cAddrsRead < pDol->cOpts) {
		free(pDol);
		free(puOptAddrs);
		fprintf(stderr, "ERROR: Failed to read OPT nodes from DOL node: ");
		if (ferror(g_pGameState->fpStory)) fprintf(stderr, "File I/O.\n");
		if (feof(g_pGameState->fpStory)) fprintf(stderr, "Premature EOF.\n");
		return 1;
	}

	// Allocate space for each opt node.
	dia_OptNode **ppOpt;
	if ((ppOpt = calloc(pDol->cOpts, sizeof(uint32_t))) == NULL) {
		free(pDol);
		free(puOptAddrs);
		return 1;
	}

	// Load each opt node and print out strings.
	int cNodesLoaded = 0;
	for (int iOpt = 0; iOpt < pDol->cOpts; iOpt++) {
		ppOpt[iOpt] = loadNode(g_pGameState->fpStory, puOptAddrs[iOpt], NT_OPT);
		if (ppOpt[iOpt] == NULL) {
			free(pDol);
			free(puOptAddrs);
			// TODO: Review this.
			// Free each loaded OPT node.
			while (cNodesLoaded > 0) {
				free(ppOpt[cNodesLoaded]);
				--cNodesLoaded;
			}
			free(ppOpt);
			return 1;
		}
		cNodesLoaded++;

		// Print out each string.
		if (((ppOpt[iOpt]->fReqStory == 0) || (ppOpt[iOpt]->fReqStory & g_pGameState->fStory)) &&
			((ppOpt[iOpt]->fReqItems == 0) || (ppOpt[iOpt]->fReqItems & g_pGameState->fItem))) {
			printf("%d: ", iOpt);
			if (printStrFromStory(g_pGameState->fpStory, ppOpt[iOpt]->uTextAddr)) {
				free(pDol);
				free(puOptAddrs);
				while (cNodesLoaded > 0) {
					free(ppOpt[cNodesLoaded]);
					--cNodesLoaded;
				}
				free(ppOpt);
				return 1;
			}
		}
	}

	// Prompt user for input.
	// char pchUserInput[2] = "\0";
	// int nUserInput;
	// char *pEnd;
	// if ((pchUserInput[0] = getchar()) == EOF) {
	// 	free(pDol);
	// 	free(puOptAddrs);
	// 	while (cNodesLoaded > 0) {
	// 		free(ppOpt[cNodesLoaded]);
	// 		--cNodesLoaded;
	// 	}
	// 	free(ppOpt);
	// 	return 1;
	// }

	int nUserInput;
	if (promptUserForOpt(&nUserInput))

	//nUserInput = strtol(pchUserInput, &pEnd, 10);
	//nUserInput = promptUserForOpt();
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

}
