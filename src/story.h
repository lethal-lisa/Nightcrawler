
// Nightcrawler Engine - Story File Header

#ifndef __STORY_H__
#define __STORY_H__ 1

#include <stdint.h>
#include <stdio.h>

// Node types:
// NOTE: An enum was considered here, but ended up increasing the file size,
// so this idea was rejected in favor of #defines.
#define NT_STORY 0
#define NT_NST 0
#define NT_SCENE 1
#define NT_NSC 1
#define NT_MOVE 2
#define NT_MOV 2
#define NT_LOOK 3
#define NT_LOK 3
#define NT_TALK 4
#define NT_TLK 4
#define NT_DIA 5
#define NT_DOL 6
#define NT_OPT 7
#define NT_USE 8
#define NT_WIN 9
#define NT_DTH 10

#define NST_HIGHEST_SUPPORTED_VER 0
#define NST_MAX_ITEM_COUNT 16

// Toplevel story node struct.
typedef struct tagStoryFileHdr
{
	char szMagic[4]; // Magic number identifer "NST".
	uint32_t uVersion; // Must be set to 0 for now.
	uint32_t uGameTitleAddr; // Address in the file where game title string begins.
	uint32_t uPromptStrAddr; // Address in the file where the prompt string begins (0 for none).
	uint32_t uHelpStrAddr; // Address in the file where the help string begins (0 for none).
	uint16_t cItems; // Count of items.
	uint32_t uItemNameAddr; // Address in the file where item name list begins.
	uint32_t uInitSceneAddr; // Address in the file where the file name for the initial scene node is found.
} __attribute((packed)) storyFileHdr;

// Scene node struct.
typedef struct tagSceneNodeHdr
{
	char szMagic[4]; // Magic number identifier "NSC".
	uint32_t uMoveClustAddr; // MOVE cluster address.
	uint32_t uLookClustAddr; // LOOK cluster address.
	uint16_t uGetMask; // GET item mask.
	uint32_t uTalkClustAddr; // TALK cluster address.
	uint32_t uUseClustAddr; // USE cluster address.
} __attribute((packed)) sceneNodeHdr;

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
} __attribute((packed)) scene_MoveCluster;

// Look node struct.
typedef struct tagScene_LookCluster
{
	char szMagic[4]; // Magic number ID "LOK".
	uint16_t fStory; // Story flags to set.
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
} __attribute((packed)) scene_LookCluster;

// Talk node struct.
typedef struct tagScene_TalkCluster
{
	char szMagic[4]; // Magic "TLK".
	uint16_t fReqStory; // Required story flags.
	uint16_t fReqItems; // Required item flags.
	uint32_t uInitAddr; // Initial DIA node addr.
} __attribute((packed)) scene_TalkCluster;

// Individual dialogue section node struct.
typedef struct tagTalk_DiaNode
{
	char szMagic[4]; // Magic "DIA".
	uint16_t fStory; // Story flags to set.
	uint16_t fItem; // Item flags to set.
	uint16_t fAltStory; // Alt story flags to set.
	uint16_t fAltItem; // Alt item flags to set.
	uint16_t fReqStory; // Required story flags.
	uint16_t fReqItems; // Required item flags.
	uint32_t uTextAddr; // Address of primary text.
	uint32_t uAltTextAddr; // Address of text to display when Req'd flags met (0 if none).
	uint32_t uOptList; // OPT node list node addr (0 if none).
} __attribute((packed)) talk_DiaNode;

// OPT node list node struct.
typedef struct tagDia_OptList
{
	char szMagic[4]; // Magic "DOL".
	uint16_t cOpts; // Count of OPT nodes.
} __attribute((packed)) dia_OptList;

// OPT node struct.
typedef struct tagDia_OptNode
{
	char szMagic[4]; // Magic "OPT".
	uint16_t fStory; // Story flags to set.
	uint16_t fItem; // Item flags to set.
	uint16_t fReqStory; // Required story flags to set.
	uint16_t fReqItems; // Required item flags to set.
	uint32_t uTextAddr; // Address to the name of the option.
	uint32_t uDiaAddr; // Address of the DIA node to branch to.
} __attribute((packed)) dia_OptNode;

// Use node struct.
typedef struct tagScene_UseCluster
{
	char szMagic[4]; // Magic "USE".
	uint16_t fStory; // Story flags to set.
	uint16_t fItem; // Item flags to set.
	uint16_t fReqStory; // Required story flags.
	uint16_t fReqItems; // Required item flags.
	uint32_t uStrAddr; // Addr of string to print.
} __attribute((packed)) scene_UseCluster;

// WIN node struct.
typedef struct tagWinNodeHdr
{
	char szMagic[4]; // Magic "WIN".
	uint32_t uStrAddr; // Addr of string to print.
} __attribute((packed)) winNodeHdr;

// DTH node struct.
typedef struct tagDthNodeHdr
{
	char szMagic[4]; // Magic "DTH".
	uint32_t uStrAddr; // Addr of string to print.
} __attribute((packed)) dthNodeHdr;

// Handlers for opening and closing story files.
FILE *openStoryFile (char *pszFileName);
int closeStoryFile (FILE *fp);

// Loads a node from address nodeAddr of type nodeType from file fpStory.
// Returns an pointer to the newly loaded struct, or NULL on failure. This
// routine prints its own error information to stderr.
void *loadNode (FILE *fpStory, const int nodeAddr, const int nodeType);

int loadStrFromStory (FILE *fpStory, const int nStrAddr, size_t *pcchStr, char **ppszStr);

// Prints a string from the story file fpStory at address strAddr to stdout.
// Returns 0 on success and 1 on failure. This routine prints its own error
// information to stderr.
int printStrFromStory (FILE *fpStory, const int strAddr);

#endif /* __STORY_H__ */
