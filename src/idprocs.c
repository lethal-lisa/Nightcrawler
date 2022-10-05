
// Nightcrawler Engine - Parser Command ID Processer

#include <stdio.h>

#include "gamestate.h"
#include "parsercmds.h"

int procLook (const char *pszParam) {

	// Load LOK node.
	// ...

	if (pszParam == NULL || strlen(pszParam) == 0) {
		puts("Selected AROUND.");
		return 0;
	}

	struct parserCmd *pparserCmd;
	if ((pparserCmd = parserCmd_inWordSet(pszParam, strlen(pszParam))) == 0) {
		fprintf(stderr, "ERROR: Invalid look direction.\n");
		return 0;
	}

	switch (pparserCmd->uId) {
		case CI_NORTH:
			puts("Selected NORTH.");
			break;

		case CI_SOUTH:
			puts("Selected SOUTH.");
			break;

		case CI_EAST:
			puts("Selected EAST.");
			break;

		case CI_WEST:
			puts("Selected WEST.");
			break;

		default:
			fprintf(stderr, "ERROR: Bad directional value.\n");

	}

	return 0;

}

int procCmdId (const unsigned int uId, const char *pszParam) {

	switch (uId) {
		case CI_HELP: // Show help.
			puts("-- Nightcrawler Engine Help --\n\
Command List (case does not matter):\n\
\tHELP         - You're looking at it.\n\
\tMOVE/GO      - Move to another scene.\n\
\tLOOK/EXAMINE - Look around the current scene.\n\
\tGET          - Grab an item in the scene.\n\
\tTALK         - Talk to a character in the scene.\n\
\tUSE/INTERACT - Do something in the scene with an item. For example:\n\
\t               Show a character an item, use an item with an object in the\n\
\t               scene, or perform some other context-sensitive action.\n\
\tQUIT/EXIT    - Quit the game.\n");
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

