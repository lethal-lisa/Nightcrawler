# NST Story File Format
The NST story format contains all information that the Nightcrawler engine can 
display.
This format is built around *nodes* which are each identified with a *magic 
number* string. Each node can be anywhere within a single file, with the sole 
exception to this being the story node, which must begin at the beginning of 
the file, and only one of which may exist.

On the general data format, all addresses within files are absolute, and all 
binary data is little-endian, and all strings are NUL terminated. Sometimes 
nodes will have fields for *required flags*, in most cases these flags can be 
set to display alternative text if the engine's progress flags pass an AND 
operation on them, or to change the way a node functions.

All the information below is from the file [inc/story.h](../inc/story.h).
Consult that file for The most up to date information.

[[_TOC_]]

## List of Node Identifiers ("Magic Numbers")
All magic numbers are 4 bytes wide with a three char string, and a terminating
NUL char. Magic numbers are used by the engine to identify the type of node.
- `NST` For the "story" node.
- `NSC` For "scene" nodes.
- `MOV` For "move" nodes.
- `LOK` For "look" nodes.
- `TLK` For "talk" nodes.
- `DIA` For "dialogue" nodes.
- `DOL` For "dialogue option list" nodes.
- `OPT` For "dialogue option" nodes.
- `USE` For "use" nodes.

## NST Node Layout
This type of node contains vital information about the story file overall, 
including story format version, global strings, the game's starting scene, and 
item names. It must be located at position 0 in the file, and only one node of 
this type may exist per story.
Binary layout to follow:

- byte (char) [4] : Magic "NST".
- uint32          : Format Version (must be zero in this version).
- uint32          : Address of the game's title screen string (zero if none).
- uint32          : Address of the game's prompt string (zero if none). If no 
prompt is specified a default prompt will be used.
- uint32          : Address of the game's help string (zero if none). The help 
string is appended to the engine's help message when the player runs `HELP`.
- uint16          : Count of items in the game (zero if none). No more than 16 
items are allowed.
- uint32          : Address where the list of item names begins.
- uint32          : Address of the initial scene of the game. This value must 
be specified.

## NSC Node Layout
These are Scene nodes, which mostly wrap the MOV, LOK, TLK, and USE nodes. They
also contain a GET mask, which is OR'd with the game's item flags.

- byte (char) [4] : Magic "NSC".
- uint32          : Address of the scene's MOV node (must not be zero).
- uint32          : Address of the scene's LOK node (must not be zero).
- uint16          : Mask of items to OR with the game's item flags when `GET` is
run.
- uint32          : Address of the scene's TLK node (optional, zero if none).
- uint32          : Address of the scene's USE node (optional, zero if none).

## MOV Node Layout
These are nodes that contain addresses to other NSC nodes, and optionally can
branch to alternative scenes based on the game's state flags being met.
Addresses can be set to zero if it is desired for the player to not be able to
move in a direction.

- byte (char) [4] : Magic "MOV".
- uint16          : Required story flags to use alt addresses.
- uint16          : Required item flags to use alt addresses.
- uint32          : Address of NSC node to use for North.
- uint32          : Address of NSC node to use for South.
- uint32          : Address of NSC node to use for East.
- uint32          : Address of NSC node to use for West.
- uint32          : Alternate address of NSC node to use for North.
- uint32          : Alternate address of NSC node to use for South.
- uint32          : Alternate address of NSC node to use for East.
- uint32          : Alternate address of NSC node to use for West.

## LOK Node Layout
These contain addresses to strings that respond to the LOOK command, and can
optionally set story flags or have alternate messages based on story and item
flags. Story flags are set before being compared for displaying alternate text.
Addresses must not be zero.

- byte (char) [4] : Magic "LOK".
- uint16          : Required story flags to use alt addresses.
- uint16          : Required item flags to use alt addresses.
- uint32          : Address of string to use for "Around".
- uint32          : Address of string to use for North.
- uint32          : Address of string to use for South.
- uint32          : Address of string to use for East.
- uint32          : Address of string to use for West.
- uint32          : Address of alternate string to use for "Around".
- uint32          : Address of alternate string to use for North.
- uint32          : Address of alternate string to use for South.
- uint32          : Address of alternate string to use for East.
- uint32          : Address of alternate string to use for West.
