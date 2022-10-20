/*
Copyright 2022 Lisa-Annette Murray

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
void printLicense (void);

int main (int argc, char *argv[]) {

	// Vars used for story file name.
	// A file given by the user can be detected by the values of the vars. If
	// pszStoryFileName is NULL then no user file is used. The routine used to
	// open the file will fallback to "default.nst" if passed a NULL pointer.
	size_t cchStoryFileName = 0;
	char *pszStoryFileName = NULL;

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
				{ "license", no_argument, 0, 0 },
				{ 0, 0, 0, 0 }
			};

			iLongOpt = 0;
			if ((nOpt = getopt_long(argc, argv, "hf:", optLongOpts, &iLongOpt)) == -1) break;

			switch (nOpt) {
				case 0: // Handle long options.
					switch (iLongOpt) {
						case 1: // Print out build information.
							printBuildInfo();
							if (pszStoryFileName) free(pszStoryFileName);
							return 0;
							break;

						case 3: // Print out the license.
							printLicense();
							if (pszStoryFileName) free(pszStoryFileName);
							return 0;
							break;

						default:
							fprintf(stderr, "Warning: Unknown/unsupported long option index (%d). Attempting to proceed.\n", iLongOpt);
							break;
					}

				case 'h': // Print out help.
					printf("Nightcrawler Text Adventure Engine\n(c) 2022 Lisa-Annette Murray.\nUsage: %s [OPTIONS] ...\n", argv[0]);
					printHelp();
					if (pszStoryFileName) free(pszStoryFileName);
					return 0;
					break;

				case 'f': // Select another file.
					if (pszStoryFileName) break; // Skip if already selected.

					// Check to make sure the file name is of a valid length.
					// This is determined by "X.nst", or any character as well
					// as the file extension.
					if ((cchStoryFileName = strlen(optarg)) < 5) {
						fprintf(stderr, "ERROR: Invalid file: File name too short.\n");
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
					if (pszStoryFileName) free(pszStoryFileName);
					return 1;
					break;
			}
		}
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
	if (cchStoryFileName > 0) free(pszStoryFileName); // Free if user defined.
	if ((g_pGameState->pStory = loadNode(g_pGameState->fpStory, 0, NT_STORY)) == NULL) {
		killGameState();
		exit(EXIT_FAILURE);
	}

	// Display game title.
	if (g_pGameState->pStory->uGameTitleAddr) {
		if (printStrFromStory(g_pGameState->fpStory, g_pGameState->pStory->uGameTitleAddr)) {
			killGameState();
			exit(EXIT_FAILURE);
		}
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
				fprintf(stderr, "ERROR: Exiting due to EOF character.\n");
				free(pszUserInput);
				break;
			}
			if (ferror(stdin)) {
				fprintf(stderr, "ERROR: Standard input error. Try again.\n");
				clearerr(stdin);
				free(pszUserInput);
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
		if ((pparserCmd = parserCmd_inWordSet(pszCmdSubstr, strlen(pszCmdSubstr))) == NULL) {
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
\t-f, --file      Select a story file to load.\n\
\t    --license   Show the BSD 3-Clause license text.\n");

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

	// Print build date.
	printf("\tBuild Date: %s\n", __DATE__);

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
	printf("%s\n\n", __VERSION__);
#else
	printf("GCC %s\n\n", __VERSION__);
#endif

}

void printLicense (void) {

	puts("Nightcrawler Text Adventure Engine\n\
Copyright 2022 Lisa-Annette Murray\n\
\n\
Redistribution and use in source and binary forms, with or without\n\
modification, are permitted provided that the following conditions are met:\n\
\n\
1. Redistributions of source code must retain the above copyright notice, this\n\
list of conditions and the following disclaimer.\n\
\n\
2. Redistributions in binary form must reproduce the above copyright notice,\n\
this list of conditions and the following disclaimer in the documentation\n\
and/or other materials provided with the distribution.\n\
\n\
3. Neither the name of the copyright holder nor the names of its contributors\n\
may be used to endorse or promote products derived from this software without\n\
specific prior written permission.\n\
\n\
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n\
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n\
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n\
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n\
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n\
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n\
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n\
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n\
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n\
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n");

}

// EOF
