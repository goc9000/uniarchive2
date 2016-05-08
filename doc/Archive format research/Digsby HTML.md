Archive Format Research: Digsby Multi-Messenger (HTML format)
=============================================================


Filename Format
---------------

- Both 1:1 conversations and conferences start with a common part that identifies the protocol and the local account on that protocol:

  `Digsby Logs/` *digsby_account* `/` *protocol* `/` *local_account*
  
  where:
  
  - *digsby_account* is the local account name for the Digsby multi-messenger app itself (note: it does not include the `@digsby.org` part)
  - *protocol* is the local account protocol
    - Observed values: `digsby`, `gtalk`, `jabber`, `msn`, `yahoo`
  - *local_account* is the account name under *protocol*
    - For all protocols except `yahoo`, it includes the `@digsby.org`, `@gmail.com` etc part

- For 1:1 conversations, the filename continues with:

  *peer_account* `_` *peer_protocol* `/` *YYYY-mm-dd* `.html`
  
  where:
  
  - *peer_account* is the peer account name (under the peer protocol)
    - **Anomaly**: for Digsby, the peer account does NOT include the `@digsby.org` part (whereas the local account name does)
  - *peer_protocol*: also `digsby`, `gtalk`, etc. Generally the same as the local protocol, except sometimes you can get `gtalk` talking to `jabber` on the other end (because they are the same)
  - *YYYY-mm-dd* is the date in **local time** (similar to Yahoo Messenger)

- For conferences, the filename continues with:

  `Group Chats/` *YYYY-mm-dd* `T` *hh.mm.ss* ` - ` *peer_account* `-` *unix_timestamp* `.html`

  where:
  
  - `YYYY-mm-ddThh.mm.ss` is the conference start date in **local time**
  - *peer_account*: not clear who this represents. Initiator? First peer?
    - Note that the remote protocol is not specified
  - *unix_timestamp* is the conference start date in Unix timestamp format (thus UTC)
    - **Note**: thus, conference time offset can be obtained
