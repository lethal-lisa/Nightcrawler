
// Nightcrawler Engine - User Actions Header

#ifndef __USERACTIONS_H__
#define _USERACTIONS_H_ 1

#include <string.h>

struct parserCmd
{
	const char *name;
	unsigned int uId;
};

const struct parserCmd *parserCmd_inWordSet (register const char *str, register size_t len);

#endif /* _USERACTIONS_H_ */
