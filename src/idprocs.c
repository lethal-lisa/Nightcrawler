
// Nightcrawler Engine - Parser Command ID Processer

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"
#include "parsercmds.h"
#include "dialogue.h"
#include "saveload.h"
#include "validate.h"
#include "input.h"
#include "idprocs.h"

// Local procs.

// Determine if alternate look text is to be shown.
// Returns an address of either the normal or alternate string.
uint32_t isAltLookTxtAvailable (const scene_LookCluster *pLook, const uint32_t uStrAddr, uint32_t uAltAddr);

// Determine if alternate movement addresses are to be used.
// Returns an address of either the normal or alternate string.
uint32_t isAltMoveAvailable (const scene_MoveCluster *pMove, const uint32_t uAddr, const uint32_t uAltAddr);

// Process a command based on its ID.
// IDs are defined in parsercmds.h, and returned by the gperf parser.
int procCmdId (const unsigned int uId, const char *pszParam) {

	switch (uId) {
		case CI_HELP: // Show help.
			puts("-- Nightcrawler Engine Help --\n\
Command List (case does not matter):\n\
\tHELP                - You're looking at it.\n\
\tMOVE/GO             - Move to another scene.\n\
\tLOOK/EXAMINE        - Look around the current scene.\n\
\tGET/GRAB/TAKE       - Grab an item in the scene.\n\
\tTALK                - Talk to a character in the scene.\n\
\tUSE/INTERACT        - Do something in the scene with an item. For example:\n\
\t                      Show a character an item, use an item with an object\n\
\t                      in the scene, or perform some other context-\n\
\t                      sensitive action.\n\
\tITEMS/INVENTORY/INV - Show a list of the items in your posession.\n\
\tSAVE                - Save the game to nightcrawler.sav.\n\
\tLOAD                - Load the game from nightcrawler.sav.\n\
\tQUIT/EXIT           - Quit the game.\n");
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
			if ((g_pGameState->pStory->cItems == 0) || (g_pGameState->fItem == 0)) {
				puts("Nothing in Inventory");
				break;
			}
			puts("-- Inventory --");
			for (int iItem = 0; iItem < g_pGameState->pStory->cItems; iItem++) {
				if (g_pGameState->fItem & (1 << iItem)) puts(g_pGameState->ppszItemName[iItem]);
			}
			break;

		case CI_TALK: // Begin talk mode.
			return procTalk(pszParam);
			break;

		case CI_NEXT: // Next scene.
			return procMove("NORTH");
			break;

		case CI_SAVE: // Save the game.
			return saveGame();
			break;

		case CI_LOAD: // Load the game.
			if (loadGame()) return 1;
			break;

		case CI_QUIT: // Quit the game.
			if (confirmYesNo("Are you sure?") == YN_YES) return 1;
			break;

		default:
			fprintf(stderr, "ERROR: %s, Invalid command ID %d. Try \"HELP\".\n", __func__, uId);
			break;
	}

	return 0;
}

// Wraps a ternary that checks for alternate look text results in a Look
// cluster.
uint32_t isAltLookTxtAvailable (const scene_LookCluster *pLook, const uint32_t uStrAddr, uint32_t uAltAddr) {
	return (((pLook->fReqdStory == 0) || (g_pGameState->fStory & pLook->fReqdStory)) &&
		((pLook->fReqdItem == 0) || (g_pGameState->fItem & pLook->fReqdItem)) &&
		(uAltAddr != 0)) ? uAltAddr : uStrAddr;
}

// Wraps a ternary that checks for alternate move locations in a move cluster.
uint32_t isAltMoveAvailable (const scene_MoveCluster *pMove, const uint32_t uAddr, const uint32_t uAltAddr) {
	return (((pMove->fReqdStory == 0) || (g_pGameState->fStory & pMove->fReqdStory)) &&
		((pMove->fReqdItem == 0) || (g_pGameState->fItem & pMove->fReqdItem)) &&
		(uAltAddr != 0)) ? uAltAddr : uAddr;
}

// Process CI_LOOK.
int procLook (const char *pszParam) {

	// Load LOK node.
	scene_LookCluster *pLook;
	pLook = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uLookClustAddr, NT_LOOK);
	if (pLook == NULL) return 1;

	// Validate LOK node.
	if (validLook(pLook) == false) {
		fprintf(stderr, "ERROR: %s: Invalid LOK node @0x%X.\n", __func__, g_pGameState->pScene->uLookClustAddr);
		return 1;
	}

	// If no parameter use the "around" string.
	if (pszParam == NULL || strlen(pszParam) == 0) {
		if (printStrFromStory(g_pGameState->fpStory, isAltLookTxtAvailable(pLook, pLook->uAroundAddr, pLook->uAltAroundAddr))) return 1;
		g_pGameState->fStory |= pLook->fStory;
		free(pLook);
		return 0;
	}

	struct parserCmd *pparserCmd;
	if ((pparserCmd = (struct parserCmd *)parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		printf("Unrecognized keyword \"%s\".\n", pszParam);
		free(pLook);
		return 0;
	}

	// Print respective look text.
	uint32_t uStrAddr;
	switch (pparserCmd->uId) {
		case CI_AROUND:

			uStrAddr = isAltLookTxtAvailable(pLook, pLook->uAroundAddr, pLook->uAltAroundAddr);

			// Set story flags based on values in the scene.
			g_pGameState->fStory |= pLook->fStory;

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
			fprintf(stderr, "ERROR: %s: Bad directional value.\n", __func__);
			free(pLook);
			return 0;
	}

	// Display look text.
	if (printStrFromStory(g_pGameState->fpStory, uStrAddr)) return 1;

	// Cleanup and return.
	free(pLook);
	return 0;
}

