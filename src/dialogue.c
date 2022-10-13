
// Nightcrawler Engine - Dialogue System Module

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"

int beginDialogue (const uint32_t uDiaAddr) {

	// Load DIA node.
	talk_DiaNode *pDia;
	pDia = loadNode(g_pGameState->fpStory, uDiaAddr, NT_DIA);
	if (pDia == NULL) return 1;

	free(pDia);
	return 0;

}
