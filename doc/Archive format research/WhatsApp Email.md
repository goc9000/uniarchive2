Archive Format Research: WhatsApp (E-mail)
==========================================


Filename Format
---------------

- `WhatsApp Chat with ` *Peer Name* `.txt`
  
  where *Peer Name* is either:

  - the screen name of the peer
  - or the name of the group
  - or a phone number

- **Note**: No easy distinction between 1:1 and group conversations
  - Finding this out requires reading the file


Distribution of Conversations Across Files
------------------------------------------

- A file contains all conversations with that peer/group, ever (so it is more like a *thread*).


File Format
-----------

- Plain text format
- Consists of a series of events, with no markup, header or footer
- No separation between sessions, will need to separate them using time gap information
- Group conversations begin with a "XXX created group ZZZ" system event and this is the only way to recognize them


Event Format
------------

### Regular Messages ###

- Format:

  `dd/mm/yyyy, hh:mm - Peer Name:  Message text`
  
- Uncertain whether the timestamp format is locale-dependent
- **Problem**: The timestamp is expressed in *local time*, i.e. versus the timezone that was in place when the conversation occurred
- **Problem**: Newlines in the message text are unquoted
  - Need to do pattern recognition to tell messages apart
  - Quoted messages cannot be distinguished from actual messages (probably - it's inconvenient to copy-paste on mobile so there are no examples of this occurring)

- Attachments are represented using the following message body format:

  `filename.ext (file attached)`
  
  or
  
  `<Media omitted>`


### System Messages ###

- Format:

  `dd/mm/yyyy, hh:mm - System message text`

- The following system messages have been observed:

  - `Peer Name created group “Group Name”`
    - Note the smart quotes
    - This event appears at the start of group conversations
  
  - `Peer Name added you`
    - Group add event
    - No events have been observed when *others* are added

  - `Messages you send to this chat and calls are now secured with end-to-end encryption. Tap for more info.`
    - Notice that appears after E2E encryption was introduced
