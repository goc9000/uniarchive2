Archive Format Research: Adium Multi-Messenger
==============================================


Filename Format
---------------

`Logs/` *protocol* `.` *local_account* `/` *peer_account* `/` *peer_account* ` (` *yyyy-MM-ddThh.mm.ss+offset* `).chatlog/` *(same as folder name)* `.xml`

where:
  
- *protocol* is the local account protocol
  - Observed values: `Yahoo!`, `Jabber`
- *yyyy-MM-ddThh.mm.ss+offset* is the conversation start date. It includes the timezone offset in `+hhmm` format.
- The last folder name is repeated in the `.xml` archive basename
  - There only seems to ever be a single archive in the last folder


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file (indicating events are appended in real time as opposed to the whole file being written at once; file structure also confirms it)
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- One file = one conversation


File Format
-----------

- Well-formed XML file
- General form:

  ```xml
  <?xml version="1.0" encoding="UTF-8" ?>
  <chat
    xmlns="http://purl.org/net/ulf/ns/0.4-02" account="(local_account)"
    service="(protocol)" adiumversion="1.5.6" buildid="90a171a6b0fc">
    <event ...></event>
    <message ...></message>
    ...
    <event ...></event>
  </chat>
  ```

Event Format
------------

- Three event tag types: `<message>`, `<status>` and `<event>`
- Common attributes:
  - `time` : always present, event time in ISO-8601 format (`yyyy-MM-ddThh:mm:ss+hh:mm`)`
    - Includes timezone offset, so both local time and absolute time are available
  - `sender` : nearly always present, the event sender
    - Always an account name
  - `alias` : sometimes present
    - Provides the screen name for `sender`


Message Event Format (`<message>` tag)
--------------------------------------

- General format example:

  ```xml
  <message sender="peer@service.com" time="2013-06-27T22:20:03+02:00" alias="Peer Name">
    <div>
      <span style="font-family: Helvetica; font-size: 12pt;">ok</span>
    </div>
  </message>
  ```

- Messages may be blank
  - If they are not, content is always contained in an attribute-less `<div>`

- Observed text formatting tags:
  - `<span style="(css code)">` : only every used to specify CSS style
  - `<a>` : `href` sometimes not present
  - `<br/>`
  - Tags are always well closed
- Newlines may be present, but they are always redundant with `<br/>` tags


System Meta-Event Format (`<event>` tag)
----------------------------------------

- Only two types of events observed:

  `<event type="windowOpened" sender="local_account" time="2013-07-23T14:41:56+02:00"/>`
     
  `<event type="windowClosed" sender="local_account" time="2013-07-23T14:42:08+02:00"/>`

- Self-explanatory


System Event Format (`<status>` tag)
------------------------------------

- General format example:

  ```xml
  <status alias="Sender Name" sender="sender_account" type="away" time="2013-05-31T23:24:49+03:00">
    <div>
      <span style="font-family: Helvetica; font-size: 12pt;">I'm not here right now</span>
    </div>
  </status>
  ```

- Primary meaning is determined by `type` attribute
- Observed values:
  - `online`, `available`, `away`, `idle`, `offline` : Status changes
    - The tag content optionally contains the status text
    - Difference between `online`/`available` is unclear
      - `online` seems to be often followed immediately by `available`
      - also `away` followed by `idle`
  
  - `connected` and `disconnected` : Seems to mark only your own connection/disconnection
    - Tag content is always `You have connected` or `You have disconnected` respectively (plus `<div>` or `<span>` tags)

  - `Notification` : Seems to be used only for pings (note the uppercased first letter)
    - Observed tag content: `Screen Name wants your attention!`
  
  - `purple` and `libpurpleMessage` : Used for other system messages
    - Often has no specified `sender`
      - Subject can be deduced from screen name present in tag content
    - The tag content further specifies what the message means
    - Observed content:
      - `account_name is now known as Screen Name.`
      - `Screen Name is offering to send file filename.ext`
      - `Starting transfer of filename.ext from Screen Name`
      - `Transfer of file filename.ext complete`
        - The filename may be contained in an `<a>` tag
      - `Screen Name cancelled the transfer of filename.ext`
