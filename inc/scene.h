
// Nightcrawler Engine - Scene Node Header

#ifndef __SCENE_H__
#define __SCENE_H__ 1

#include <stdint.h>

typedef struct tagSceneNodeHdr
{
	char szMagic[4]; // Magic number identifier "NSC".
	uint32_t uMoveClustAddr; // MOVE cluster address.
	uint32_t uLookClustAddr; // LOOK cluster address.
	uint16_t uGetMask; // GET item mask;
	uint32_t uTalkClustAddr; // TALK cluster address.
	uint32_t uUseClustAddr; // USE cluster address.
} __attribute((packed, aligned(4))) sceneNodeHdr;

#endif /* __SCENE_H__ */
