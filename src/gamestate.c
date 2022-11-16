
// Nightcrawler Engine - Game State Machine Module

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "story.h"
#include "wingetline.h"
#include "gamestate.h"

struct gameState *g_pGameState;

int initGameState () {

	// Allocate and initialize memory for game state struct.
	if ((g_pGameState = malloc(sizeof(struct gameState))) == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to allocate space for game state: %s.\n", __func__, strerror(errno));
		return 1;
	}
	memset(g_pGameState, 0, sizeof(struct gameState));

	return 0;
}

int killGameState () {

	closeStoryFile(g_pGameState->fpStory);

	for (int iItem = 0; iItem < g_pGameState->pStory->cItems; iItem++) {
		if ((g_pGameState->ppszItemName) && (g_pGameState->ppszItemName[iItem])) free(g_pGameState->ppszItemName[iItem]);
	}
	if (g_pGameState->pcchItemName) free(g_pGameState->pcchItemName);
	if (g_pGameState->ppszItemName) free(g_pGameState->ppszItemName);

	if (g_pGameState->pStory) free(g_pGameState->pStory);
	if (g_pGameState->pScene) free(g_pGameState->pScene);
	if (g_pGameState->pszPromptString) free(g_pGameState->pszPromptString);
	if (g_pGameState->pszHelpString) free(g_pGameState->pszHelpString);

	if (g_pGameState) free(g_pGameState);

	return 0;
}

int resetGameState () {

	// Re-initialize flags and scene addresses.
	g_pGameState->fStory = 0;
	g_pGameState->fItem = 0;
	g_pGameState->uCurSceneAddr = g_pGameState->pStory->uInitSceneAddr;
	g_pGameState->nWonLost = GS_NORMAL;

	if (reloadScene() != 0) return 1;

	// Display game's title again.
	if (g_pGameState->pStory->uGameTitleAddr) {
		if (printStrFromStory(g_pGameState->fpStory, g_pGameState->pStory->uGameTitleAddr)) return 1;
	}

	return 0;
}

// Reloads the current scene.
// This gets called after loading a game and after game over and such.
int reloadScene (void) {

	free(g_pGameState->pScene);
	g_pGameState->pScene = loadNode(g_pGameState->fpStory, g_pGameState->uCurSceneAddr, NT_SCENE);
	if (g_pGameState->pScene == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to reload scene.\n", __func__);
		return 1;
	}

	return 0;
}

int procDeath (const int nodeAddr) {

	char szMagic[4];

	// Seek to the specified offset in the file.
	if (fseek(g_pGameState->fpStory, nodeAddr, SEEK_SET)) {
		perror("ERROR: procDeath: fseek fail.");
		return 1;
	}

	// Read in the node.
	if (fread(szMagic, 4, 1, g_pGameState->fpStory) != 1) {
		if (ferror(g_pGameState->fpStory)) fprintf(stderr, "ERROR: %s: fread tripped file I/O error.\n", __func__);
		if (feof(g_pGameState->fpStory)) fprintf(stderr, "ERROR: %s: fread reached EOF.\n", __func__);
		return 1;
	}

	if (strncmp(szMagic, "DTH", 4) != 0) return 0;

	dthNodeHdr *pDth;
	pDth = loadNode(g_pGameState->fpStory, nodeAddr, NT_DTH);
	if (pDth == NULL) return 1;

	if (printStrFromStory(g_pGameState->fpStory, pDth->uStrAddr)) return 1;

	g_pGameState->nWonLost = GS_LOST;

	return 0;
}

int procWin (const int nodeAddr) {

	char szMagic[4];

	// Seek to the specified offset in the file.
	if (fseek(g_pGameState->fpStory, nodeAddr, SEEK_SET)) {
		perror("ERROR: procWin: fseek fail.");
		return 1;
	}

	// Read in the node.
	if (fread(szMagic, 4, 1, g_pGameState->fpStory) != 1) {
		if (ferror(g_pGameState->fpStory)) fprintf(stderr, "ERROR: %s: fread tripped file I/O error.\n", __func__);
		if (feof(g_pGameState->fpStory)) fprintf(stderr, "ERROR: %s: fread reached EOF.\n", __func__);
		return 1;
	}

	if (strncmp(szMagic, "WIN", 4) != 0) return 0;

	winNodeHdr *pWin;
	pWin = loadNode(g_pGameState->fpStory, nodeAddr, NT_WIN);
	if (pWin == NULL) return 1;

	if (printStrFromStory(g_pGameState->fpStory, pWin->uStrAddr)) return 1;

	g_pGameState->nWonLost = GS_WON;

	return 0;
}

int getStrsFromStoryFile () {

	const char pszDefPrompt[] = "N> ";

	// Get prompt string.
	if (g_pGameState->pStory->uPromptStrAddr != 0) {
		if (loadStrFromStory(g_pGameState->fpStory, g_pGameState->pStory->uPromptStrAddr, &g_pGameState->cchPromptString, &g_pGameState->pszPromptString)) {
			fprintf(stderr, "ERROR: %s: Failed to load prompt string.\n", __func__);
			return 1;
		}
	} else {
		// Use default string if none specified.
		if ((g_pGameState->pszPromptString = malloc(strlen(pszDefPrompt) + 1)) == NULL) {
			perror("Failed to allocate space for prompt string");
			return 1;
		}
		strcpy(g_pGameState->pszPromptString, pszDefPrompt);
	}

	// Load help string.
	if (g_pGameState->pStory->uHelpStrAddr != 0) {
		if (loadStrFromStory(g_pGameState->fpStory, g_pGameState->pStory->uHelpStrAddr, &g_pGameState->cchHelpString, &g_pGameState->pszHelpString)) {
			fprintf(stderr, "ERROR: %s: Failed to load help string.\n", __func__);
			return 1;
		}
	}

	// Load item names.
	if (g_pGameState->pStory->cItems != 0) {

		// Allocate space for each item.
		g_pGameState->pcchItemName = calloc(g_pGameState->pStory->cItems, sizeof(size_t));
		g_pGameState->ppszItemName = calloc(g_pGameState->pStory->cItems, sizeof(char *));
		if ((g_pGameState->pcchItemName == NULL) || (g_pGameState->ppszItemName == NULL)) {
			perror("Failed to allocate space for item names");
			return 1;
		}

		// Load in each item.
		for (int iItem = 0; iItem < g_pGameState->pStory->cItems; iItem++) {
			if (wingetdelim(&g_pGameState->ppszItemName[iItem], &g_pGameState->pcchItemName[iItem], '\0', g_pGameState->fpStory) == -1) {
				if (ferror(g_pGameState->fpStory)) fprintf(stderr, "ERROR: %s: Error reading string: %s.\n", __func__, strerror(errno));
				if (feof(g_pGameState->fpStory)) fprintf(stderr, "WARN: %s: Error reading string: End of file reached.\n", __func__);
				return 1;
			}
		}
	}

	return 0;
}
