# Nightcrawler Text Adventure Engine
(c) 2022 Lisa-Annette Murray Commissioned by Dan Butler/Technomage

A simplistic text-adventure engine designed primarily to fit within a
miniscule binary footprint (i.e. a floppy disk). Key features to follow.

## Summary
Simple prompt based interaction with predefined keywords used to crawl
through a set of binary data files containing the game information itself.
Game information is split into individual "scene" files that link to one
another, and a "game" file with information like names for the hardcoded
inventory items (by default called "Item 0", "Item 1", ... (up to 15 for use
with a uint16_t bitmask)), and title screen info. Scene files (maybe folders
depending on how things go) will contain dialogue trees, responses to the
various commands like TALK and LOOK, this list of items may be affected by
items in the player's inventory, and/or story flags, of which there will also
be 16 hardcoded. Unlike engines like Ren'py no characters will be defined.
Character naming is to be included in the dialogue script itself. Only one
item/flag per scene is allowed.

Game size and scope will be limited by the engine's above constraints.

### Platform
As for the actual platform of the engine. The release included by default will
be for Win32, tested against Wine on Linux & Mac, though the source code will
be in C and built against Linux GCC for testing, and should be
capable of compiling on any decently modern 64 or 32-bit OS.
Initially DOS compatibility was discussed but Lisa rejected this in favor of
actually getting the project to release.


### Engine File Layout
Binary layout of data files to come...

## Proposed Keywords to be Recognized by the Engine
`MOVE|GO [NORTH|SOUTH|EAST|WEST|N|S|E|W]` (interactive "Where?" prompt if nothing provided).
`LOOK|EXAMINE [NORTH|SOUTH|EAST|WEST|N|S|E|W]` (special "around" field if nothing provided).
`GET` (adds the item in the scene to the player's inventory).
`TALK` (triggers the dialogue tree for the scene).
`USE` (interact with the scene, optionally using an item in the inventory, selected automatically).

## Internal Specifics
Sometimes multiple items may be needed to proceed. This is handled internally
by bitmask operations on the game flags. Different scenes will be connected
by their filenames stored within each scene file. Broken filename will cripple
games, so care must be taken to ensure paths are correct.

*This document is adapted from the original project pitch. See doc/pitch.txt*
