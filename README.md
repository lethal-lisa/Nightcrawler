# Nightcrawler Text Adventure Engine

(c) 2022-2023 Lisa-Annette Murray Commissioned by Dan Butler from Techno Mage

## Summary

A simplistic text-adventure engine designed primarily to fit within a miniscule 
binary footprint (i.e. a floppy disk) and uses simple prompt based interaction
with predefined keywords used to crawl through a binary story file containing
the game information.

### Documentation

Information for working with the engine is available in header files, and in
various files in the doc/ folder. Information on the story file format is in
[doc/nstspecs.md](doc/nstspecs.md). You can also see the
[original project pitch](doc/pitch.txt) there.

Documentation is also available on the
[website](https://lethallisa.neocities.org/nightcrawler) in a potentially easier
to digest format.

[[_TOC_]]

## Engine Restrictions and Limits

- Data files are limited to 2GiB in size due to using 32-bit absolute addresses.
- Engine is built around 32/64-bit little-endian architectures using GCC. Other 
systems may not function.

## Platform

As for the actual platform of the engine. The release included by default will
be for Win32, tested against Wine on Linux & Mac, though the source code will be
in C and built against x86_64 Linux with GCC for testing, and should be capable
of compiling on any decently modern 64 or 32-bit OS.

You may be able to get the engine to compile on platforms besides these with 
GCC installed. Early versions were proven to work on Termux with some minor 
makefile changes.

## Tools Needed/Used

- GCC/Clang for Linux compilation.
- mingw32-gcc for Windows compilation.
- gperf used for tokenizer generation. This is only required if you desire to change the set of keywords recognized by Nightcrawler.
- fasm used to build story files.

### Using the Build System

Nightcrawlers makefile recognizes the following environment variables:
- `WINDOWS` : Builds a 32-bit Windows binary.
- `WIN64` : If this and `WINDOWS` are set, a 64-bit binary will be generated,
otherwise no effect.

The makefile recognizes the following targets:
- `make debug` to generate a debug build.
- `make clean` to clean up.
- `make all` (default) to build.

## Using the Engine

Nightcrawler binaries will look for a file called `"default.nst"` in the
current working directory. This will be loaded as the default story file,
and can be overridden with the `-f` or `--file` flag. Saves are saved to
a file in the current working directory called `"nightcrawler.sav"`.

### List of Keywords Recognized by the Engine

- `HELP` Shows engine help & then help included in a story file.
- `MOVE|GO NORTH|SOUTH|EAST|WEST|N|S|E|W` Moves to a different scene.
- `LOOK|EXAMINE [AROUND|NORTH|SOUTH|EAST|WEST|N|S|E|W]` Triggers look node for 
the scene.
- `GET|GRAB|TAKE` Adds the item in the scene to the player's inventory.
- `TALK` Triggers the dialogue tree for the scene.
- `USE` Interact with the scene, optionally using an item in the inventory, 
selected automatically.
- `ITEMS|INVENTORY|INV` Displays your inventory.
- `SAVE` Saves the game state to a `"nightcrawler.sav"` file.
- `LOAD` Loads the game state from a `"nightcrawler.sav"` file.
- `QUIT` Quits the game.

## License

The Nightcrawler engine is licensed under the BSD 3-Clause license (see 
[LICENSE](LICENSE). Story files compatible with Nightcrawler may be licensed
however the author of these files sees fit.

## To Do

* [ ] Initial scene's exposition string does not auto-print until death &
  restart.
* [ ] Swap order of game over string with individual death strings.
* [ ] Options to auto-set story flags on scene load.
