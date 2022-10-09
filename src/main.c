
// Nightcrawler Engine - Main Module
// (C) 2022 Lisa Murray

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "wingetline.h"
#include "parsercmds.h"
#include "idprocs.h"
#include "story.h"
#include "gamestate.h"

void printHelp (void);
void printBuildInfo (void);

int main (int argc, char *argv[]) {

	// Vars used for story file name.
	// A file given by the user can be detected by the values of the vars. If
	// pszStoryFileName is NULL then no user file is used. Later on if no file
	// is supplied pszStoryFileName is set to point to pszDefStoryFileName and
	// cchStoryFileName is set to 0.
	size_t cchStoryFileName;
	char *pszStoryFileName = NULL;
	const char pszDefStoryFileName[] = "default.nst";

	// Process args.
	if (argc > 1) {
		while (1) {

			// Variables necessary for processing options.
			static int nOpt; // Buffer for return value from getopt.
			static int iLongOpt; // Long option index.
			// List of available options.
			static struct option optLongOpts[] = {
				{ "help", no_argument, 0, 'h' },
				{ "info", no_argument, 0, 0 },
				{ "file", required_argument, 0, 'f' },
				{ 0, 0, 0, 0 }
			};

			iLongOpt = 0;
			if ((nOpt = getopt_long(argc, argv, "hf:", optLongOpts, &iLongOpt)) == -1) break;

			switch (nOpt) {
				case 0: // Handle long options.
					switch (iLongOpt) {
						case 1: // Print out build information.
							printBuildInfo();
							return 0;
							break;

						default:
							fprintf(stderr, "Warning: Unknown/unsupported long option index (%d). Attempting to proceed.\n", iLongOpt);
							break;
					}

				case 'h': // Print out help.
					printf("Nightcrawler Text Adventure Engine\n(c) 2022 Lisa-Annette Murray\nUsage: %s [OPTIONS] ...\n", argv[0]);
					printHelp();
					return 0;
					break;

				case 'f': // Select another file.
					if (pszStoryFileName) break; // Skip if already selected.

					// Check to make sure the file name is of a valid length.
					// This is determined by "X.nst", or any character as well
					// as the file extension.
					if ((cchStoryFileName = strlen(optarg)) < 5) {
						fprintf(stderr, "Invalid file: File name too short.\n");
						exit(EXIT_FAILURE);
					}

					// Allocate space for the name.
					if ((pszStoryFileName = malloc(cchStoryFileName * sizeof(char))) == NULL) {
						perror("Failed to obtain file name: malloc");
						exit(EXIT_FAILURE);
					}

					// Copy the name into the allocated buffer.
					if (!strncpy(pszStoryFileName, optarg, cchStoryFileName)) {
						perror("Failed to obtain file name: strncpy");
						free(pszStoryFileName);
						exit(EXIT_FAILURE);
					}
					break;

				default:
					fprintf(stderr, "Error: Unhandled getopt result (%d).\n", nOpt);
					return 1;
					break;
			}
		}
	}

	// If a file is specified use that, otherwise use default.
	// If no user file is specified set the story file name's buffer's pointer
	// to the (constant) default string.
	if (pszStoryFileName == NULL) {
		pszStoryFileName = pszDefStoryFileName;
		cchStoryFileName = 0; // Set to zero to indicate the fallback to default.
	}

#ifdef _DEBUG
	printf("DEBUG: Selected story file: \"%s\".\n", pszStoryFileName);
#endif

	// Initialize game state struct.
	if (initGameState()) exit(EXIT_FAILURE);

#ifdef _DEBUG
	printf("DEBUG: Initialized game state struct.\n");
#endif

	// Load game file.
	if ((g_pGameState->fpStory = openStoryFile(pszStoryFileName)) == NULL) return 1;
	if (cchStoryFileName != 0) free(pszStoryFileName); // Free if user defined.
	if ((g_pGameState->pStory = loadNode(g_pGameState->fpStory, 0, NT_STORY)) == NULL) {
		killGameState();
		exit(EXIT_FAILURE);
	}

	// Display game title.
	if (printStrFromStory(g_pGameState->fpStory, g_pGameState->pStory->uGameTitleAddr)) {
		killGameState();
		exit(EXIT_FAILURE);
	}

	// Load necessary strings from the file.
	if (getStrsFromStoryFile()) {
		killGameState();
		exit(EXIT_FAILURE);
	}

	// Load initial scene.
	g_pGameState->uCurSceneAddr = g_pGameState->pStory->uInitSceneAddr;
	g_pGameState->pScene = loadNode(g_pGameState->fpStory, g_pGameState->uCurSceneAddr, NT_SCENE);
	if (g_pGameState->pScene == NULL) {
		killGameState();
		exit(EXIT_FAILURE);
	}

	// Main game loop.
	while (1) {
		static char *pszUserInput; // Buffer for user input.
		static size_t cchUserInput; // Bytes allocated by getline.
		static ssize_t cbReadUserInput; // Bytes read from user.

		// (re)initialize the user input buffer to NULL (free()'d at end-of-loop).
		pszUserInput = NULL; cchUserInput = 0;

		// Grab input from user.
		printf("%s", g_pGameState->pszPromptString);
		cbReadUserInput = wingetline(&pszUserInput, &cchUserInput, stdin);
		if (cbReadUserInput == -1) {
			if (feof(stdin)) {
				fprintf(stderr, "Exiting due to EOF character.\n");
				free(pszUserInput);
				break;
			}
			if (ferror(stdin)) {
				fprintf(stderr, "ERROR: Standard input error. Try again.\n");
				clearerr(stdin);
				continue;
			}
		}
#ifdef _DEBUG
		printf("DEBUG: Allocated %zu chars & read %zu bytes from user: \"%s\".\n", cchUserInput, cbReadUserInput, pszUserInput);
#endif

		// Tokenize input strings.
		static char *pszCmdSubstr; // Buffer for the command.
		static char *pszParamSubstr; // Buffer for an optional parameter.
		pszCmdSubstr = strtok(pszUserInput, " \t\n");
		pszParamSubstr = strtok(NULL, "\t\n");
#ifdef _DEBUG
		printf("DEBUG: CmdSubstr: \"%s\", ParamSubstr: \"%s\".\n", pszCmdSubstr, pszParamSubstr);
#endif

		// Skip processing if nothing was passed.
		if (pszCmdSubstr == NULL) {
			free(pszUserInput);
			continue;
		}

		// Process command.
		static struct parserCmd *pparserCmd;
		if ((pparserCmd = parserCmd_inWordSet(pszCmdSubstr, strlen(pszCmdSubstr)))  == NULL) {
			fprintf(stderr, "\"%s\" is not a valid command. Try HELP.\n", pszCmdSubstr);
		} else {
#ifdef _DEBUG
			printf("DEBUG: Selected \"%s\" (ID: %d).\n", pparserCmd->name, pparserCmd->uId);
#endif
			if (procCmdId(pparserCmd->uId, pszParamSubstr) != 0) {
				free(pszUserInput);
				break;
			}
		}

		free(pszUserInput);
	}

	// Destroy game state object.
	killGameState();

	return 0;

};

// Print help message.
void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n\
\t    --info      Display build info.\n\
\t-f, --file      Select a story file to load.\n");

}

// Prints out build info.
void printBuildInfo (void) {

	puts("Nightcrawler Build Info:");

	// Print build type.
	printf("\tBuild Type: ");
#ifdef _DEBUG
	puts("Debug");
#else
	puts("Release");
#endif

	// Release platform.
	printf("\tRelease Platform: ");
#ifndef _WINDOWS
	puts("Linux");
#else
#	ifdef _WIN64
	puts("Win64");
#	else
	puts("Win32");
#	endif
#endif

	// Compiler version.
	printf("\tBuilt with: ");
#ifdef __clang__
	// Clang includes "Clang <ver>" in its definition of __VERSION__.
	printf(__VERSION__);
#else
	printf("GCC %s\n", __VERSION__);
#endif

}

// EOF
