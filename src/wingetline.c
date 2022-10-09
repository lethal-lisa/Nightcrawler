/* The original code is public domain -- Will Hartung 4/9/09 */
/* Modifications, public domain as well, by Antti Haapala, 11/10/17
   - Switched to getc on 5/23/19 */

/* Modified by Lisa-Annette 2022-09-26 as kludge for Nightcrawler */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

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

	//pos = 0;
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

		//((unsigned char *)(*lineptr))[pos++] = c;
		//if (c == delim) break;
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
