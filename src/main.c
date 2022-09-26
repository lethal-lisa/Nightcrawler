
// Nightcrawler Engine - Main Module
// (C) 2022 Lisa Murray

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

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
				{ 0, 0, 0, 0 }
			};

			iLongOpt = 0;
			if ((nOpt = getopt_long(argc, argv, "hf:m:", optLongOpts, &iLongOpt)) == -1) break;

			switch (nOpt) {
				case 0: // Handle long options.
					switch (iLongOpt) {
						default:
							fprintf(stderr, "Warning: Unknown/unsupported long option index (%d). Attempting to proceed.\n", iLongOpt);
							break;
					}

				case 'h': // Print out help.
					printf("Nightcrawler Text Adventure Engine\n(c) 2022 Lisa-Annette Murray\nUsage: %s [OPTIONS] ...\n", argv[0]);
					printHelp();
					return 0;
					break;

				default:
					fprintf(stderr, "Error: Unhandled getopt result (%d).\n", nOpt);
					return 1;
					break;
			}
		}
	}

	char szWord[9]; // Length from src/useractions.c's MAX_WORD_LENGTH +1.
	struct parserCmd *pparserCmd;

	// Prompt the user for their next move.
	printf("Your next move? ");
	if (fgets(szWord, 9, stdin) == NULL) {
		perror("fgets"); exit(errno);
	}

	// Process input so there's no trailing newline (just replace the
	// newline char with a NUL).
	// NOTE: To be secure the rest of the string after and including the
	// newline should be initialized to NUL. This is hacky and works for
	// our purposes.
	char *pszWordNewLine = strpbrk(szWord, "\r\n");
	if (pszWordNewLine != NULL) *pszWordNewLine = 0;

	// Process the user's command.
	if ((pparserCmd = parserCmd_inWordSet((const char *)szWord, strlen(szWord))) == NULL) {
		fprintf(stderr, "\"%s\" is not a valid option. Try HELP.\n", szWord);
	} else {
		printf("DEBUG: Selected \"%s\" (ID: %d).\n", pparserCmd->name, pparserCmd->uId);
	}

	procCmdId(pparserCmd->uId);

	return 0;

};


// Print help message.
void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n");

}

// EOF
