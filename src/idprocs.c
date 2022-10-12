
// Nightcrawler Engine - Parser Command ID Processer

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"
#include "parsercmds.h"

// Local procs.
uint32_t isAltLookTxtAvailable (const scene_LookCluster *pLook, const uint32_t uStrAddr, uint32_t uAltAddr);
int procLook (const char *pszParam);
uint32_t isAltMoveAvailable (const scene_MoveCluster *pMove, const uint32_t uAddr, const uint32_t uAltAddr);
int procMove (const char *pszParam);
int procGet (const char *pszParam);

// Determine if alternate look text is to be shown.
// Returns an address of either the normal or alternate string.
uint32_t isAltLookTxtAvailable (const scene_LookCluster *pLook, const uint32_t uStrAddr, uint32_t uAltAddr) {
	return (((pLook->fReqdStory == 0) || (g_pGameState->fStory & pLook->fReqdStory)) &&
		((pLook->fReqdItem == 0) || (g_pGameState->fItem & pLook->fReqdItem)) &&
		(uAltAddr != 0)) ? uAltAddr : uStrAddr;
}

// Process CI_LOOK.
int procLook (const char *pszParam) {

	// Load LOK node.
	scene_LookCluster *pLook;
	pLook = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uLookClustAddr, NT_LOOK);
	if (pLook == NULL) return 1;

	// Set story flags based on values in the scene.
	g_pGameState->fStory |= pLook->fStory;

	uint32_t uStrAddr;

	// If no parameter use the "around" string.
	if (pszParam == NULL || strlen(pszParam) == 0) {
		if (printStrFromStory(g_pGameState->fpStory, isAltLookTxtAvailable(pLook, pLook->uAroundAddr, pLook->uAltAroundAddr))) return 1;
		free(pLook);
		return 0;
	}

	struct parserCmd *pparserCmd;
	if ((pparserCmd = parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		fprintf(stderr, "ERROR: Unrecognized keyword \"%s\".\n", pszParam);
		free(pLook);
		return 0;
	}

	switch (pparserCmd->uId) {
		case CI_AROUND:
			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uAroundAddr, pLook->uAltAroundAddr);
			break;

		case CI_NORTH:
			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uNorthAddr, pLook->uAltNorthAddr);
			break;

		case CI_SOUTH:
			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uSouthAddr, pLook->uAltSouthAddr);
			break;

		case CI_EAST:
			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uEastAddr, pLook->uAltEastAddr);
			break;

		case CI_WEST:
			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uWestAddr, pLook->uAltWestAddr);
			break;

		default:
			fprintf(stderr, "ERROR: Bad directional value.\n");
			free(pLook);
			return 0;

	}

	if (printStrFromStory(g_pGameState->fpStory, uStrAddr)) return 1;

	free(pLook);
	return 0;

}

uint32_t isAltMoveAvailable (const scene_MoveCluster *pMove, const uint32_t uAddr, const uint32_t uAltAddr) {
	return (((pMove->fReqdStory == 0) || (g_pGameState->fStory & pMove->fReqdStory)) &&
		((pMove->fReqdItem == 0) || (g_pGameState->fItem & pMove->fReqdItem)) &&
		(uAltAddr != 0)) ? uAltAddr : uAddr;
}

// Process CI_MOVE.
int procMove (const char *pszParam) {

	// Load MOV node.
	scene_MoveCluster *pMove;
	pMove = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uMoveClustAddr, NT_MOVE);
	if (pMove == NULL) return 1;

	if (pszParam == NULL || strlen(pszParam) == 0) {
		printf("Go where?\n");
		free(pMove);
		return 0;
	}

	struct parserCmd *pparserCmd;
	if ((pparserCmd = parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		fprintf(stderr, "ERROR: Unrecognized keyword \"%s\".\n", pszParam);
		free(pMove);
		return 0;
	}

	uint32_t uNewSceneAddr;

	switch (pparserCmd->uId) {
		case CI_NORTH:
			uNewSceneAddr = isAltMoveAvailable(pMove, pMove->uNorthAddr, pMove->uAltNorthAddr);
			break;

		case CI_SOUTH:
			uNewSceneAddr = isAltMoveAvailable(pMove, pMove->uSouthAddr, pMove->uAltSouthAddr);
			break;

		case CI_EAST:
			uNewSceneAddr = isAltMoveAvailable(pMove, pMove->uEastAddr, pMove->uAltEastAddr);
			break;

		case CI_WEST:
			uNewSceneAddr = isAltMoveAvailable(pMove, pMove->uWestAddr, pMove->uAltWestAddr);
			break;

		default:
			fprintf(stderr, "ERROR: Bad directional value.\n");
			free(pMove);
			return 0;

	}

	// Change scenes.
	if (uNewSceneAddr != 0) {
		free(g_pGameState->pScene);
		g_pGameState->pScene = loadNode(g_pGameState->fpStory, uNewSceneAddr, NT_SCENE);

		// Run LOOK AROUND after changing scenes.
		if ((g_pGameState->pScene == NULL) || procLook(NULL)) {
			free(pMove);
			return 1;
		}
	} else {
		fprintf(stderr, "Unable to move that way.\n");
	}

	free(pMove);
	return 0;

}

