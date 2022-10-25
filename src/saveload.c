
// Nightcrawler Engine - Save/Load module.

#include <stdio.h>
#include <stdlib.h>

#include "gamestate.h"
#include "saveload.h"

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

	// TODO: Check if file exists beforehand, and prompt user to overwrite if
	// it does.
	if ((fp = fopen("nightcrawler.sav", "w")) == NULL) {
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
	return 0;

}

int loadGame (void) {

	FILE *fp;
	struct l_saveGameData sgd;

	if ((fp = fopen("nightcrawler.sav", "r")) == NULL) {
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
	//free(g_pGameState->pScene);
	//g_pGameState->pScene = loadNode(g_pGameState->fpStory, g_pGameState->uCurSceneAddr, NT_SCENE);
	//if (g_pGameState->pScene == NULL) {
		//fprintf(stderr, "ERROR: Failed to reload scene from story file.\n");
		//return 1;
	//}
	if (reloadScene() != 0) return 1;

	return 0;

}
