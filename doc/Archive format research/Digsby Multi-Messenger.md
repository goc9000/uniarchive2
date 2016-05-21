Archive Format Research: Digsby Multi-Messenger
===============================================


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


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file (indicating events are appended in real time as opposed to the whole file being written at once; file structure also confirms it)
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- A file contains all conversations with that peer for a day
- When the date rolls over at midnight, *local time*, events start being written to a new file
  - Therefore, conversations within a file need to be split, and conversations straddling files need to be reassembled
- This is the same system used by Yahoo Messenger


File Format
-----------

- Malformed XHTML file
- General form:

  ```xhtml
  <?xml version="1.0" encoding="UTF-8"?>
      <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
      "http://www.w3.org/TR/html4/strict.dtd">
  <HTML>
     <HEAD>
        <meta http-equiv="Content-Type" content="text/html;charset=UTF-8" />
        <TITLE>IM Logs with remote_peer on YYYY-MM-DD</TITLE>
     <style>
       .buddy { font-weight: bold; }
       .buddy:after { content: ":" }
  
       .time {
          color: #a0a0a0;
          font-family: monaco, courier new, monospace;
          font-size: 75%;
       }
       .time:hover { color: black; }
  
       .outgoing { background-color: #efefef; }
       .incoming { background-color: #ffffff; }
     </style>
     <script type="text/javascript">
  //<![CDATA[
      function convert_time(datetime){
          var dt = datetime.split(" ");
          var date = dt[0].split("-");
          var time = dt[1].split(":");
          var t = new Date;
          t.setUTCFullYear(date[0],date[1]-1,date[2]);
          t.setUTCHours(time[0],time[1],time[2]);
          return t.toLocaleTimeString();
      }
  
      function utc_to_local(){
          var node;
          for (var i=0; i<document.body.childNodes.length; i++){
              node = document.body.childNodes[i];
              if(node.nodeType == 1 && node.className.match("message")){
                  var showtime = convert_time(node.getAttribute('timestamp'));
                  var newspan = '<span class="time">(' + showtime + ') </span>';
                  var msgnode = node;
                  msgnode.innerHTML = newspan + msgnode.innerHTML;
              }
          }
      }
  //]]>
     </script>
     </HEAD>
     <BODY onload="utc_to_local()">
  <div class="outgoing message" ...>...</div>
  <div class="incoming message" ...>...</div>
  <div class="incoming message" ...>...</div>
  ...
  <div class="incoming message" ...>...</div>
  ```

- For conferences, the `<title>` changes to " `Chat in ` *peer_name* `-` *unix_timestamp* ` on ` *YYYY-mm-dd* "
- **Anomaly**: **There is never a closing `</BODY></HTML>` sequence** (thus the file is always malformed)
  - Explanation: note how the event `<div>`s are unaligned. Very likely, a fixed HTML header (ending at `<BODY onload="utc_to_local()">`) is written when the archive is created, and, subsequently, event `<div>`s are simply appended to the file without re-reading the structure.


Event Format
------------

- **Major problem**: **Due to a bug, events occasionally get corrupted!**
  - The specific way in which this happens: a random number of characters are removed from the beginning the HTML corresponding to the `<div>` for the event
    - Educated speculation: probably a circular buffer where wraparound is not handled correctly in some cases
  - Occurs rarely (about 1 in 9200 events)
  - May occur in consecutive messages, but this is very rare (observed in only 1 conversation)

- Events always start on new line (this helps with recovery from the above bug)
- Only message events observed
- General format example (note: has been reformatted, in reality all tags are usually on a single line):

  ```html
  <div class="incoming message" auto="False" timestamp="2010-02-05 23:53:08">
    <span class="buddy">buddy_account</span>
    <span class="msgcontent">hello world</span>
  </div>
  ```

- `class`: Only two observed values are `incoming message` and `outgoing message`
- `auto`: Unclear what this stands for. Perhaps autoreply?
- `timestamp`: Definitely UTC
  - Evidence: the embedded JavaScript code parses it as UTC and converts it to local time for display
- `buddy`: Always the sender account name
- `msgcontent`: The content, in HTML format (may contain tags)
  - **Note**: Sometimes this contains newlines, but they are always doubled by `<br>` tags, so they should be ignored
  - **Bug**: Sometimes, tag-like constructs entered by the user (e.g. "`<checks agenda>`") are **not escaped** and, what's worse, a spurious end tag is added for them at the end of the message, e.g. `</checks>` in this case.
  - Observed tags:
    - `<b>`, `<i>`, `<u>`, `<a>`, `<br>`: These work as normal
    - `<span>`: Only used to specify CSS (never any attribute other than `style`)
      - **Anomaly**: Digsby protocol messages contain a `<span>` with no attributes
    - `<font>`: Any combination of the attributes `face`, `color` and `style` may appear. The `style` itself is used to specify the face, color or size.
      - **Anomaly**: A strange alternate form has been observed, looking like this:
      
        `<font bdyencid-0 bdpk-6491815c1d2d43b0152e9648a6773d842e63d59c3f4862c21b21194c70aff177 bdch-44768887836493963772329000228050>`

        No idea what the pseudo-attributes represent or whether they can be decoded.
    - `<fade>`: Seems to represent the Yahoo `<FADE>` tag, but the colors are never transmitted (!)
  - Tags are generally closed correctly (except for `<br>`, which is written as-is, i.e. not self-closed)

- **Note**: On Yahoo at least, pings from the other side are interpreted as a text message containing the literal text `<ding>`.


General Notes
-------------

- A very problematic format due to bugs that cause corruption and ambiguity
- Seems to draw inspiration from the Yahoo archive format (same distribution algorithm, senders always specified as accounts)
