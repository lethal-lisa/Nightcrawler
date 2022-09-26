
// Nightcrawler Engine - Scene File Header

#ifndef __SCENE_H__
#define __SCENE_H__ 1

#include <stdint.h>

struct sceneFileHdr
{
	char szMagic[4]; // Magic number identifier "NSC".
	uint32_t uVersion; // Must be set to 0 for now.
	uint32_t uMoveClustOff; // MOVE cluster offset.
	uint32_t uLookClustOff; // LOOK cluster offset.
	uint16_t uGetMask; // GET item mask;
	uint32_t uDialogueClustOff; // TALK cluster offset.
	uint32_t uUseClustOff; // USE cluster offset.
};

#endif /* __SCENE_H__ */
