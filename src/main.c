
// Nightcrawler Engine - Main Module
// (C) 2022 Lisa Murray

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include "useractions.h"

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
	if (fgets(szWord, 9, stdin) == NULL) {
		perror("fgets"); exit(errno);
	}
	if (useractions_inWordSet((const char *)szWord, strlen(szWord)) == 0) {
		fprintf(stderr, "\"%s\" is not in the command set.\n", szWord);
	} else {
		printf("Selected \"%s\".\n", szWord);
	}

	return 0;

};


// Print help message.
void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n");

}

// EOF
