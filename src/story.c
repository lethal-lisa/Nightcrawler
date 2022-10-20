
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wingetline.h"
#include "story.h"

// Open story file.
// Normally this kind of thing would be handled elsewhere and you'd just ask
// the user to open their own file and pass a FILE * to loadStoryHeader, but
// it's defined here as its own function in case special handling is required
// later, and to keep things consistent.
FILE *openStoryFile (char *pszFileName) {

	const char pszDefFileName[] = "default.nst";
	FILE *fp;

	if (pszFileName == NULL) {
#ifdef _DEBUG
		printf("DEBUG: Falling back on default story file.\n");
#endif
		pszFileName = pszDefFileName;
	}

	// Open the file spec'd by pszFileName.
	if ((fp = fopen(pszFileName, "r")) == NULL) {
		perror("Failed to open story file");
		return NULL;
	}

	return fp;

}

// Counterpart to openStoryFile.
int closeStoryFile (FILE *fp) {

	if (fclose(fp) != 0) {
		perror("Failed to close story file");
		return 1;
	}

	return 0;

}


// NOTE: Be extra sure you know what you're doing before you start playing
// around with this one. This function can return an arbitrary pointer if
// something goes wrong.
void *loadNode (FILE *fpStory, const int nodeAddr, const int nodeType) {

	if (fpStory == NULL) {
		fprintf(stderr, "ERROR: loadNode: Bad FILE pointer.\n");
		return NULL;
	}

	// Error checking for this array is done by the default statement of the
	// following switch block.
	const static char *szMagic[] = {
		"NST",
		"NSC",
		"MOV",
		"LOK",
		"TLK",
		"DIA",
		"DOL",
		"OPT",
		"USE"
	};

	size_t cbNode; // Size of node struct.
	void *pNode; // Pointer to node struct.

	// Get size of the node per node type.
	switch (nodeType) {
		case NT_STORY:
			cbNode = sizeof(storyFileHdr);
			break;

		case NT_SCENE:
			cbNode = sizeof(sceneNodeHdr);
			break;

		case NT_MOVE:
			cbNode = sizeof(scene_MoveCluster);
			break;

		case NT_LOOK:
			cbNode = sizeof(scene_LookCluster);
			break;

		case NT_TALK:
			cbNode = sizeof(scene_TalkCluster);
			break;

		case NT_DIA:
			cbNode = sizeof(talk_DiaNode);
			break;

		case NT_DOL:
			cbNode = sizeof(dia_OptList);
			break;

		case NT_OPT:
			cbNode = sizeof(dia_OptNode);
			break;

		case NT_USE:
			cbNode = sizeof(scene_UseCluster);
			break;

		default:
			fprintf(stderr, "ERROR: loadNode: Unknown node type (%d).\n", nodeType);
			return NULL;
	}

	// Seek to the specified offset in the file.
	if (fseek(fpStory, nodeAddr, SEEK_SET)) {
		perror("ERROR: loadNode: fseek fail.");
		return NULL;
	}

	// Allocate space for the node.
	if ((pNode = malloc(cbNode)) == NULL) {
		perror("ERROR: loadNode: malloc fail.");
		return NULL;
	}

	// Read in the node.
	if (fread(pNode, cbNode, 1, fpStory) != 1) {
		if (ferror(fpStory)) perror("ERROR: loadNode: fread tripped file error.");
		if (feof(fpStory)) fprintf(stderr, "ERROR: loadNode: fread reached EOF.\n");
		free(pNode);
		return NULL;
	}

	// Verify node as type.
	if (strncmp((const char *)pNode, szMagic[nodeType], 4) != 0) {
		fprintf(stderr, "ERROR: loadNode: Node's magic ID is invalid for its type.\n");
		free(pNode);
		return NULL;
	}

	return pNode;

}

int printStrFromStory (FILE *fpStory, const int strAddr) {

	// Verify file pointer.
	if (fpStory == NULL) {
		fprintf(stderr, "ERROR: printStrFromStory: Bad FILE pointer.\n");
		return 1;
	}

	// Seek to string position.
	if (fseek(fpStory, strAddr, SEEK_SET)) {
		perror("ERROR: printStrFromStory: fseek failed");
		return 1;
	}

	char *pszString = NULL;
	size_t cchString;
	ssize_t cbReadString;

	// Read string.
	if ((cbReadString = wingetdelim(&pszString, &cchString, '\0', fpStory)) == -1) {
		if (ferror(fpStory)) perror("ERROR: printStrFromStory: Error reading string");
		if (feof(fpStory)) fprintf(stderr, "WARN: printStrFromStory: Error reading string: End of file reached.\n");
		return 1;
	}

	// Write string out to stdout.
	puts(pszString);
	free(pszString);

	return 0;

}
