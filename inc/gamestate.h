
// Nightcrawler Engine - Game State Machine Header

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__ 1

#include <stdint.h>

struct gameState
{
	uint16_t uStoryFlags;
	uint16_t uItemFlags;
	uint32_t uCurScene;
};

extern struct gameState *g_pGameState;

#endif /* __GAMESTATE_H__ */
