
// Nightcrawler Engine - Input Routines Header

#ifndef __INPUT_H__
#define __INPUT_H__ 1

#include <stdint.h>
#include <stdio.h>

#define YN_YES 1
#define YN_NO  0

// This serves to implement getline and getdelim on platforms that
// normally don't support them and wrap them on platforms that do.

ssize_t wingetdelim (char **lineptr, size_t *n, int delim, FILE *stream);
ssize_t wingetline (char **lineptr, size_t *n, FILE *stream);

// Gets the user's choice as an unsigned integer.
int promptUserForInt (unsigned int *puUserInput);

// Confirms a prompt with a [y/N] (no is default) suffix.
// Returns 1 if Yes, 0 elsewise.
int confirmYesNo (const char *pszPrompt);

#endif /* __INPUT_H__ */
