Archive Format Research: MSN Messenger (XML format)
===================================================


Filename Format
---------------

- The filename takes the form:

  *own_account_id* `/History/` *peer_account_id* `.xml`

  Where *account_id* takes one of two formats:

    - Either the account email address (in old versions of MSN messenger), e.g. `account@hotmail.com`
    - Or an encoded form, like `account1234567`. Note how the domain part is replaced by a numeric hash. The formula for the hash can be defined recursively like this:

          hash("") = 0
          hash(string + last_character) = (hash(string) * 101 + ascii_code(last_character)) mod 2^32
  
      The original account can generally be recovered from the encoded form, by trying all posibilities for the domain and checking which version, when hashed, matches the hash in the filename. Note that we must also try different lengths for the account name part, because if it ends with a number, it is not clear where the account name ends and the hash begins.

### Anomalies Observed ###

- In my archive, there is also a file where *peer_account_id* is equal to *own_account_id*, but, contrary to intuition, it does not consist of conversations where I talk to myself, nor of conferences. I can't figure out what's special about the conversations therein that cause them to be assigned to this file.


Other File Properties
---------------------

- Last modified date
  - Nearly always corresponds to date of last event in file
  - Unreliable due to FAT timezone problem


Distribution of Conversations Across Files
------------------------------------------

- A file contains all conversations with that peer, ever
- There seems to be no mechanism for splitting a per-peer archive among multiple files
- Given the form of the XML format (it has a header saying how many conversations are in the file), it seems that MSN messenger always loads the entire file in memory and saves it back. The programmers must have assumed that archives never grow to a size such that they do not fit in RAM anymore.


File Format
-----------

- Well-formed XML file
- General form:

  ```xml
  <?xml version="1.0"?>
  <?xml-stylesheet type='text/xsl' href='MessageLog.xsl'?>
  <Log FirstSessionID="1" LastSessionID="37">
      <Message Date="..." Time="..." DateTime="..." SessionID="1">...</Message>
      <Message Date="..." Time="..." DateTime="..." SessionID="1">...</Message>
      <Message Date="..." Time="..." DateTime="..." SessionID="1">...</Message>
      ...
      <Message Date="..." Time="..." DateTime="..." SessionID="2">...</Message>
      ...
  </Log>
  ```

- The events are divided into **sessions**; these are assumed to correspond to conversations
- Three type of event nodes observed: `<Message>`, `<Invitation>` and `<InvitationResponse>`

### Anomalies Observed ###

- The `LastSessionID` is not reliable, I have observed files containing seessions with higher numbers than specified


General Format of an Event Node
-------------------------------

```xml
<EventTagName Date="dd/mm/yyyy" Time="hh:mm:dd" DateTime="yyyy-mm-ddThh:mm:ss.nnnZ" SessionID="...">
  <From><User FriendlyName="..."/></From>
  ...
</EventTagName>
```

- `Date` and `Time` seem to be the *local* date and time, i.e. expressed in the timezone that applied at the time the messages were received
  - It is not clear if the local date format is consistent or locale-dependent
- `DateTime`, conversely, expresses the absolute UTC time. It is expressed in the standard XML/ISO 8601 date format as described here: http://books.xmlschemata.org/relaxng/ch19-77049.html . The `nnn` here stand for milliseconds and the `Z` signifies that the time is UTC.
- The `FriendlyName` is the screen name of the user (not the account, which complicates matters)
  - No other form of specifying the user has been observed
  - The exact significance depends on the event (it is not always the sender)

----------------

WORK IN PROGRESS

----------------

Open Questions
--------------

- What do conferences look like?
