Yahoo Messenger Archive Format Research
=======================================

----------------

WORK IN PROGRESS

----------------


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

  - Not clear which user *peer_account_name* refers to (initiator? first peer?). Also, sometimes it doesn't look like a permanent YM account name. Maybe best not to rely on it.


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
