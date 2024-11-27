
// Nightcrawler Engine - Story Module

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "input.h"
#include "story.h"

// Local structure that holds magic data<->node size associations for
// `loadNode`.
struct nodeMagicData
{
	const char szMagic[4]; // Buffer that holds the four-byte magic identifier.
	const size_t cbNode; // Size of the node in bytes.
};

const static struct nodeMagicData s_magicData[] = {
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
		puts("DEBUG: Falling back on default story file.");
#endif
		pszFileName = (char *)pszDefFileName;
	}

	// Open the file spec'd by pszFileName.
	if ((fp = fopen(pszFileName, "rb")) == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to open story file: %s\n", __func__, strerror(errno));
		return NULL;
	}

	return fp;
}

// Counterpart to openStoryFile.
int closeStoryFile (FILE *fp) {

	if (fclose(fp) != 0) {
		fprintf(stderr, "ERROR: %s: Failed to close story file: %s\n", __func__, strerror(errno));
		return 1;
	}

	return 0;
}

// Loads a node from a story file.
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

	size_t cbNode; // Size of node.
	void *pNode; // Node pointer.

	// Get size of node by associative array.
	cbNode = s_magicData[nodeType].cbNode;

	// Allocate space for the node.
	if ((pNode = malloc(cbNode)) == NULL) {
		fprintf(stderr, "ERROR: %s: Failed to allocate space for the node: %s\n", __func__, strerror(errno));
		return NULL;
	}

	// Seek to the specified offset in the file.
	if (fseek(fpStory, nodeAddr, SEEK_SET)) {
		fprintf(stderr, "ERROR: %s: Failed to seek to node position 0x%X: %s\n", __func__, nodeAddr, strerror(errno));
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
	if (strncmp((const char *)pNode, s_magicData[nodeType].szMagic, 4) != 0) {
		fprintf(stderr, "ERROR: %s: Node's magic ID is invalid for its type (\"%s\" @ 0x%X).\n", __func__, s_magicData[nodeType].szMagic, nodeAddr);
		free(pNode);
		return NULL;
	}

	return pNode;
}

int loadStrFromStory (FILE *fpStory, const int nStrAddr, size_t *pcchStr, char **ppszStr) {

#ifdef _DEBUG
	printf("DEBUG: %s: Loading a string from @0x%X.\n", __func__, nStrAddr);
#endif

	if (fseek(fpStory, nStrAddr, SEEK_SET)) {
		fprintf(stderr, "ERROR: %s: Failed to seek to the string start: %s\n", __func__, strerror(errno));
		return 1;
	}

	if (wingetdelim(ppszStr, pcchStr, '\0', fpStory) == -1) {
		if (ferror(fpStory)) fprintf(stderr, "ERROR: %s: Error reading string: %s.\n", __func__, strerror(errno));
		if (feof(fpStory)) fprintf(stderr, "ERROR: %s: Error reading string: End of file reached.\n", __func__);
		return 1;
	}

	return 0;
}

int printStrFromStory (FILE *fpStory, const int strAddr) {

	// Verify file pointer.
	if (fpStory == NULL) {
		fprintf(stderr, "ERROR: %s: Bad FILE pointer.\n", __func__);
		return 1;
	}

	// Sanity check string address.
	if (strAddr == 0) {
		fprintf(stderr, "ERROR: %s: Invalid string address.\n", __func__);
		return 1;
	}

	char *pszString = NULL; // String buffer, initialize to NULL.
	size_t cchString; // Size of string buffer in chars.
	size_t cNewLines = 0; // Count of newlines

	if (loadStrFromStory(fpStory, strAddr, &cchString, &pszString)) {
		fprintf(stderr, "ERROR: %s: Unable to load string.\n", __func__);
		return 1;
	}

	// Write string out to stdout; breaking every MAX_DISPLAYED_LINES.
	//puts(pszString);
	for (size_t iChar = 0; iChar < cchString; iChar++) {
		if (pszString[iChar] == '\0') break;
		if (pszString[iChar] == '\n') {
			if (cNewLines++ == MAX_DISPLAYED_LINES) {
				puts("\n-- Press Return to Continue... --");
				getchar();
			}
		}
		putchar(pszString[iChar]);
	}

	// Put trailing newline to replicate behavior of `puts()` as used in prior
	// versions of Nightcrawler.
	putchar('\n');

	// Cleanup string buffer and return successfully.
	memset(pszString, 0, cchString);
	free(pszString);
	return 0;
}