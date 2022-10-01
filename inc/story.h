
// Nightcrawler Engine - Story File Header

#ifndef __STORY_H__
#define __STORY_H__ 1

// Nightcrawler story files contain information about the game itself: things
// like the game's "title screen" (not actually a title screen, just a string
// with intoductory text), names for the items used in the game, and the
// starting scene.

#include <stdint.h>

struct storyFileHdr
{
	char szMagic[4]; // Magic number identifer "NST".
	uint32_t uVersion; // Must be set to 0 for now.
	uint32_t uGameTitleAddr; // Address in the file where game title string begins.
	uint16_t uMaskUsedItems; // Mask of used items (also used as a count of the items).
	uint32_t uItemNameAddr; // Address in the file where item name cluster begins.
	uint32_t uInitSceneAddr; // Address in the file where the file name for the initial scene node is found.
};

FILE *openStoryFile (const char *pszFileName);
int closeStoryFile (FILE *fp);
struct storyFileHdr *loadStoryHdr (FILE *fp);

#endif /* __STORY_H__ */
