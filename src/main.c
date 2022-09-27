
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

void printHelp (void);

int main (int argc, char *argv[]) {

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
				{ 0, 0, 0, 0 }
			};

			iLongOpt = 0;
			if ((nOpt = getopt_long(argc, argv, "hf:m:", optLongOpts, &iLongOpt)) == -1) break;

			switch (nOpt) {
				case 0: // Handle long options.
					switch (iLongOpt) {
						case 1: // Print out build information.
							puts("Nightcrawler Build Info:");
							printf("\tBuild Type: ");
							#ifdef _DEBUG
							puts("Debug");
							#else
							puts("Release");
							#endif

							printf("\tRelease Platform: ");
							#ifndef _WINDOWS
							#ifdef __amd64__
							puts("Linux 64-bit");
							#else
							puts("Linux 32-bit");
							#endif
							#endif
							#ifdef _WIN32
							puts("Win32");
							#endif
							#ifdef _WIN64
							puts("Win64");
							#endif

							printf("\tBuilt with GCC %s\n", __VERSION__);
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

				case 'i':

				default:
					fprintf(stderr, "Error: Unhandled getopt result (%d).\n", nOpt);
					return 1;
					break;
			}
		}
	}

	// OLD-STYLE MAIN GAME LOOP
	/*
	while (1) {
		//static char szWord[9]; // Length from src/parsercmds.c's MAX_WORD_LENGTH +1.
		//static struct parserCmd *pparserCmd;
		static char *pszUserInput;
		static struct parserCmd *pparserCmd;

		// Clear old word out.
		//memset(szWord, 0, sizeof(szWord));

		// Prompt the user for their next move.
		printf("Your next move? ");
		if (fgets(pszUserInput, 32, stdin) == NULL) {
			perror("fgets"); exit(errno);
		}

		// Process input so there's no trailing newline (just replace the
		// newline char with a NUL).
		// NOTE: To be secure the rest of the string after and including the
		// newline should be initialized to NUL. This is hacky and works for
		// our purposes.
		static char *pszWordNewLine;
		pszWordNewLine = strpbrk(szWord, "\r\n");
		if (pszWordNewLine == NULL) {
			#ifdef _DEBUG
			printf("DEBUG: Skipping excessive input.\n");
			#endif
			continue;
		}
		*pszWordNewLine = 0;

		// Process the user's command.
		if ((pparserCmd = parserCmd_inWordSet((const char *)szWord, strlen(szWord))) == NULL) {
			fprintf(stderr, "\"%s\" is not a valid command. Try HELP.\n", szWord);
		} else {
			#ifdef _DEBUG
			printf("DEBUG: Selected \"%s\" (ID: %d).\n", pparserCmd->name, pparserCmd->uId);
			#endif
			if (procCmdId(pparserCmd->uId) != 0) break;
		}
	}*/

	// Main game loop
	while (1) {
		static char *pszUserInput; // Buffer for user input.
		static size_t cchUserInput; // Bytes allocated by getline.
		static ssize_t cbReadUserInput; // Bytes read from user.

		// (re)initialize the user input buffer to NULL (free()'d at end-of-loop).
		pszUserInput = NULL; cchUserInput = 0;

		// Grab input from user.
		printf("Your next move? ");
		cbReadUserInput = wingetline(&pszUserInput, &cchUserInput, stdin);
		if (ferror(stdin)) {
			fprintf(stderr, "ERROR: Standard input error. Try again.\n");
			clearerr(stdin);
			continue;
		}
		#ifdef _DEBUG
		printf("DEBUG: Allocated %zu chars & read %zu bytes from user: \"%s\".\n", cchUserInput, cbReadUserInput, pszUserInput);
		#endif

		// Tokenize input strings.
		static char *pszCmdSubstr; // Buffer for the command.
		static char *pszParamSubstr; // Buffer for an optional parameter.
		pszCmdSubstr = strtok(pszUserInput, " \t\n");
		pszParamSubstr = strtok(NULL, " \t\n");
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
		if ((pparserCmd = parserCmd_inWordSet(pszUserInput, strlen(pszCmdSubstr)))  == NULL) {
			fprintf(stderr, "\"%s\" is not a valid command. Try HELP.\n", pszCmdSubstr);
		} else {
			#ifdef _DEBUG
			printf("DEBUG: Selected \"%s\" (ID: %d).\n", pparserCmd->name, pparserCmd->uId);
			#endif
			if (procCmdId(pparserCmd->uId) != 0) {
				free(pszUserInput); break;
			}
		}

		free(pszUserInput);
	}

	return 0;

};


// Print help message.
void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n\
\t    --info      Display build info.");

}

// EOF
