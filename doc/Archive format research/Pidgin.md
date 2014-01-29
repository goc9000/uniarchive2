Pidgin Archive Format
=====================

Commonalities
-------------

- Also valid for older version of Pidgin called Gaim
- Filenames for regular conversations look like:

  `logs/PROTOCOL/YOUR_ACCOUNT/FRIEND_ACCOUNT/YYYY-MM-DD.HHMMSS+TIMEZONE{.html|.txt}`
  
  e.g.
  
  `logs/jabber/alice@gmail.com/bob@gmail.com/2007-12-02.233301+0200EET.txt`

  - Some old versions of Pidgin do not seem to attach timezone info
    - **Problem**: Does this mean it automatically converts to UTC, or does it just omit TZ
      info and we have to guess it?

- For conferences, the filename format is:

  `logs/PROTOCOL/YOUR_ACCOUNT/FRIEND_ACCOUNT-CONFERENCE_ID.chat/YYYY-MM-DD.HHMMSS+TIMEZONE{.html|.txt}`

  e.g.

  `logs/jabber/alice/bob-1238153419/2009-03-29.132814+0300EEST.txt`

  where FRIEND_ACCOUNT is one of the friends involved in the conference (probably the first to join)
  and CONFERENCE_ID is a seemingly opaque numeric identifier.

- Conversation files are highly self-contained, we for the most part we don't need any hints from the filename or
  path. One minor exception is the '.chat' termination for the containing folder, which indicates this conversation is
  a conference.
- Seems to always use UTF-8 encoding
- Not counting markup, for a regular conversation the header has the form:

  `Conversation with FRIEND_ACCOUNT at DATE on YOUR_ACCOUNT (PROTOCOL)`

  e.g.

  `Conversation with bob@gmail.com at Sun 23 Nov 2008 03:28:51 PM EET on alice@gmail.com (jabber)`

  - Date seems to be in a RFC2822-like format
  - When the filename is missing the timezone, so does the header
  - Seems easier to use the filename to get the conversation date (fixed format)
  - For conferences, the dash and numeric ID get appended to FRIEND_ACCOUNT in the header

- **Problem**: Replies use aliases, not accounts
- **Problem**: There is no explicit indication as to which of the speakers is you
- **Problem**: Except for offline messages, message timestamps do feature the date, not even
  when they cross into a new day.
- First message date may precede conversation date, if the conversation starts with offline
  messages
- **Problem**: Sometimes on jabber you get a weird friend id like <random_chars>@public.talk.google.com.
  It may not be possible to determine the real account if the friend's alias (or a "X is now known as Y"
  message) never appears.

Plaintext Variant
-----------------

- Sample:

      Conversation with bob123 at Sun 23 Nov 2008 03:28:51 PM EET on alice456 (yahoo)
      (03:28:57 PM) Alice Allison: Hi Bob
      (03:29:34 PM) Bob Bundy: Oh hi Alice :)
      (03:29:42 PM) Alice Allison: last night was great
      (03:30:56 PM) Alice Allison: we should hang out again sometime

- Offline messages look like:

      Conversation with bob123 at Mon 03 Dec 2007 09:28:39 PM EET on alice456 (yahoo)
      (12/03/2007 03:59:34 PM) Bob Bundy: Alice you there?
	  (12/03/2007 03:59:41 PM) Bob Bundy: helloooo
	  (12/03/2007 03:59:42 PM) Bob Bundy: echo ... echo
      (09:28:45 PM) Alice Allison: I am now

- **Problem**: There is no quoting in effect for multi-line messages:

      (10:24:00 PM) Alice Allison: Here is a haiku
      This is the second like
      And the Haiku has now come to an end
      (10:24:34 PM) Bob Bundy: nice

- This is especially problematic when they contain copy-pasted replies:

      (11:50:04 PM) Bob Bundy: This is what I was saying before the connection dropped:
      (11:40:28 PM) Bob Bundy: helloooo
      (11:40:32 PM) Bob Bundy: echo.. echo

- Some lines are system messages, distinguished by the fact that they don't begin with
  "Alias:", e.g.:

      (01:59:42 AM) Alice Allison has signed off.

  - The following types are known:
    - `ALIAS logged in.`
    - `ALIAS logged out.`
    - `ALIAS has signed on.`
    - `ALIAS has signed off.`
    - `ALIAS has gone away.`
    - `ALIAS is no longer away.`
    - `ALIAS has become idle.`
    - `ALIAS is no longer idle.`
    - `Offering to send FILENAME to ALIAS`
    - `ALIAS is offering to send file FILENAME`
    - `ALIAS is trying to send you a group of N files.`
    - `Starting transfer of FILENAME from ALIAS`
    - `ALIAS canceled the transfer of FILENAME`
    - `Transfer of file FILENAME complete`
    - `ALIAS1 is now known as ALIAS2.`
      - Note: The ALIAS may be equal to an account name
    - `ALIAS entered the room.`
    - `ALIAS left the room.`
    - `ALIAS has sent you a webcam invite, which is not yet supported.`
    - `ALIAS just sent you a Buzz!`
    - `Message could not be sent because a connection error occurred:`
    - `Unable to send message: The message is too large.`
    - `Message could not be sent because the user is offline:`

  - **Quirk**: Sometimes (on some versions?) system messages have an extra
    space at the end, so trimming is recommended.
  - **Quirk**: When you buzz someone, the system message looks like this:

      (20:00:38) : Buzz!!

- **Problem**: Even after date rollover correction, message timestamps can
  still show temporal discontinuity. Sometimes this occurs due to offline
  messages that are not explicitly marked as such; other times this occurs
  when quoting messages from the past, as, due to the lack of a quoting
  system, the pasted messages are indistinguishable from real ones. Other
  times there is no logical explanation at all. Unfortunately, there seems
  to be no solution that can correctly identify all these situations without
  the use of detailed user hints or hard A.I.
