Archive Format Research: Pidgin Multi-Messenger (TXT and HTML formats)
======================================================================


Preliminary Notes
-----------------

- Pidgin is a `libpurple`-based multi-messenger (like Adium)
- It was also called GAIM in older versions
- Saves conversations in text format pre-approx. Aug 2009 and in HTML format thereafter
- Very problematic format:
  - Multi-line messages are hard to tell apart in the text format, esp. when they are quotes of previous messages
  - Timestamps are incomplete (the date part is usually implicit)


Filename Format
---------------

- Format for 1:1 conversations:

  *protocol* `/` *local_account* `/` *peer_account* `/` *YYYY-MM-DD.hhmmss* *offset* *timezone* `.html` (or `.txt`)

- Format for conferences:

  *protocol* `/` *local_account* `/` *conference_id* `.chat` `/` *YYYY-MM-DD.hhmmss* *offset* *timezone* `.html` (or `.txt`)

- Observed values for *protocol*: `jabber`, `msn`, `yahoo`
- Example *offset*: `+0300`
- Example *timezone*: `EEST` (it's the abbreviation for the timezone that was in effect at the given date)
- Offset is missing pre-14 Nov 2006 (in this case, the date represents the local date)
- Extension is `.txt` if pre-Aug 2009, `.html` otherwise
- *conference_id* has no stable format. Some observed:
  - *peer_account_name* `-` *unix_timestamp*
  - `private-chat-9631bd3a-7352-4427-8040-d5ebfee942cd@groupchat.google.com`


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file (indicating events are appended in real time as opposed to the whole file being written at once; file structure also confirms it)
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- One file = one conversation


File Format (HTML variant)
--------------------------

- We will start with the HTML variant as it is easier to handle
- General form (reformatted for clarity):

  ```html
  <html>
    <head>
      <meta http-equiv="content-type" content="text/html; charset=UTF-8">
      <title>Conversation with peer_account at Tue 12 Oct 2010 09:18:15 PM EEST on local_account (protocol)</title>
    </head>
    <body>
      <h3>Conversation with peer_account at Tue 12 Oct 2010 09:18:15 PM EEST on local_account (protocol)</h3>
      <font color="#A82F2F"><font size="2">(09:18:14 PM)</font> <b>Peer Name:</b></font> bla bla<br/>
      <font color="#16569E"><font size="2">(09:20:13 PM)</font> <b>Local Name:</b></font> ba bla<br/>
      ...
      <font size="2">(10:01:25 PM)</font><b> Local Name left the room.</b><br/>
      ...
    </body>
  </html>
  ```
  
- In practice, the header up to `</h3>` is on a single line
- The `</body></html>` is on a single line and sometimes missing
- Two kinds of events:
  - Message events
  - System message events (sender is absent, cover all kinds of other events)


Message Format (HTML variant)
-----------------------------

- General form:

  `<font color="#A82F2F"><font size="2">(09:18:14 PM)</font> <b>Sender Name:</b></font> bla bla<br/>`
  
- Always ends with `<br/>` and newline
- The color indicates whether the screen name is the identity (`#16569E`) or a peer (`#A82F2F`)
  - In a conference, the same color is used for all peers
- **Problem**: Timestamp usually does not include date
  - Seems to be included in offline messages
  - Needs more research
- **Problem**: Timestamp format is unstable
  - Observed formats:
    - `h:mm:ss AM/PM`
    - `hh:mm:ss`
    - `YYYY.MM.DD hh:mm:ss`
    - `d/m/yyyy hh:mm:ss`
    - etc.
- The sender name is a screen name
- The content may include:
  - `<br/>` or newlines, but not both
  - `<a>`
  - `<em>`
  - `<span>` (used only for `style`)
- Some Jabber messages are wrapped in a sequence like:
  
  ```html
  <html xmlns='http://jabber.org/protocol/xhtml-im'>
    <body xmlns='http://www.w3.org/1999/xhtml'>
      <p>...</p>
    </body>
  </html>
  ```

- Occasional **problems** with the HTML encoding:
  - Attribute values are enclosed in non-standard single quotes instead of double-quotes
  - Escaping is sometimes broken leading to difficult to parse values, as in this example:
  
    `<span style='font-family: 'Times New Roman';'>`

  - Sometimes values are double-encoded:
  
    `<span style='font-family: &amp;apos;Helvetica&amp;apos;;font-size: 12px;'>`


System Message Format (HTML variant)
------------------------------------

- General form:

  `<font size="2">(10:01:25 PM)</font><b> Local Name left the room.</b><br/>`
  
- Sometimes there is a color tag, always `#FF0000`:

  `<font color="#FF0000"><font size="2">(10:01:25 PM)</font><b> File transfer cancelled</b></font><br/>`

- Except for the timestamp, the semantics are completely defined by the content
  - See the *Libpurple System Messages* document


File Format (TXT variant)
--------------------------

- Can be seen as the HTML version with all tags stripped out
- More difficult and ambiguous to parse
- General form:

  ```
  Conversation with peer_account at Tue 12 Oct 2010 09:18:15 PM EEST on local_account (protocol)
  (09:18:14 PM) Peer Name: bla bla
  (09:20:13 PM) Local Name: ba bla
  ...
  (09:44:17 PM) Local Name: this is a
  multi line
  message
  ...
  (10:01:25 PM) Local Name left the room.
  ...
  ```

- **Problem**: Newlines in multi-line messages are not quoted, and there is no markup to delimit different messages
  - Quoted messages are indistinguishable from normal messages
