
// Nightcrawler Engine - Windows Getline Kludge Header

#ifndef __WINGETLINE_H__
#define __WINGETLINE_H__ 1

#include <stdint.h>
#include <stdio.h>

ssize_t wingetdelim (char **lineptr, size_t *n, int delim, FILE *stream);
ssize_t wingetline (char **lineptr, size_t *n, FILE *stream);

#endif /* __WINGETLINE_H__ */
