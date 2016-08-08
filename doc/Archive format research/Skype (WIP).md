Archive Format Research: Skype
==============================


General Notes
-------------

- The most nightmarish, treacherous, asinine format of all those studied
- Despite being the only format with a proper database structure, this actually makes things much more difficult
  - An excellent example of nearly every possible failure of normalization
  - The same information is often present in multiple redundant forms, all of them potentially missing or contradictory
  - Example:
    - Calls are represented both as individual entries in the `Calls` table, and as pairs of type=30/39 messages
    - The two are in close, but not exact (!) correspondence
    - Said correspondence is not reflected by any explicit foreign key link, but needs to be inferred by matching using start timestamp and duration, which almost never correspond exactly, requiring guesswork on part of the algorithm
    - The peers of the call are represented in at least three, contradictory places:
      - The `identities` field in the message
      - The `<partlist>` XML structure encoded in the `body_xml` field of the message
      - The entries in the `CallMembers` table linked to the call record in the `Calls` table.
    - Etc.
- Partly because of this, the Skype conversation extractor deviates from the principle of representing the data exactly as it is and leaving any fixes for later. We apply whatever fixes and guesses seem necessary to coerce the data to a consistent representation in the `RawConversation` space and preserve as many details as possible.


Archive Directory Structure
---------------------------

- The exact contents differ slightly depending on version and platform
- The following files and directories have been found to exist in all tested Skype archives:

  ```
  <USERNAME>/bistats.db
  <USERNAME>/chatsync/
  <USERNAME>/config.lck
  <USERNAME>/config.xml
  <USERNAME>/dc.db
  <USERNAME>/httpfe/cookies.dat
  <USERNAME>/keyval.db
  <USERNAME>/main.db
  <USERNAME>/msn.db
  <USERNAME>/voicemail/
  shared.lck
  shared.xml
  shared_dynco/dc.db
  shared_httpfe/queue.db
  ```

  where `<USERNAME>` corresponds to the Skype account name of an user

- **The main file of interest is `<USERNAME>/main.db`**
- The others have been found to be empty, caches, or uninteresting
- The `<USERNAME>/chatsync/` directory seems to contain an index of temporary sync data, in binary format
  - We are assuming that in a quiescent archive, the `main.db` file is completely synced
  - Therefore, we will not parse the files in `chatsync/`
  - **Warning**: Might want to revisit this decision after more research


File Format
-----------

- SQLite database
- **Warning**: There might be an adjacent `main.db-journal` file.
- **Note**: In old versions there are sometimes adjacent `<tablename><N>.dbb` files present, where `N` is a power of two. These seem to contain backups (DBB = DataBase Backup?) of rows within that size of the given tables in `main.db`.


Database Contents
-----------------

stub
