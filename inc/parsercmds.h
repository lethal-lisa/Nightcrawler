
// Nightcrawler Engine - Parser Commands Header

#ifndef __PARSERCMDS_H__
#define __PARSERCMDS_H__ 1

#include <string.h>

// Parser command IDs.
#define CI_HELP 0
#define CI_MOVE 1
#define CI_LOOK 2
#define CI_GET 3
#define CI_TALK 4
#define CI_USE 5
#define CI_INTERACT 6
#define CI_NORTH 100
#define CI_SOUTH 101
#define CI_EAST 102
#define CI_WEST 103

struct parserCmd
{
	const char *name;
	unsigned int uId;
};

const struct parserCmd *parserCmd_inWordSet (register const char *str, register size_t len);

#endif /* __PARSERCMDS_H__ */
