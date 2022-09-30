
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "story.h"

const char s_pszStoryMagicId[4] = "NST";

// Open story file.
// Normally this kind of thing would be handled elsewhere and you'd just ask
// the user to open their own file and pass a FILE * to loadStoryHeader, but
// it's defined here as its own function in case special handling is required
// later, and to keep things consistent.
inline FILE *openStoryFile (const char *pszFileName) {

	// Open the file spec'd by pszFileName.
	FILE *fp;
	if ((fp = fopen(pszFileName, "r")) == NULL) {
		perror("Failed to open story file");
		return NULL;
	}

	return fp;

}

inline int closeStoryFile (FILE *fp) {

	if (fclose(fp) != 0) {
		perror("Failed to close the story file");
		return 1;
	}

	return 0;

}

struct storyFileHdr *loadStoryHdr (FILE *fp) {

	if (fp == NULL) {
		fprintf(stderr, "ERROR: loadStoryHdr: file pointer invalid.\n");
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
		return NULL;
	}

	// Verify as story file.
	if (strcmp(pStory->szMagic, s_pszStoryMagicId) != 0) {
		fprintf(stderr, "Invalid story file.\n");
		free(pStory);
		return NULL;
	}

	return pStory;

}
