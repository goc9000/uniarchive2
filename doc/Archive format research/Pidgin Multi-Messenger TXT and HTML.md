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
  
  *peer_account_name* `-` *unix_timestamp*
  `private-chat-9631bd3a-7352-4427-8040-d5ebfee942cd@groupchat.google.com`


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file (indicating events are appended in real time as opposed to the whole file being written at once; file structure also confirms it)
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- One file = one conversation