// Process CI_GET.
int procGet (const char *pszParam) {

	// TODO: Write the rest of this. Checking item names and such, being able
	// to grab items from multiple sources besides scenes.

	// Print special string if there's nothing to grab, or items already
	// grabbed.
	if ((g_pGameState->pScene->uGetMask == 0) ||
		(g_pGameState->pStory->cItems == 0) ||
		(g_pGameState->fItem & g_pGameState->pScene->uGetMask)) {
		puts("Nothing to get.");
		return 0;
	}

	// Print string(s) of items grabbed.
	for (int iItem = 0; iItem < g_pGameState->pStory->cItems; iItem++) {
		if (g_pGameState->pScene->uGetMask & (1 << iItem)) {
			printf("Got %s.\n", g_pGameState->ppszItemName[iItem]);
		}
	}

	g_pGameState->fItem |= g_pGameState->pScene->uGetMask;
	return 0;

}

// Process CI_USE.
int procUse (const char *pszParam) {

	// Reject with no error if no use cluster.
	if (g_pGameState->pScene->uUseClustAddr == 0) {
		puts("No effect.");
		return 0;
	}

	// Load USE node.
	scene_UseCluster *pUse;
	pUse = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uUseClustAddr, NT_USE);
	if (pUse == NULL) return 1;

	if (((pUse->fReqStory == 0) || (pUse->fReqStory & g_pGameState->fStory)) &&
		((pUse->fReqItems == 0) || (pUse->fReqItems & g_pGameState->fItem))) {

		g_pGameState->fStory |= pUse->fStory;
		g_pGameState->fItem |= pUse->fItem;

		// Print string of USE node.
		if ((pUse->uStrAddr != 0) && (printStrFromStory(g_pGameState->fpStory, pUse->uStrAddr))) {
			free(pUse);
			return 1;
		}
	} else {
		puts("No effect.");
	}

	free(pUse);
	return 0;

}

int procCmdId (const unsigned int uId, const char *pszParam) {

	switch (uId) {
		case CI_HELP: // Show help.
			puts("-- Nightcrawler Engine Help --\n\
Command List (case does not matter):\n\
\tHELP            - You're looking at it.\n\
\tMOVE/GO         - Move to another scene.\n\
\tLOOK/EXAMINE    - Look around the current scene.\n\
\tGET             - Grab an item in the scene.\n\
\tTALK            - Talk to a character in the scene.\n\
\tUSE/INTERACT    - Do something in the scene with an item. For example:\n\
\t                  Show a character an item, use an item with an object\n\
\t                  in the scene, or perform some other context-\n\
\t                  sensitive action.\n\
\tITEMS/INVENTORY - Show a list of the items in your posession.\n\
\tQUIT/EXIT       - Quit the game.\n");
			if (g_pGameState->pszHelpString) puts(g_pGameState->pszHelpString);
			break;

		case CI_MOVE: // Move to a new scene.
			return procMove(pszParam);
			break;

		case CI_LOOK: // Look around.
			return procLook(pszParam);
			break;

		case CI_GET: // Get the item(s) in the scene.
			return procGet(pszParam);
			break;

		case CI_USE: // Use an item to set flags based on scene.
			return procUse(pszParam);
			break;

		case CI_ITEMS: // Show a list of items.
			if ((g_pGameState->pStory->cItems == 0) || (g_pGameState->fItem == 0)) break;
			puts("-- Inventory --");
			for (int iItem = 0; iItem < g_pGameState->pStory->cItems; iItem++) {
				if (g_pGameState->fItem & (1 << iItem)) puts(g_pGameState->ppszItemName[iItem]);
			}
			break;

		case CI_QUIT: // Quit the game.
			return 1;
			break;

		default:
			fprintf(stderr, "NYI: Command with ID %d not yet implemented.\n", uId);
			break;

	}

	return 0;

}

