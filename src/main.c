
// C CLI Project Template - Main Module
// (C) 2022 Lisa Murray

#include <stdio.h>
#include <string.h>
#include <getopt.h>

inline void printHelp (void);

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
					printf("Usage: %s [OPTIONS] ...\n", argv[0]);
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

	return 0;

};


// Print help message.
inline void printHelp (void) {

	puts("\n\
\t-h, --help      Show this help.\n");

}

// EOF
