Archive Format Research: Skype
==============================


General Notes
-------------

- The most nightmarish, treacherous, asinine format of all those studied
- Despite being the only format with a proper database structure, this actually makes things much more difficult
  - An excellent example of nearly every possible failure of normalization
  - The same information is often present in multiple redundant forms, all of them potentially missing or contradictory
  - Example:
    - Calls are represented both as individual entries in the Calls table, and as pairs of type=30/39 messages
    - The two are in close, but not exact (!) correspondence
    - Said correspondence is not reflected by any explicit foreign key link, but needs to be inferred by matching using start timestamp and duration, which almost never correspond exactly, requiring guesswork on part of the algorithm
    - The peers of the call are represented in at least three, contradictory places:
      - The `identities` field in the message
      - The `<partlist>` XML structure encoded in the `body_xml` field of the message
      - The entries in the `CallMembers` table linked to the call record in the `Calls` table.
    - Etc.
- Partly because of this, the Skype conversation extractor deviates from the principle of representing the data exactly as it is and leaving any fixes for later. We apply whatever fixes and guesses seem necessary to coerce the data to a consistent representation in the `RawConversation` space and preserve as many details as possible.


Filename Format
---------------

stub


Distribution of Conversations Across Files
------------------------------------------

stub


File Format
-----------

stub


Database Contents
-----------------

stub
