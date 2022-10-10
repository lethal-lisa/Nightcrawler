
// Nightcrawler Engine - Story File Header

#ifndef __STORY_H__
#define __STORY_H__ 1

#include <stdint.h>

// Node types:
// NOTE: An enum was considered here, but ended up increasing the file size,
// so this idea was rejected in favor of #defines.
#define NT_STORY 0
#define NT_SCENE 1
#define NT_MOVE 2
#define NT_LOOK 3
#define NT_TALK 4
#define NT_DIA 5

// Toplevel story node struct.
typedef struct tagStoryFileHdr
{
	char szMagic[4]; // Magic number identifer "NST".
	uint32_t uVersion; // Must be set to 0 for now.
	uint32_t uGameTitleAddr; // Address in the file where game title string begins.
	uint32_t uPromptStrAddr; // Address in the file where the prompt string begins (0 for none).
	uint32_t uHelpStrAddr; // Address in the file where the help string begins (0 for none).
	uint32_t uItemNameAddr; // Address in the file where item name cluster begins.
	uint32_t uInitSceneAddr; // Address in the file where the file name for the initial scene node is found.
} __attribute__((packed, aligned(4))) storyFileHdr;

// Scene node struct.
typedef struct tagSceneNodeHdr
{
	char szMagic[4]; // Magic number identifier "NSC".
	uint32_t uMoveClustAddr; // MOVE cluster address.
	uint32_t uLookClustAddr; // LOOK cluster address.
	uint16_t uGetMask; // GET item mask;
	uint32_t uTalkClustAddr; // TALK cluster address.
	uint32_t uUseClustAddr; // USE cluster address.
} __attribute((packed, aligned(4))) sceneNodeHdr;

// Move node struct.
typedef struct tagScene_MoveCluster
{
	char szMagic[4]; // Magic number identifier "MOV".
	uint16_t fReqdStory; // Required story flags to display alt text.
	uint16_t fReqdItem; // Required item flags to display alt text.
	uint32_t uNorthAddr; // Scene node to the North.
	uint32_t uSouthAddr; // Scene node to the South.
	uint32_t uEastAddr; // Scene node to the East.
	uint32_t uWestAddr; // Scene node to the West.
	uint32_t uAltNorthAddr; // Alt scene node to the North.
	uint32_t uAltSouthAddr; // Alt scene node to the South.
	uint32_t uAltEastAddr; // Alt scene node to the East.
	uint32_t uAltWestAddr; // Alt scene node to the West.
} __attribute((packed, aligned(4))) scene_MoveCluster;

// Look node struct.
typedef struct tagScene_LookCluster
{
	char szMagic[4]; // Magic number ID "LOK".
	uint16_t fReqdStory; // Required story flags to display alt text.
	uint16_t fReqdItem; // Required item flags to display alt text.
	uint32_t uAroundAddr; // Look around str addr.
	uint32_t uNorthAddr; // Look North str addr.
	uint32_t uSouthAddr; // Look South str addr.
	uint32_t uEastAddr; // Look East str addr.
	uint32_t uWestAddr; // Look West str addr.
	uint32_t uAltAroundAddr; // Alt look around string addr.
	uint32_t uAltNorthAddr; // Alt look North string addr.
	uint32_t uAltSouthAddr; // Alt look South string addr.
	uint32_t uAltEastAddr; // Alt look East string addr.
	uint32_t uAltWestAddr; // Alt look West string addr.
} __attribute((packed, aligned(4))) scene_LookCluster;

// Talk node struct.
typedef struct tagScene_TalkCluster
{
	char szMagic[4]; // Magic "TLK".
	uint16_t cDiaNodes; // Count of DIA nodes.
} __attribute((packed, aligned(4))) scene_TalkCluster;

// Individual dialogue section node.
typedef struct tagTalk_DiaNode
{
	char szMagic[4]; // Magic "DIA".
	uint16_t cOpts; // Count of OPT nodes (if any).
	uint16_t fStory; // Story flags to set.
	uint16_t fItem; // Item flags to set.
	uint16_t fReqItems; // Required item flags.
	uint16_t fReqStory; // Required story flags.
	uint32_t uTextAddr; // Address of primary text.
	uint32_t uAltTextAddr; // Address of text to display when Req'd flags not met.
} __attribute((packed, aligned(4))) talk_DiaNode;

// Handlers for opening and closing story files.
FILE *openStoryFile (const char *pszFileName);
int closeStoryFile (FILE *fp);

// Loads a node from address nodeAddr of type nodeType from file fpStory.
// Returns an pointer to the newly loaded struct, or NULL on failure. This
// routine prints its own error information to stderr.
void *loadNode (FILE *fpStory, const int nodeAddr, const int nodeType);

// Prints a string from the story file fpStory at address strAddr to stdout.
// Returns 0 on success and 1 on failure. This routine prints its own error
// information to stderr.
int printStrFromStory (FILE *fpStory, const int strAddr);

#endif /* __STORY_H__ */
