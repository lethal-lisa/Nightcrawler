
// Nightcrawler Engine - Game State Machine Header

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__ 1

#include <stdint.h>
#include <stdio.h>

#include "story.h"

struct gameState
{
	storyFileHdr *pStory; // Story file header object.
	FILE *fpStory; // Story file stream.
	uint32_t uCurSceneAddr; // Address of the current scene's node.
	sceneNodeHdr *pScene; // Current scene node object.
	uint16_t fStory; // Current story flags.
	uint16_t fItem; // Current item flags.
	size_t *pcchItemName; // Lengths of strings in ppszItemName.
	char **ppszItemName; // Pointers to item name strings.
	size_t cchPromptString; // Length of pszPromptString.
	char *pszPromptString; // Prompt string.
	size_t cchHelpString; // Length of pszHelpString.
	char *pszHelpString; // Help string.
};

extern struct gameState *g_pGameState;

// Initialize & destroy g_pGameState.
int initGameState ();
int killGameState ();

// Reset the game.
int resetGameState ();

// Reloads the current scene.
int reloadScene (void);

int procDeath (const int nodeAddr);
int procWin (const int nodeAddr);

// Loads global strings from the story file, part of initialization.
int getStrsFromStoryFile ();

#endif /* __GAMESTATE_H__ */
