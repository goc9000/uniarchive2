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
  - *YYYYMMDD* = seems to be the date the conversation began
    - In what timezone? UTC? Local?
  - *own_account_name* = YM account name for ourselves; probably included so as to ensure that the decrpytion key is available even if the file is separated from the archive

  Notes:
  
  - What if there are multiple convos in the same date with the same peer?
    - It seems they get appended to the same file

- Conference:

  `Conferences/` *peer_account_name* `/` *YYYYMMDD* `-` *own_account_name* `.dat`

  - Not clear which user *peer_account_name* refers to (initiator? first peer?). Also, sometimes it doesn't look like a permanent YM account name. Maybe best not to rely on it.


Other File Properties
---------------------

- Last modified date
  - Is this date of last written event, or date of file creation (the latter would occur if convo is written all at once)
    - Must check
  - Could be useful to determine timezone of other dates in convo
