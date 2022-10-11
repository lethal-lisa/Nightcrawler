
// Nightcrawler Engine - Game State Machine Module

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wingetline.h"
#include "gamestate.h"

struct gameState *g_pGameState;

int initGameState () {

	if ((g_pGameState = malloc(sizeof(struct gameState))) == NULL) {
		perror("initGameState");
		return 1;
	}
	memset(g_pGameState, 0, sizeof(struct gameState));

	return 0;
}

int killGameState () {

	closeStoryFile(g_pGameState->fpStory);

	if (g_pGameState->pStory) free(g_pGameState->pStory);
	if (g_pGameState->pScene) free(g_pGameState->pScene);
	if (g_pGameState->pszPromptString) free(g_pGameState->pszPromptString);
	if (g_pGameState->pszHelpString) free(g_pGameState->pszHelpString);

	if (g_pGameState) free(g_pGameState);

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
		/* if (ferror(g_pGameState->fpStory)) {
			fprintf(stderr, "ERROR: Couldn't read prompt string.\n");
			return 1;
		} */
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
		/* if (ferror(g_pGameState->fpStory)) {
			fprintf(stderr, "ERROR: Couldn't read help string.\n");
			return 1;
		} */
		if (getStrsFromStory_handleError(cbBytesRead, "Failed to load help string due to file error")) return 1;
	}

	// Load item names.
	// ...oh boy... here we go... If I actually get this done tonight I'll be a...
	// VERY surprised.


	return 0;

}
