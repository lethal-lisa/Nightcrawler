
// Nightcrawler Engine - Save/Load module.

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // access

#include "gamestate.h"
#include "input.h"
#include "saveload.h"

// Hard-coded save file name.
const char *s_pszNcSaveFile = "nightcrawler.sav";

// Hard-coded save game version that THIS version of Nightcrawler is compatible
// with.
const uint32_t s_uSaveGameVersion = 1;

// Local structure that defines the layout of the save game data.
struct l_saveGameData
{
	uint32_t uSaveGameVer; // Save game version
	uint32_t uCurSceneAddr; // Address of current scene in loaded file
	uint32_t fStory; // Current story flags
	uint32_t fItem; // Current item flags
} __attribute((packed));

int saveGame (void) {

	FILE *fp; // Save file pointer.
	struct l_saveGameData sgd; // Save game data buffer structure.

	// Prompt user to overwrite if file already exists.
	if (access(s_pszNcSaveFile, F_OK) == 0) {
		if (confirmYesNo("Save file already exists. Ok to overwrite?") == YN_NO) {
			puts("Save cancelled...");
			return 0;
		}
	}

	// Actually open the file.
	if ((fp = fopen(s_pszNcSaveFile, "w")) == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to open Nightcrawler save file: %s\n", __func__, strerror(errno));
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

	if (fclose(fp) != 0) {
		fprintf(stderr, "ERROR: %s: Failed to close/flush Nightcrawler save file: %s\n", __func__, strerror(errno));
		return 1;
	}

	puts("Save successful...");

	return 0;
}

int loadGame (void) {

	FILE *fp;
	struct l_saveGameData sgd;

	if ((fp = fopen(s_pszNcSaveFile, "r")) == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to open Nightcrawler save file: %s\n", __func__, strerror(errno));
		return 0;
	}

	// Read from file.
	if (fread(&sgd, sizeof(struct l_saveGameData), 1, fp) != 1) {
		fclose(fp);
		return 1;
	}

	if (fclose(fp) != 0) {
		fprintf(stderr, "ERROR: %s: Failed to close Nightcrawler save file: %s\n", __func__, strerror(errno));
		return 1;
	}

	// Verify save file version.
	if (sgd.uSaveGameVer != s_uSaveGameVersion) {
		fprintf(stderr, "ERROR: %s: Save game is for a different version of Nightcrawler!\nThis version expects 0x%X\n", __func__, s_uSaveGameVersion);
		return 1;
	}

	// Copy from save data.
	g_pGameState->uCurSceneAddr = sgd.uCurSceneAddr;
	g_pGameState->fStory = sgd.fStory;
	g_pGameState->fItem = sgd.fItem;

	puts("Load successful...");

	// Reload scene.
	if (reloadScene() != 0) return 1;

	return 0;
}