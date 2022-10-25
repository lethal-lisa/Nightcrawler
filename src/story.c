
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "wingetline.h"
#include "story.h"

struct nodeMagicData
{
	const char szMagic[4];
	const size_t cbNode;
};

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
		fprintf(stderr, "ERROR: %s: Bad FILE pointer.\n", __func__);
		return NULL;
	}

	if ((nodeType > NT_DTH) || (nodeType < 0)) {
		fprintf(stderr, "ERROR: %s: Unknown node type %d.\n", __func__, nodeType);
		return NULL;
	}

	const static struct nodeMagicData magicData[] = {
		{ "NST", sizeof(storyFileHdr) },
		{ "NSC", sizeof(sceneNodeHdr) },
		{ "MOV", sizeof(scene_MoveCluster) },
		{ "LOK", sizeof(scene_LookCluster) },
		{ "TLK", sizeof(scene_TalkCluster) },
		{ "DIA", sizeof(talk_DiaNode) },
		{ "DOL", sizeof(dia_OptList) },
		{ "OPT", sizeof(dia_OptNode) },
		{ "USE", sizeof(scene_UseCluster) },
		{ "WIN", sizeof(winNodeHdr) },
		{ "DTH", sizeof(dthNodeHdr) }
	};

	size_t cbNode; // Size of node.
	void *pNode; // Node pointer.

	cbNode = magicData[nodeType].cbNode;

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
		if (ferror(fpStory)) fprintf(stderr, "ERROR: %s: fread tripped file I/O error.\n", __func__);
		if (feof(fpStory)) fprintf(stderr, "ERROR: %s: fread reached EOF.\n", __func__);
		free(pNode);
		return NULL;
	}

	// Verify node as type.
	if (strncmp((const char *)pNode, magicData[nodeType].szMagic, 4) != 0) {
		fprintf(stderr, "ERROR: %s: Node's magic ID is invalid for its type (\"%s\" @ 0x%X).\n", __func__, magicData[nodeType].szMagic, nodeAddr);
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

	// Read string.
	if (wingetdelim(&pszString, &cchString, '\0', fpStory) == -1) {
		if (ferror(fpStory)) perror("ERROR: printStrFromStory: Error reading string");
		if (feof(fpStory)) fprintf(stderr, "WARN: printStrFromStory: Error reading string: End of file reached.\n");
		return 1;
	}

	// Write string out to stdout.
	puts(pszString);
	free(pszString);

	return 0;

}
