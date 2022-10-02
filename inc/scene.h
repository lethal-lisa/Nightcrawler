
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

typedef struct tagScene_MoveCluster
{
	char szMagic[4]; // Magic number identifier "MOV".
	uint32_t uNorthAddr; // Scene node to the North.
	uint32_t uSouthAddr; // Scene node to the South.
	uint32_t uEastAddr; // Scene node to the East.
	uint32_t uWestAddr; // Scene node to the West.
} __attribute((packed, aligned(4))) scene_MoveCluster;

typedef struct tagScene_LookCluster
{
	char szMagic[4]; // Magic number ID "LOK".
	uint32_t uAroundAddr; // Look around str addr.
	uint32_t uNorthAddr; // Look North str addr.
	uint32_t uSouthAddr; // Look South str addr.
	uint32_t uEastAddr; // Look East str addr.
	uint32_t uWestAddr; // Look West str addr.
} __attribute((packed, aligned(4))) scene_LookCluster;

typedef struct tagScene_TalkCluster
{
	char szMagic[4]; // Magic "TLK".
	uint16_t cDiaNodes; // Count of DIA nodes.
} __attribute((packed, aligned(4))) scene_TalkCluster;

typedef struct tagTalk_DiaNode
{
	char szMagic[4]; // Magic "DIA".
	uint16_t cOpts; // Count of OPT nodes (if any).
	uint16_t fItems; // Item flags to set.
	uint16_t fStory; // Story flags to set.
	uint16_t fReqItems; // Required item flags.
	uint16_t fReqStory; // Required story flags.
	uint32_t uTextAddr; // Address of primary text.
	uint32_t uAltTextAddr; // Address of text to display when Req'd flags not met.
} __attribute((packed, aligned(4))) talk_DiaNode;

#endif /* __SCENE_H__ */
