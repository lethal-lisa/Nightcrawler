
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"

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

	// TODO: Branch to OPT node processor.

	free(pDia);
	return 0;

}
