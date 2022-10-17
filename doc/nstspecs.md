# NST Story File Format
The NST story format contains all information that the Nightcrawler engine can display.
This format is built around *nodes* which are each identified with a *magic number* string. Each node can be anywhere within a single file, with the sole exception to this being the story node, which must begin at the beginning of the file, and only one of which may exist.

On the general data format, all addresses within files are absolute, and all binary data is little-endian, and all strings are NUL terminated. Sometimes nodes will have fields for *required flags*, in most cases these flags can be set to display alternative text if the engine's progress flags pass an AND operation on them, or to change the way a node functions.

## List of Node Identifiers ("Magic Numbers")
All magic numbers are 4 bytes wide with a three-string
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
This type of node contains vital information about the story file overall, including story format version, global strings, the game's starting scene, and item names. It must be located at position 0 in the file, and only one node of this type may exist per story.
Binary layout to follow:

- byte (char) [4] : Magic "NST"
- uint32          : Format Version (must be zero in this version).
- uint32          : Address of the game's title screen string (zero if none).
- uint32          : Address of the game's prompt string (zero if none). If no prompt is specified a default prompt will be used.
- uint32          : Address of the game's help string (zero if none). The help string is appended to the engine's help message when the player runs `HELP`.
- uint16          : Count of items in the game (zero if none). No more than 16 items are allowed.
- uint32          : Address where the list of item names begins.
- uint32          : Address of the initial scene of the game. This value must be specified.
