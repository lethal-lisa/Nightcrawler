
// Nightcrawler Engine - Parser Command ID Processer

#include <stdio.h>
// #include "gamestate.h"
#include "parsercmds.h"

int procCmdId (const unsigned int uId) {

	switch (uId) {
		case CI_HELP:
			puts("-- Nightcrawler Engine Help --\n\
Command List (case does not matter):\n\
\tHELP         - You're looking at it.\n\
\tMOVE/GO      - Move to another scene.\n\
\tLOOK/EXAMINE - Look around the current scene.\n\
\tGET          - Grab an item in the scene.\n\
\tTALK         - Talk to a character in the scene.\n\
\tUSE/INTERACT - Do something in the scene with an item. For example:\n\
\t               Show a character an item, use an item with an object in the\n\
\t               scene, or perform some other context-sensitive action.\n");
			break;

		case CI_QUIT:
			return 1;
			break;

		default:
			fprintf(stderr, "NYI: Command with ID %d not yet implemented.\n", uId);
			break;

	}

	return 0;

}


