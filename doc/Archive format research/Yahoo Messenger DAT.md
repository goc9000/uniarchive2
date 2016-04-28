Archive Format Research: Yahoo Messenger (.DAT) 
===============================================


Filename Format
---------------

- 1:1 conversations:

  `Messages/` *peer_account_name* `/` *YYYYMMDD* `-` *own_account_name* `.dat`

  where:
  
  - *remote_account_name* = YM account name for peer (the person we are talking to)
  - *YYYYMMDD* = the date the conversation began, in *local time* (see [Distribution of Conversations Across Files] (#distribution-of-conversations-across-files) section below)
  - *own_account_name* = YM account name for ourselves; probably included so as to ensure that the decrpytion key is available even if the file is separated from the archive

- Conference:

  `Conferences/` *peer_account_name* `/` *YYYYMMDD* `-` *own_account_name* `.dat`

  - Not clear which user *peer_account_name* refers to (initiator? first peer?).


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file (indicating events are appended in real time as opposed to the whole file being written at once)
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- A file contains all conversations with that peer for a day
- When the date rolls over at midnight, *local time*, events start being written to a new file
  - Therefore, conversations within a file need to be split, and conversations straddling files need to be reassembled


File Format
-----------

- Pure binary format
- No header
- Consists solely of a series of *events*
- Event format:

  | Size                   | Base type                              | Significance    |
  |------------------------|----------------------------------------|-----------------|
  | 4 bytes                | integer (least significant byte first) | Timestamp       |
  | 4 bytes                | integer                                | Event type      |
  | 4 bytes                | integer                                | Direction       |
  | 4 bytes                | integer                                | Message length  |
  | *message length* bytes | UTF-8 character data                   | Message content |
  | 4 bytes                | integer                                | Extra length    |
  | *extra length* bytes   | character data                         | Extra content   |

- About the fields:
  - **Timestamp**
    - Is definitely a Unix timestamp
      - Evidence: I have a message where I explicitly state the local time. Translating to UTC given my timezone at the time, we get the exact same timestamp as in the field.
    - What is the significance of the timestamp? When event sent? Received?
      - It seems to be when the message was sent, given that offline messages appear with clearly separate times, not all bunched together
  - **Event type**: See next section for the different types of events.
  - **Direction**: This can be:
    - 0: outgoing
    - 1: incoming
    - 6: offline message
  - **Message content**
    - "Encrypted" (more like obfuscated) using local account name as a key
    - Definitely UTF-8
    - Mix of plain text and different markup styles
    - See dedicated section for details
  - **Extra**
    - Only seen this used to specify the sender account name in a conference
    - Assumed to be text

### Anomalies Observed ###

- Timestamps may not be strictly in order
- Needs more investigation; on first glance, temporal disruptions are minor and atributable to clock skew


Event Types
-----------

Note: the protocol likely supports more events, but Yahoo only logs a subset of these.

### 0: Start Conversation ###

- Appears at the beginning of a conversation (including conferences)
- `message` and `extra` fields seem to be always empty
- `direction` may be either incoming or outgoing depending on who initiated the conversation

### 6: Conversation Message ###

- Appears **only** in 1:1 conversations
- `extra` field is always empty
- Sender is implicit (local account for outgoing, remote account for incoming)

### 29: Conference Message ###

- Appears **only** in conferences
- If the direction is incoming, `extra` field contains the sender's Yahoo account ID
- If the direction is outgoing, the `extra` field does **not** contain the local account ID, but instead what seems to be the ID of the last remote peer that spoke; thus, it can be interpreted as the receiver. 

### 25: Join Conference ###

- Appears only in conferences and the direction is always incoming
- The `extra` field contains the ID of the peer that joined

### 26: Decline Conference ###

- Appears only in conferences and the direction is always incoming
- The `extra` field contains the ID of the peer that declined
- The `message` field may not be empty, instead containing the reason the user gave for declining

### 27: Leave Conference ###

- Appears only in conferences and the direction is always incoming
- The `extra` field contains the ID of the peer that left


Message Content Format
----------------------

### Encryption / Obfuscation ###

- This field (and only this field) is obfuscated by XOR-ing with a virtual string consisting of the *local account name* repeated endlessly
- In short:

  deobfuscated_content[i] = obfuscated_content[i] **xor** local_account_name[i **mod** len(local_account_name)]

### Text Data ###

- Encoding is UTF-8
- Consists of plain text with an ecletic set of markup tags thrown in
- Special characters appear as-is, there are no HTML entities
- Text may contain newlines, carriage returns and tabs

### Markup ###

- Three kinds of markup observed:
  - Pseudo-ANSI sequences
  - The HTML `<font>` tag
  - HTML-like `<FADE>` and `<ALT>` tags for Yahoo-specific coloring

#### Pseudo-ANSI sequences ####

- Similar to ANSI sequences described in: https://en.wikipedia.org/wiki/ANSI_escape_code
  - However, they violate and extend the syntax, plus some meanings are different
  - Only the `m` (Select Graphic Rendition) sequence seems to be used

- Form: `ESC` `[` *format* `m`
  - Where `ESC` is the control character with codepoint 27

- Observed sequences:
  - `ESC` `[` `0` `m` : ANSI defines it as "reset formatting". Presumably end the effects of all ANSI sequences before it.
  - `ESC` `[` `1` `m` : Turns on **bold** formatting
  - `ESC` `[` `x1` `m` : Turns off **bold** formatting
    - This and other `x` sequences are Yahoo extensions to ANSI
  - `ESC` `[` `2` `m` : ANSI defines this as turning on "faint" formatting. Yahoo seems to use this for **italics**.
  - `ESC` `[` `x2` `m` : Turns off **italics**
  - `ESC` `[` `4` `m` : Turns on **underline** formatting (`x4` turns it off, etc.)
  - `ESC` `[` `l` `m` : Turns on **hyperlink** formatting (`xl` turns it off). Another Yahoo extension.
  - `ESC` `[` `3` *color* `m` : Where *color* = 0 to 8. Applies the specified color to the text, according to the ANSI colors table:

    | Index | Color      |
    |-------|------------|
    | 0     | Black      |
    | 1     | Red        |
    | 2     | Green      |
    | 3     | Yellow     |
    | 4     | Blue       |
    | 5     | Magenta    |
    | 6     | Cyan       |
    | 7     | White      |
    | 8     | "Extended" |
    
    - Not clear what "extended" actually means for Yahoo. In the ANSI standard, extra parameters follow it describing the color in terms of RGB or a palette index, but this does not occur in the data.
    - Extended seems to occur more frequently when messages are quoted, around the acccount name. Maybe it means "default color"? (i.e. like ANSI SGR code `39`)
  
  - `ESC` `[` `#rrggbb` `m` : Applies the specified text color, as defined by its CSS-like hex representation. Another Yahoo extension.

#### `<font>` tags ####

- The only proper HTML tag observed
- Has the attributes `face` and/or `size`
  - `face` can actually be a comma-separated list of typefaces to try (as allowed by the HTML standard)
  - `size` can include units, e.g. `12pt`
- May or may not be closed by the end of the message

#### `<FADE>` and `<ALT>` tags ####

- Look somewhat like HTML, but violate the syntax
- The `<ALT>` tag looks like:

  `<ALT #123456,#7890ab>`
  
  i.e. two CSS colors are specified directly in the tag body (no attribute name).
  
  - The meaning is that the text color **alt**ernates between the two every letter
  - There are always exactly 2 colors specified

- The `<FADE>` tag looks like:

  `<FADE #123456,#7890ab,#cdef01>`
  
  i.e. at least one CSS color is specified
  
  - Sets up a gradient between the specified text colors
  - Not clear exactly how this works for just one color (fades between default color and this color?)
  - Gradient geometry is unclear (at which point in the text do we switch to the second, third etc. color?)
