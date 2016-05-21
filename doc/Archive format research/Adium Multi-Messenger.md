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
