
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

void printHelp (void);
void printBuildInfo(void);
void printLicense (void);

int main (int argc, char *argv[]) {

	// Sizes and buffers for the input and output file names.
	size_t cchOutputFileName = 0;
	char *pszOutputFileName = NULL;
	size_t cchInputFileName = 0;
	char *pszInputFileName = NULL;

	// Process args.
	if (argc > 1) {
		while (1) {

			// Variables necessary for processing options.
			static int nOpt; // Buffer for return value from getopt.
			static int iLongOpt; // Long option index.
			// List of available options.
			const static struct option optLongOpts[] = {
				{ "help", no_argument, 0, 'h' },
				{ "info", no_argument, 0, 0 },
				{ "input", required_argument, 0, 'i' },
				{ "output", required_argument, 0, 'o' },
				{ "license", no_argument, 0, 0 },
				{ 0, 0, 0, 0 }
			};

			iLongOpt = 0;
			if ((nOpt = getopt_long(argc, argv, "hi:o:", optLongOpts, &iLongOpt)) == -1) break;

			switch (nOpt) {
				case 0: // Handle long options.
					switch (iLongOpt) {
						case 1: // Print out build information.
							printBuildInfo();
							if (pszOutputFileName) free(pszOutputFileName);
							if (pszInputFileName) free(pszInputFileName);
							return 0;
							break;

						case 4: // Print out the license.
							printLicense();
							if (pszOutputFileName) free(pszOutputFileName);
							if (pszInputFileName) free(pszInputFileName);
							return 0;
							break;

						default:
							fprintf(stderr, "Warning: Unknown/unsupported long option index (%d). Attempting to proceed.\n", iLongOpt);
							break;
					}

				case 'h': // Print out help.
					printf("Nightcrawler Packer\n(c) 2022-2023 Lisa-Annette Murray.\nUsage: %s [OPTIONS] ...\n", argv[0]);
					printHelp();
					return 0;
					break;

				case 'i': // Select input file.
					if (pszInputFileName) break; // Skip if already selected.

					// Get string length.
					cchInputFileName = strlen(optarg);

					// Allocate space for the name.
					if ((pszInputFileName = malloc(cchInputFileName * sizeof(char))) == NULL) {
						perror("Failed to obtain input file name: malloc");
						if (pszOutputFileName) free(pszOutputFileName);
						exit(EXIT_FAILURE);
					}

					// Copy the name into the allocated buffer.
					if (!strncpy(pszInputFileName, optarg, cchInputFileName)) {
						perror("Failed to obtain input file name: strncpy");
						free(pszInputFileName);
						if (pszOutputFileName) free(pszOutputFileName);
						exit(EXIT_FAILURE);
					}
					break;

				case 'o':
					if (pszOutputFileName) break; // Skip if already selected.

					cchOutputFileName = strlen(optarg);

					if ((pszOutputFileName = malloc(cchOutputFileName * sizeof(char))) == NULL) {
						perror("Failed to obtain output file name: malloc");
						if (pszInputFileName) free(pszInputFileName);
						exit(EXIT_FAILURE);
					}

					if (!strncpy(pszOutputFileName, optarg, cchOutputFileName)) {
						perror("Failed to obtain output file name: strncpy");
						free(pszOutputFileName);
						if (pszInputFileName) free(pszInputFileName);
						exit(EXIT_FAILURE);
					}
					break;

				default:
					fprintf(stderr, "Error: Unhandled getopt result (%d).\n", nOpt);
					if (pszOutputFileName) free(pszOutputFileName);
					if (pszInputFileName) free(pszInputFileName);
					return 1;
					break;
			}
		}
	}

#ifdef _DEBUG
	fprintf(stderr, "DEBUG: %s: Selected files:\n\tInput:  \"%s\"\n\tOutput: \"%s\"\n", __func__,pszInputFileName, pszOutputFileName);
#endif

	// Do main routine.

	// Verify that we have an input file.
	if (pszInputFileName == NULL) {
		fprintf(stderr, "ERROR: %s: No input file.\n", __func__);
		if (pszOutputFileName) free(pszOutputFileName);
		exit(EXIT_FAILURE);
	}

	// Cleanup.
	if (pszOutputFileName) free(pszOutputFileName);
	if (pszInputFileName) free(pszInputFileName);
	exit(EXIT_SUCCESS);
}

// Print help message.
void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n\
\t    --info      Display build info.\n\
\t-i, --input     Select an input file to parse (required).\n\
\t-o, --output    Select an output file to parse. If not\n\
\t                specified this will be extrapolated from input\n\
\t                file's name.\n\
\t    --license   Show the BSD 3-Clause license text.\n");
}

// Prints out build info.
void printBuildInfo (void) {

	puts("Nightcrawler Packer Build Info:");

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

	puts("Nightcrawler Packer\n\
Copyright 2022-2023 Lisa-Annette Murray\n\
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

