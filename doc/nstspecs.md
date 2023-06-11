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
Consult that file for the most up to date information.

[[_TOC_]]

## List of Node Identifiers ("Magic Numbers")
All magic numbers are 4 bytes wide with a three char string, and a terminating
NUL char. Magic numbers are used by the engine to identify the type of node.
- `NST` : For the "story" node.
- `NSC` : For "scene" nodes.
- `MOV` : For "move" nodes.
- `LOK` : For "look" nodes.
- `TLK` : For "talk" nodes.
- `DIA` : For "dialogue" nodes.
- `DOL` : For "dialogue option list" nodes.
- `OPT` : For "dialogue option" nodes.
- `USE` : For "use" nodes.
- `WIN` : For "win" nodes.
- `DTH` : For "death" nodes.

## NST Node Layout
This type of node contains vital information about the story file overall, 
including story format version, global strings, the game's starting scene, and 
item names. It must be located at position 0 in the file, and only one node of 
this type may exist per story.
Binary layout to follow:

- byte (char) [4] : Magic `"NST"`.
- uint32          : Format Version (must be one in this version).
- uint32          : Address of the game's title screen string (zero if none).
- uint32          : Address of the game's credits screen (zero if none).
  Credits are displayed after the game is won.
- uint32          : Address of the game's prompt string (zero if none). If no
  prompt is specified a default prompt will be used.
- uint32          : Address of the game's help string (zero if none). The help
  string is appended to the engine's help message when the player runs `HELP`.
- uint32          : Address of the game's "launch" string. Displayed when the
  engine starts.
- uint32          : Address of the game's "death" string. Displayed when a game
  over occurs.
- uint16          : Count of items in the game (zero if none). No more than 16
  items are allowed.
- uint32          : Address where the list of item names begins.
- uint32          : Address of the initial scene of the game. This value must
  be specified.

## NSC Node Layout
These are Scene nodes, which mostly wrap the `MOV`, `LOK`, `TLK`, and `USE`
nodes. They also contain a `GET` mask, which is OR'd with the game's item
flags. The `USE` node address may also point to a `WIN` or `DTH` node which will
run appropriate actions.

- byte (char) [4] : Magic `"NSC"`.
- uint16          : Flags used to determine autocommands.
- uint32          : Address of the scene's exposition string. This will be
  displayed when the scene is loaded.
- uint32          : Address of the scene's `MOV` node (must not be zero).
- uint32          : Address of the scene's `LOK` node (must not be zero).
- uint16          : Mask of items to OR with the game's item flags when `GET`
  is run.
- uint32          : Address of the scene's `TLK` node (optional, zero if none).
- uint32          : Address of the scene's `USE` node (optional, zero if none).

### Autocommands
Setting certain bits in the flags field will automatically trigger commands
when the scene is loaded.
0x1: `TALK`
0x2: `LOOK`
0x4: `GET`
0x8: `USE`

## MOV Node Layout
These are nodes that contain addresses to other `NSC` nodes, and optionally can
branch to alternative scenes based on the game's state flags being met.
Addresses can be set to zero if it is desired for the player to not be able to
move in a direction. Additionally instead of an `NSC` node, a `WIN` or `DTH`
node's address may be used instead, and will perform the respective actions.

- byte (char) [4] : Magic `"MOV"`.
- uint32          : Required story flags to use alt addresses.
- uint32          : Required item flags to use alt addresses.
- uint32          : Address of NSC node to use for North.
- uint32          : Address of NSC node to use for South.
- uint32          : Address of NSC node to use for East.
- uint32          : Address of NSC node to use for West.
- uint32          : Alternate address of NSC node to use for North.
- uint32          : Alternate address of NSC node to use for South.
- uint32          : Alternate address of NSC node to use for East.
- uint32          : Alternate address of NSC node to use for West.

## LOK Node Layout
These contain addresses to strings that respond to the `LOOK` command, and can
optionally set story flags or have alternate messages based on story and item
flags. Story flags are set before being compared for displaying alternate text.
Addresses must not be zero.

- byte (char) [4] : Magic `"LOK"`.
- uint32          : Story flags to set.
- uint32          : Required story flags to use alt addresses.
- uint32          : Required item flags to use alt addresses.
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

## TLK Node Layout
This type of node defines the beginning of a dialogue tree, including required
flags and the initial `DIA` node. If the required flags are not met a `"There is
no one here to talk to."` message is displayed.

- byte (char) [4] : Magic `"TLK"`.
- uint32          : Required story flags to initiate dialogue.
- uint32          : Required item flags to initiate dialogue.
- uint32          : Address of initial `DIA` node.

## DIA Node Layout
`DIA` nodes holds information about a dialogue phase; including textual and flag
data. The node can set flags, has an alternate set of flags and text that can
be set if required flags are met, and can point to an `DOL` node optionally.
If a `DOL` node is specified the game will prompt the player for their choice
from a list of options, if not, the dialogue mode will end and the game will
return to its standard input processing mode.

- byte (char) [4] : Magic `"DIA"`.
- uint32          : Story flags to set.
- uint32          : Item flags to set.
- uint32          : Alternate story flags to set.
- uint32          : Alternate item flags to set.
- uint32          : Required story flags for alternate fields.
- uint32          : Required item flags for alternate fields.
- uint32          : Address of string to display.
- uint32          : Address of alternate string to display.
- uint32          : Address of options list (zero if none).

## DOL Node Layout
This type of node works slightly differently from the others. It begins with a
header structured like all the others, but has a list of uint32s that hold
addresses of `OPT` nodes.

- byte (char) [4] : Magic `"DOL"`.
- uint16          : Count of subsequent OPT node addresses.

## OPT Node Layout
OPT nodes define a dialogue option the player can choose. They can set flags,
and have required flags to display. All the options will be displayed in a list
for the player to select. but when the required flags for a node are not met,
the node will be omitted in the list. Additionally instead of pointing to a
`DIA` node `OPT` nodes can point to either `WIN` or `DTH` to perform those
respective actions.

- byte (char) [4] : Magic `"OPT"`.
- uint32          : Story flags to set.
- uint32          : Item flags to set.
- uint32          : Required story flags to display the option.
- uint32          : Required item flags to display the option.
- uint32          : Address of the string for the option.
- uint32          : Address of the `DIA` node to branch to (optional).
- uint32          : Address of the `NSC` node to move to (optional).

## USE Node Layout
Defines the action of an `USE` command, including flags that are set,
prerequisites, and the text to display. If the prerequisite flags are not met
no flags will be set and the user will get a `"No effect."` message.

- byte (char) [4] : Magic `"USE"`.
- uint32          : Story flags to set.
- uint32          : Item flags to set.
- uint32          : Required story flags to run the `USE` command.
- uint32          : Required item flags to run the `USE` command.
- uint32          : Address of the string to print on successful `USE` command.
- uint32          : Address of the `NSC` node to move to (optional).

## WIN and DTH Node Layout
Win and Death nodes are structured exactly the same, but do different things
when called. Death nodes will print their string and reset the game. Win nodes
will print their string and exit the game, and print an `"Press Enter to exit
the game..."` message. These nodes can be called by `MOVE`, `USE`, or `TALK`
commands. See the `MOV`, `NSC`, and `OPT` node documentation on how that works.

- byte (char) [4] : Magic `"WIN"`, or `"DTH"`.
- uint32          : Address of string to print out.
