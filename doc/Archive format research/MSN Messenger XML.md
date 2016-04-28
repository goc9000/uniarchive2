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

- **Caveat**: in my archive, there is also a file where *peer_account_id* is equal to *own_account_id*, but, contrary to intuition, it does not consist of conversations where I talk to myself, nor of conferences. I can't figure out what's special about the conversations therein that cause them to be assigned to this file.


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

----------------

WORK IN PROGRESS

----------------

Open Questions
--------------

- What do conferences look like?
