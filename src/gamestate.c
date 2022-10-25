
// Nightcrawler Engine - Game State Machine Module

#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

	dthNodeHdr *pDth;
	pDth = loadNode(g_pGameState->fpStory, nodeAddr, NT_DTH);
	if (pDth == NULL) return 1;

	if (printStrFromStory(g_pGameState->fpStory, pDth->uStrAddr)) return 1;

	if (resetGameState()) return 1;

	g_pGameState->nWonLost = GS_LOST;

	return 0;

}

int procWin (const int nodeAddr) {

	winNodeHdr *pWin;
	pWin = loadNode(g_pGameState->fpStory, nodeAddr, NT_WIN);
	if (pWin == NULL) return 1;

	if (printStrFromStory(g_pGameState->fpStory, pWin->uStrAddr)) return 1;

	if (resetGameState()) return 1;

	g_pGameState->nWonLost = GS_WON;

	return 0;

}

int getStrsFromStory_handleError (const ssize_t cbBytesRead, const char *pszErrMsg) {

	if ((cbBytesRead == -1) && (feof(g_pGameState->fpStory) || ferror(g_pGameState->fpStory))) {
		fprintf(stderr, "ERROR: getStrsFromStory: %s: %s\n", pszErrMsg, strerror(errno));
		return 1;
	}

	return 0;

}

int getStrsFromStoryFile () {

	ssize_t cbBytesRead;
	const char pszDefPrompt[] = "Your next move? ";

	// Get prompt string.
	if (g_pGameState->pStory->uPromptStrAddr != 0) {
		if (fseek(g_pGameState->fpStory, g_pGameState->pStory->uPromptStrAddr, SEEK_SET)) {
			perror("Failed seeking for prompt string");
			return 1;
		}
		cbBytesRead = wingetdelim(&g_pGameState->pszPromptString, &g_pGameState->cchPromptString, '\0', g_pGameState->fpStory);
		if (getStrsFromStory_handleError(cbBytesRead, "Failed to load prompt string due to file error")) return 1;

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
		if (fseek(g_pGameState->fpStory, g_pGameState->pStory->uHelpStrAddr, SEEK_SET)) {
			perror("Failed seeking for help string");
			return 1;
		}
		cbBytesRead = wingetdelim(&g_pGameState->pszHelpString, &g_pGameState->cchPromptString, '\0', g_pGameState->fpStory);
		if (getStrsFromStory_handleError(cbBytesRead, "Failed to load help string due to file error")) return 1;
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
			cbBytesRead = wingetdelim(&g_pGameState->ppszItemName[iItem], &g_pGameState->pcchItemName[iItem], '\0', g_pGameState->fpStory);
			if (getStrsFromStory_handleError(cbBytesRead, "Failed to load an item")) return 1;
		}
	}

	return 0;

}
