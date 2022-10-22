
// Nightcrawler Engine - Save/Load module.

#include <stdio.h>

#include "gamestate.h"
#include "saveload.h"

int saveGame (void) {

	FILE *fp;

	if ((fp = fopen("nightcrawler.sav", "w")) == NULL) return 1;

	fclose(fp);
	return 0;

}
