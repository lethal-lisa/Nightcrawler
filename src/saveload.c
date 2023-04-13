
// Nightcrawler Engine - Save/Load module.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // access
#include <ctype.h> // toupper

#include "gamestate.h"
#include "input.h"
#include "saveload.h"

const char *s_pszNcSaveFile = "nightcrawler.sav";
const uint32_t s_uSaveGameVersion = 0;

struct l_saveGameData
{
	uint32_t uSaveGameVer;
	uint32_t uCurSceneAddr;
	uint16_t fStory;
	uint16_t fItem;
} __attribute((packed));

int saveGame (void) {

	FILE *fp;
	struct l_saveGameData sgd;

	// Prompt user to overwrite if file already exists.
	if (access(s_pszNcSaveFile, F_OK) == 0) {
		if (confirmYesNo("Save file already exists. Ok to overwrite?") == YN_NO) {
			puts("Save cancelled...");
			return 0;
		}
	}

	if ((fp = fopen(s_pszNcSaveFile, "w")) == NULL) {
		perror("Failed to open Nightcrawler save file");
		return 0;
	}

	// Transfer g_pGameState to sgd.
	sgd.uSaveGameVer = s_uSaveGameVersion;
	sgd.uCurSceneAddr = g_pGameState->uCurSceneAddr;
	sgd.fStory = g_pGameState->fStory;
	sgd.fItem = g_pGameState->fItem;

	// Write to file.
	if (fwrite(&sgd, sizeof(struct l_saveGameData), 1, fp) != 1) {
		fclose(fp);
		return 1;
	}

	fclose(fp);

	puts("Save successful...");

	return 0;
}

int loadGame (void) {

	FILE *fp;
	struct l_saveGameData sgd;

	if ((fp = fopen(s_pszNcSaveFile, "r")) == NULL) {
		perror("Failed to open Nightcrawler save file");
		return 0;
	}

	// Read from file.
	if (fread(&sgd, sizeof(struct l_saveGameData), 1, fp) != 1) {
		fclose(fp);
		return 1;
	}

	fclose(fp);

	// Verify save file version.
	if (sgd.uSaveGameVer != s_uSaveGameVersion) {
		fprintf(stderr, "ERROR: Save game is for a different version of Nightcrawler!\nThis version expects 0x%X\n", s_uSaveGameVersion);
		return 1;
	}

	// Copy from save data.
	g_pGameState->uCurSceneAddr = sgd.uCurSceneAddr;
	g_pGameState->fStory = sgd.fStory;
	g_pGameState->fItem = sgd.fItem;

	// Reload scene.
	if (reloadScene() != 0) return 1;

	puts("Load successful...");

	return 0;
}
