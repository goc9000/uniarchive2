Archive Format Research: Facebook (DYI format)
==============================================


How to Extract
--------------

This conversation archive can be obtained as part of your profile data when you use the Download Your Information (DYI) feature in Facebook.


Filename Format
---------------

- The filename takes the form:

  `html/messages.htm`

  within the downloaded profile archive.


Other File Properties
---------------------

- Last modified date
  - Corresponds to the date when the archive was extracted
  - Note that this might be a server date


Distribution of Conversations Across Files
------------------------------------------

- The `messages.htm` file contains all conversations with everyone, ever


File Format
-----------

- Reasonably well-formed HTML file
- General form:

  ```html
  <html>
    <head>
      <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
      <title>Your Name - Messages</title>
      <link rel="stylesheet" href="../html/style.css" type="text/css" />
    </head>
    <body>
      <div class="nav">
        <img src="../photos/profile.jpg" />
        <ul>
          <li><a href="../index.htm">Profile</a></li>
          <li><a href="../html/contact_info.htm">Contact Info</a></li>
          <li><a href="../html/wall.htm">Wall</a></li>
          <li><a href="../html/photos.htm">Photos</a></li>
          <li><a href="../html/synced_photos.htm">Synced Photos</a></li>
          <li><a href="../html/videos.htm">Videos</a></li>
          <li><a href="../html/friends.htm">Friends</a></li>
          <li class="selected">Messages</li>
          <li><a href="../html/pokes.htm">Pokes</a></li>
          <li><a href="../html/events.htm">Events</a></li>
          <li><a href="../html/settings.htm">Settings</a></li>
          <li><a href="../html/security.htm">Security</a></li>
          <li><a href="../html/ads.htm">Ads</a></li>
          <li><a href="../html/mobile_devices.htm">Mobile Devices</a></li>
          <li><a href="../html/places.htm">Places</a></li>
          <li><a href="../html/survey_responses.htm">Survey Responses</a></li>
        </ul>
      </div>
      <div class="contents">
        <h1>Your Name</h1>
        <div>
          <div class="thread">...</div>
          <div class="thread">...</div>
          <div class="thread">...</div>
          ...
          <div class="thread">...</div>
        </div>
        <div>
          <div class="thread">...</div>
          ...
        </div>
        ...
      </div>
      <div class="footer">Downloaded by Your Name on Saturday, January 23, 2016 at 9:22pm UTC</div>
    </body>
  </html>
  ```

- The `contents` div is divided into a number of sections (marked by `div`s with no class). Sections in turn contain threads, which contain the messages.
  - The criteria for separating sections are unknown


Thread Format
-------------

- General form:

  ```html
  <div class="thread">
    Your Name, Peer Name, Another Peer Name
    <div class="message">
      <div class="message_header">
        <span class="user">Sender Name</span>
        <span class="meta">Tuesday, January 21, 2014 at 4:47am UTC</span>
      </div>
    </div>
    <p>message content</p>
    <div class="message">
      ...
    </div>
    <p>message content</p>
    ...
  </div>
  ```

- A thread seems to correspond to all conversations with a given combination of peers, ever
  - Will need splitting during the postprocessing phase

- **Anomaly**: Threads appear in a random order (definitely not chronological)

- The first thing in the thread is a comma-separated list of participants
  - It includes yourself
  - Participants are specified as screen names
    - **Problem**: There are no unique account identifiers
    - It seems that these are the screen names that were valid at the time of extraction

- **Note**: Messages appear in **reverse** chronological order


Message Format
--------------

- The message format is a bit strange, you'd expect the `<p>` with the content to be inside the message `div`, but it is actually a sibling that immediately follows it

- The message header specifies:
  
  - The sender
    - Also as a screen name
    - However, this is the screen name that was valid at the time of the message (so might differ from the one in the participants list). Problematic.
  
  - The message date
    - Observed format like:
    
      `Tuesday, January 21, 2014 at 4:47am UTC+01`

    - **Problem**: Seconds are not specified
      - Must always rely on message sequence number for total ordering
    
    - It is not clear whether this format ever varies, in particular, whether the month and day names are always in English
      - However, this seems to always be a server date
        - Evidence: for me the UTC offset was always either absent or `+01` hours, even though I lived in other timezones. Therefore this is likely the local date of a server somewhere in the UK area.
      - Therefore, one can expect the date format to stay the same and be in English unless FB messes up

- There seems to always be only one `<p>` with the message content
- The content may contain significant newlines
- No formatting tags were observed in the content
- No system messages were observed
- **Problem**: No method of specifying attachments was observed. Seems they are not saved in the archive.
