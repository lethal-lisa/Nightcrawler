
// Nightcrawler Engine - Game State Machine Header

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__ 1

#include <stdint.h>
#include <stdio.h>

#include "story.h"

struct gameState
{
	storyFileHdr *pStory;
	uint16_t uStoryFlags;
	uint16_t uItemFlags;
	uint32_t uCurScene;
	FILE *fpStory;
	size_t cchPromptString;
	char *pszPromptString;
	size_t cchHelpString;
	char *pszHelpString;
};

extern struct gameState *g_pGameState;

int initGameState ();
int killGameState ();
int getStrsFromStoryFile ();

#endif /* __GAMESTATE_H__ */
