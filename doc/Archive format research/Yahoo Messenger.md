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
