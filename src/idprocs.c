
// Nightcrawler Engine - Parser Command ID Processer

#include <stdio.h>

#include "gamestate.h"
#include "parsercmds.h"

int procLook (const char *pszParam) {

	// Load LOK node.
	scene_LookCluster *pLook;
	pLook = loadNode(g_pGameState->fpStory, g_pGameState->pScene->uLookClustAddr, NT_LOOK);
	if (pLook == NULL) return 1;

	uint32_t uStrAddr;

	// If no parameter use the "around" string.
	if (pszParam == NULL || strlen(pszParam) == 0) {
		// TODO: This works but it's hella messy and I don't wanna copy it
		// everywhere. Find a way to make this more legible.
		if ((g_pGameState->fItem & pLook->fReqdStory) &&
			(g_pGameState->fStory & pLook->fReqdItem) &&
			(pLook->uAltAroundAddr != 0)) {
			uStrAddr = pLook->uAltAroundAddr;
		} else {
			uStrAddr = pLook->uAroundAddr;
		}
		if (printStrFromStory(g_pGameState->fpStory, uStrAddr)) return 1;
		return 0;
	}

	struct parserCmd *pparserCmd;
	if ((pparserCmd = parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		fprintf(stderr, "ERROR: Invalid look direction.\n");
		return 0;
	}

	switch (pparserCmd->uId) {
		case CI_AROUND:
			uStrAddr = pLook->uAroundAddr;
			break;

		case CI_NORTH:
			uStrAddr = pLook->uNorthAddr;
			break;

		case CI_SOUTH:
			uStrAddr = pLook->uSouthAddr;
			break;

		case CI_EAST:
			uStrAddr = pLook->uEastAddr;
			break;

		case CI_WEST:
			uStrAddr = pLook->uWestAddr;
			break;

		default:
			fprintf(stderr, "ERROR: Bad directional value.\n");
			return 0;

	}

	if (printStrFromStory(g_pGameState->fpStory, uStrAddr)) return 1;

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

		case CI_LOOK: // Look around.
			procLook(pszParam);
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

