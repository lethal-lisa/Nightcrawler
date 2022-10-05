
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "story.h"
#include "scene.h"

const char s_pszStoryMagicId[4] = "NST";
const char s_pszSceneMagicId[4] = "NSC";

// Open story file.
// Normally this kind of thing would be handled elsewhere and you'd just ask
// the user to open their own file and pass a FILE * to loadStoryHeader, but
// it's defined here as its own function in case special handling is required
// later, and to keep things consistent.
FILE *openStoryFile (const char *pszFileName) {

	// Open the file spec'd by pszFileName.
	FILE *fp;
	if ((fp = fopen(pszFileName, "r")) == NULL) {
		perror("Failed to open story file");
		return NULL;
	}

	return fp;

}

int closeStoryFile (FILE *fp) {

	if (fclose(fp) != 0) {
		perror("Failed to close the story file");
		return 1;
	}

	return 0;

}

storyFileHdr *loadStoryHdr (FILE *fp) {

	if (fp == NULL) {
		fprintf(stderr, "ERROR: loadStoryHdr: file pointer invalid.\n");
		return NULL;
	}

	// Allocate memory space for the story file header object.
	storyFileHdr *pStory;
	pStory = malloc(sizeof(storyFileHdr));
	if (pStory == NULL) {
		perror("malloc");
		return NULL;
	}

	// Set file position to zero.
	if (fseek(fp, 0, SEEK_SET)) {
		perror("fseek");
		return NULL;
	}

	// Read file header into the structure.
	if (fread(pStory, sizeof(storyFileHdr), 1, fp) != 1) {
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

sceneNodeHdr *loadSceneHdr (FILE *fp, const int uSceneAddr) {

	if (fp == NULL) {
		fprintf(stderr, "ERROR: loadSceneHdr: file pointer invalid.\n");
		return NULL;
	}

	// Allocate memory for the scene node structure.
	sceneNodeHdr *pScene;
	pScene = malloc(sizeof(sceneNodeHdr));
	if (pScene == NULL) {
		perror("malloc");
		return NULL;
	}

	// Set file position to start of node.
	if (fseek(fp, uSceneAddr, SEEK_SET)) {
		perror("fseek");
		free(pScene);
		return NULL;
	}

	// Read node in.
	if (fread(pScene, sizeof(sceneNodeHdr), 1, fp) != 1) {
		if (ferror(fp)) perror("Read error");
		if (feof(fp)) fprintf(stderr, "Invalid scene node address.\n");
		free(pScene);
		return NULL;
	}

	// Verify as scene node.
	if (strcmp(pScene->szMagic, s_pszSceneMagicId) != 0) {
		fprintf(stderr, "Invalid scene node ID.\n");
		free(pScene);
		return NULL;
	}

	return pScene;

}
