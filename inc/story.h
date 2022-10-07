
// Nightcrawler Engine - Story File Header

#ifndef __STORY_H__
#define __STORY_H__ 1

// Nightcrawler story files contain information about the game itself: things
// like the game's "title screen" (not actually a title screen, just a string
// with intoductory text), names for the items used in the game, and the
// starting scene.

#include <stdint.h>

#include "scene.h"

#define NT_STORY 0
#define NT_SCENE 1
#define NT_MOVE 2
#define NT_LOOK 3
#define NT_TALK 4
#define NT_DIA 5

typedef struct tagStoryFileHdr
{
	char szMagic[4]; // Magic number identifer "NST".
	uint32_t uVersion; // Must be set to 0 for now.
	uint32_t uGameTitleAddr; // Address in the file where game title string begins.
	uint32_t uPromptStrAddr; // Address in the file where the prompt string begins (0 for none).
	uint32_t uHelpStrAddr; // Address in the file where the help string begins (0 for none).
	uint16_t uMaskUsedItems; // Mask of used items (also used as a count of the items).
	uint32_t uItemNameAddr; // Address in the file where item name cluster begins.
	uint32_t uInitSceneAddr; // Address in the file where the file name for the initial scene node is found.
} __attribute__((packed, aligned(4))) storyFileHdr;

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
