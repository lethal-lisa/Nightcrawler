
// Nightcrawler Engien - Save/Load module header.

#ifndef SAVELOAD_H_
#define SAVELOAD_H_ 1

#include <stdint.h>

// Saves the game state to a file.
int saveGame (void);

// Loads the game state from a file.
int loadGame (void);

#endif /* SAVELOAD_H_ */