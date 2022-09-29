
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "story.h"

const char s_pszStoryMagicId[4] = "NST";

struct storyFileHdr *loadStoryFile (const char *pszFileName) {

	// Open the file spec'd by pszFileName.
	FILE *fp;
	if ((fp = fopen(pszFileName, "r")) == NULL) {
		perror("Failed to open story file");
		return NULL;
	}

	// Allocate memory space for the story file header object.
	struct storyFileHdr *pStory;
	pStory = malloc(sizeof(pStory));
	if (pStory == NULL) {
		perror("malloc");
		return NULL;
	}

	// Read file header into the structure.
	if (fread(pStory, sizeof(pStory), 1, fp) != 1) {
		if (ferror(fp)) perror("Read error");
		if (feof(fp)) fprintf(stderr, "Invalid story file.\n");
		free(pStory);
		fclose(fp);
		return NULL;
	}

	// Verify as story file.
	if (strcmp(pStory->szMagic, s_pszStoryMagicId) != 0) {
		fprintf(stderr, "Invalid story file.\n");
		free(pStory);
		fclose(fp);
		return NULL;
	}

	return pStory;

}
