# Nightcrawler Text Adventure Engine
(c) 2022 Lisa-Annette Murray Commissioned by Dan Butler/Technomage

## Summary
A simplistic text-adventure engine designed primarily to fit within a miniscule 
binary footprint (i.e. a floppy disk).

Prompt based interaction with predefined keywords used to crawl through a 
binary story file containing the game information.

[[_TOC_]]

## Engine Restrictions and Limits
- Data files are limited to 2GiB in size due to using 32-bit absolute addresses.
- Engine is built around 32/64-bit little-endian architectures using GCC. Other 
systems may not function.

## Platform
As for the actual platform of the engine. The release included by default will 
be for Win32, tested against Wine on Linux & Mac, though the source code will 
be in C and built against Linux GCC for testing, and should be capable of 
compiling on any decently modern 64 or 32-bit OS.

You may be able to get the engine to compile on platforms besides these with 
GCC installed. Early versions were proven to work on Termux with some minor 
makefile changes.

## Tools Needed/Used
- GCC/Clang for Linux compilation.
- mingw32-gcc for Windows compilation.
- gperf used for tokenizer generation.
- fasm used to build story files.

## List of Keywords Recognized by the Engine
- `HELP` Shows engine help & then help included in a story file.
- `MOVE|GO NORTH|SOUTH|EAST|WEST|N|S|E|W` Moves to a different scene.
- `LOOK|EXAMINE [AROUND|NORTH|SOUTH|EAST|WEST|N|S|E|W]` Triggers look node for 
the scene.
- `GET` Adds the item in the scene to the player's inventory.
- `TALK` Triggers the dialogue tree for the scene.
- `USE` Interact with the scene, optionally using an item in the inventory, 
selected automatically.
- `ITEMS|INVENTORY` Displays your inventory.
- `QUIT` Quits the game.

### Unimplemented Keywords (To-Do)
- `SAVE` Saves to a fixed save slot.
- `LOAD` Loads from a fixed save slot.

## License
The Nightcrawler engine is licensed under the BSD 3-Clause license (see 
[LICENSE](LICENSE). Story files compatible with Nightcrawler may be licensed however the 
author of these files sees fit.

*This document is adapted from the original project pitch. See [doc/pitch.txt](doc/pitch.txt)*
