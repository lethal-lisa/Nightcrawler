
// Nightcrawler Engine - Game State Machine Header

#ifndef GAMESTATE_H_
#define GAMESTATE_H_ 1

#include <stdint.h>
#include <stdio.h>

#include "story.h"

// Values for nWonLost.
enum WONLOSTSTATE
{
	GS_NORMAL,
	GS_WON,
	GS_LOST
};

// Structure for game state machine
struct gameState
{
	storyFileHdr *pStory; // Story file header object.
	FILE *fpStory; // Story file stream.
	uint32_t uCurSceneAddr; // Address of the current scene's node.
	sceneNodeHdr *pScene; // Current scene node object.
	uint32_t fStory; // Current story flags.
	uint32_t fItem; // Current item flags.
	size_t *pcchItemName; // Lengths of strings in ppszItemName.
	char **ppszItemName; // Pointers to item name strings.
	size_t cchPromptString; // Length of pszPromptString.
	char *pszPromptString; // Prompt string.
	size_t cchHelpString; // Length of pszHelpString.
	char *pszHelpString; // Help string.
	enum WONLOSTSTATE nWonLost; // Win/lose state.
};

// Global state machine structure.
extern struct gameState *g_pGameState;

// Initialize & destroy g_pGameState.
int initGameState ();
int killGameState ();

// Reset the game.
int resetGameState ();

// Reloads the current scene.
int reloadScene (void);

// Attempt to process a node as if it were a WIN or DTH node.
int procDeath (const int nodeAddr);
int procWin (const int nodeAddr);

// Loads global strings from the story file, part of initialization.
int getStrsFromStoryFile ();

#endif /* GAMESTATE_H_ */