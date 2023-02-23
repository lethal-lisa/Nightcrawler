
// Nightcrawler Engine - Parser Commands Header

#ifndef PARSERCMDS_H_
#define PARSERCMDS_H_ 1

#include <string.h>

// Parser command IDs.
#define CI_HELP 0
#define CI_MOVE 1
#define CI_LOOK 2
#define CI_GET 3
#define CI_TALK 4
#define CI_USE 5
#define CI_ITEMS 6
#define CI_SAVE 50
#define CI_LOAD 51
#define CI_QUIT 99
#define CI_AROUND 100
#define CI_NORTH 101
#define CI_SOUTH 102
#define CI_EAST 103
#define CI_WEST 104

// Parser command result structure.
struct parserCmd
{
	const char *name;
	unsigned int uId;
};

const struct parserCmd *parserCmd_inWordSet (register const char *str, register size_t len);

#endif /* PARSERCMDS_H_ */
