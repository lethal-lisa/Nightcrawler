
// Nightcrawler Engine - Parser Command ID Processer Header

#ifndef IDPROCS_H_
#define IDPROCS_H_ 1

// Process a command by its ID. Command IDs are in parsercmds.h.
int procCmdId (const unsigned int uId, const char *pszParam);

// Process a LOOK command.
int procLook (const char *pszParam);

// Process a MOVE command.
int procMove (const char *pszParam);

// Process a GET command.
int procGet (const char *pszParam);

// Process a USE command.
int procUse (const char *pszParam);

// Process a TALK command.
int procTalk (const char *pszParam);

#endif /* IDPROCS_H_ */