/* The original code is public domain -- Will Hartung 4/9/09 */
/* Modifications, public domain as well, by Antti Haapala, 11/10/17
   - Switched to getc on 5/23/19 */

/* Modified by Lisa-Annette 2022-09-26 as kludge for Nightcrawler */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

ssize_t wingetdelim (char **lineptr, size_t *n, int delim, FILE *stream)  {
#ifdef _WINDOWS
	size_t pos;
	int c;

	if (lineptr == NULL || stream == NULL || n == NULL) {
		errno = EINVAL;
		return -1;
	}

	if ((c = getc(stream)) == EOF) return -1;

	if (*lineptr == NULL) {
		if ((*lineptr = malloc(128)) == NULL) return -1;
		*n = 128;
	}

	pos = 0;
	while(c != EOF) {
		if (pos + 1 >= *n) {
			size_t new_size = *n + (*n >> 2);
			if (new_size < 128) new_size = 128;
			char *new_ptr = realloc(*lineptr, new_size);
			if (new_ptr == NULL) return -1;
			*n = new_size;
			*lineptr = new_ptr;
		}

		((unsigned char *)(*lineptr))[pos ++] = c;
		if (c == delim) break;
		c = getc(stream);
	}

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