// Process CI_MOVE.
int procMove (const char *pszParam) {

	// Print "go where" if no direction given.
	if ((pszParam == NULL) || (strlen(pszParam) == 0)) {
		puts("Go where?");
		return 0;
	}

	// Load MOV node.
	scene_MoveCluster *pMove;
	pMove = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uMoveClustAddr, NT_MOVE);
	if (pMove == NULL) return 1;

	// Validate MOV node.
	if (validMove(pMove) == false) {
		fprintf(stderr, "ERROR: %s: Invalid MOV node @0x%X.\n", __func__, g_pGameState->pScene->uMoveClustAddr);
		return 1;
	}

	// Get the direction subcommand.
	struct parserCmd *pparserCmd;
	if ((pparserCmd = (struct parserCmd *)parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		// NOTE: This is not an error and is actually output intended for the
		// player, but this is the fastest way for this information to be
		// dispensed.
		printf("Unrecognized keyword \"%s\".\n", pszParam);
		free(pMove);
		return 0;
	}

	// Get the new scene address
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
			fprintf(stderr, "ERROR: %s: Bad directional value.\n", __func__);
			free(pMove);
			return 0;
	}

	// Change scenes.
	if (uNewSceneAddr != 0) {
		if (procDeath(uNewSceneAddr) || procWin(uNewSceneAddr) || (g_pGameState->nWonLost != GS_NORMAL)) {
			free(pMove);
			if (g_pGameState->nWonLost == GS_LOST) return 0;
			return 1;
		}

		// Run LOOK AROUND after changing scenes.
		g_pGameState->uCurSceneAddr = uNewSceneAddr;
		if (reloadScene()) {
			free(pMove);
			return 1;
		}
	} else {
		puts("Unable to move that way.");
	}

	free(pMove);
	return 0;
}

// Process CI_GET.
int procGet (const char *pszParam) {

	// TODO: Potentially implement checking against item names.

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

	// OR the scene's getmask with the player's inventory.
	g_pGameState->fItem |= g_pGameState->pScene->uGetMask;
	return 0;
}

// Process CI_USE.
int procUse (const char *pszParam) {

	// It's a constant string that says "No effect." It's used when an item does
	// nothing ORZ.
	const char pszNoEffect[] = "No effect.";

	// Reject with no error if no use cluster.
	if (g_pGameState->pScene->uUseClustAddr == 0) {
		puts(pszNoEffect);
		return 0;
	}

	// See if the player should die for their actions >:3
	if (procDeath(g_pGameState->pScene->uUseClustAddr) || procWin(g_pGameState->pScene->uUseClustAddr) || (g_pGameState->nWonLost != GS_NORMAL)) {
		if (g_pGameState->nWonLost == GS_LOST) return 0;
		return 1;
	}

	// Load USE node.
	scene_UseCluster *pUse;
	pUse = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uUseClustAddr, NT_USE);
	if (pUse == NULL) return 1;

	// Validate USE node.
	if (validUse(pUse) == false) {
		fprintf(stderr, "ERROR: %s: Invalid USE node @0x%X.\n", __func__, g_pGameState->pScene->uUseClustAddr);
		return 1;
	}

	// See if the player meets the requirements to use the item.
	if (((pUse->fReqStory == 0) || (pUse->fReqStory & g_pGameState->fStory)) &&
		((pUse->fReqItems == 0) || (pUse->fReqItems & g_pGameState->fItem))) {

		// Update story and item flags.
		g_pGameState->fStory |= pUse->fStory;
		g_pGameState->fItem |= pUse->fItem;

		// Print string of USE node.
		if ((pUse->uStrAddr != 0) && (printStrFromStory(g_pGameState->fpStory, pUse->uStrAddr))) {
			free(pUse);
			return 1;
		}

		// See if the player should be moved.
		if (pUse->uMoveAddr != 0) {
			g_pGameState->uCurSceneAddr = pUse->uMoveAddr;
			if (reloadScene()) {
				free(pUse);
				return 1;
			}
		}
	} else {
		puts(pszNoEffect);
	}

	free(pUse);
	return 0;
}

// Process CI_TALK.
int procTalk (const char *pszParam) {

	const char *pszNoOneHere = "There is no one here to talk to.";

	// Reject with no error if no talk cluster.
	if (g_pGameState->pScene->uTalkClustAddr == 0) {
		puts(pszNoOneHere);
		return 0;
	}

	// Load TLK node.
	scene_TalkCluster *pTalk;
	pTalk = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uTalkClustAddr, NT_TALK);
	if (pTalk == NULL) return 1;

	// Validate TLK node.
	if (validTalk(pTalk) == false) {
		fprintf(stderr, "ERROR: %s: Invalid TLK node @0x%X.\n", __func__, g_pGameState->pScene->uTalkClustAddr);
		return 1;
	}

	// Gate based on required flags.
	if (((pTalk->fReqStory == 0) || (pTalk->fReqStory & g_pGameState->fStory)) &&
		((pTalk->fReqItems == 0) || (pTalk->fReqItems & g_pGameState->fItem))) {

		// Process DIA node.
		if (beginDialogue(pTalk->uInitAddr)) {
			free(pTalk);
			return 1;
		}

	} else {
		puts(pszNoOneHere);
	}

	free(pTalk);
	return 0;
}