/* The original code is public domain -- Will Hartung 4/9/09 */
/* Modifications, public domain as well, by Antti Haapala, 11/10/17
   - Switched to getc on 5/23/19 */

/* Modified by Lisa-Annette 2022-09-26 as kludge for Nightcrawler */

/* Modification Details */
/* Modifications fall under BSD 3-Clause, as per use in Nightcrawler.
 * - Changed to wrap native getline and getdelim on non-Windows (detected via
 *   _WINDOWS define being defined or not).
 * - Modifications to conform to the style of the rest of the Nightcrawler
 *   codebase.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <ctype.h>

// Size to allocate to when using Windows version.
#ifdef _WINDOWS
#	define WGL_DEF_CBLINEALLOC 128
#endif

ssize_t wingetdelim (char **lineptr, size_t *n, int delim, FILE *stream)  {
#ifdef _WINDOWS
	size_t pos = 0;
	int c;

	// Verify input.
	if (lineptr == NULL || stream == NULL || n == NULL) {
		errno = EINVAL;
		return -1;
	}

	// Get char & exit if EOF.
	if ((c = getc(stream)) == EOF) return -1;

	// Allocate space for lineptr if lineptr isn't already a pointer.
	if (*lineptr == NULL) {
		if ((*lineptr = malloc(WGL_DEF_CBLINEALLOC)) == NULL) return -1;
		*n = WGL_DEF_CBLINEALLOC;
	}

	while (c != EOF) {

		// Reallocate if buffer too small.
		if (pos + 1 >= *n) {
			// Get the size to realloc to by doubling the original buffer.
			size_t cbNew = *n + (*n >> 2);
			if (cbNew < WGL_DEF_CBLINEALLOC) cbNew = WGL_DEF_CBLINEALLOC;

			// Perform realloc.
			char *pchNew = realloc(*lineptr, cbNew);
			if (pchNew == NULL) return -1;

			// Set new size and pointer.
			*n = cbNew;
			*lineptr = pchNew;
		}

		if ((((unsigned char *)(*lineptr))[pos++] = c) == delim) break;
		c = getc(stream);
	}

	// Set last byte to NUL and return bytes read.
	(*lineptr)[pos] = '\0';
	return pos;
#else
	return getdelim(lineptr, n, delim, stream);
#endif
}

ssize_t wingetline (char **lineptr, size_t *n, FILE *stream) {
#ifdef _WINDOWS
	return wingetdelim(lineptr, n, '\n', stream);
#else
	return getline(lineptr, n, stream);
#endif
}

// Prompt for user's choice as an unsigned int.
int promptUserForOpt (unsigned int *puUserInput) {

	char *pszUserInput;
	size_t cchUserInput;
	char *endptr; // End pointer used by strtoul.

	while (1) {
		pszUserInput = NULL;
		cchUserInput = 0;

		// Get input from user.
		printf("Select an option: ");
		if (wingetline(&pszUserInput, &cchUserInput, stdin) == -1) {
			if (feof(stdin)) {
				fprintf(stderr, "ERROR: %s: Encountered EOF.\n", __func__);
				free(pszUserInput);
				return 1;
			}
			if (ferror(stdin)) {
				fprintf(stderr, "ERROR: %s: Standard input error. Try again.\n", __func__);
				free(pszUserInput);
				clearerr(stdin);
				continue;
			}
		}

#ifdef _DEBUG
		printf("DEBUG: Allocated %zu chars.\n", cchUserInput);
#endif

		// Convert to uint.
		errno = 0;
		*puUserInput = strtoul(pszUserInput, &endptr, 10);
		if (errno != 0) {
			fprintf(stderr, "ERROR: %s: Unable to convert user input: %s\n", __func__, strerror(errno));
			free(pszUserInput);
			return 1;
		}

		// If no input, try again.
		if (endptr == pszUserInput) {
			puts("Invalid input.");
			free(pszUserInput);
			continue;
		}

		free(pszUserInput);
		return 0;
	}
}

int confirmYesNo (const char *pszPrompt) {

	printf("%s [y/N] ", pszPrompt);

	// Get first char and eat chars until newline.
	int chFirst = toupper(getchar());
	if (chFirst == '\n') return 0;
	while (getchar() != '\n');

	// Return 1 if 'Y'.
	if (chFirst == 'Y') return 1;
	return 0;
}