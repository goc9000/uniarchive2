Libpurple System Messages
=========================


General Description
-------------------

Libpurple system messages are subject-less messages used to indicate events such as file transfers, status changes, etc. in Pidgin and Adium. The meaning and parameters of the events can be obtained by matching the system message text against a series of patterns.

Libpurple messages may be rendered as either plain text or HTML. If the latter, tags are only used to add extra information to subjects and filenames.


Observed Patterns
-----------------

#### Status Changes

- *Subject Name* ` has signed on.`
- *Subject Name* ` logged in.`
  - Seems to be equivalent to the above
- *Subject Name* ` has signed off.`
- *Subject Name* ` logged out.`
  - Seems to be equivalent to the above
- *Subject Name* ` has gone away.`
- *Subject Name* ` is no longer away.`
- *Subject Name* ` has become idle.`
- *Subject Name* ` is no longer idle.`

#### Pings

- `Buzzing ` *Subject Name* `...`
- `: Buzz!!`
  - This is used in Pidgin text archives in this exact form, i.e. `(12:34:00 AM) : Buzz!!` to indicate you buzzing the other party
- `Unable to buzz, because ` *Subject Name* `does not support it or does not wish to receive buzzes now.`
- *Subject Name* ` has buzzed you!`
- *Subject Name* ` just sent you a Buzz!`
  - Alternate pattern used in old Pidgin text archives

#### File Transfers

- *Subject Name* ` is trying to send you a group of ` *N* ` files.`
- *Subject Name* ` is offering to send file ` *filename.ext*
- `Offering to send ` *filename.ext* ` to ` *Subject Name*
- `Starting transfer of ` *filename.ext* ` from ` *Subject Name*
- `You cancelled the transfer of ` *filename.ext*
- *Subject Name* ` cancelled the transfer of ` *filename.ext*
  - Also spelled `canceled` (single l) in older Pidgin versions
- `File transfer cancelled`
  - Note that in this variant, the filename is unspecified
  - Not known whether this refers to inbound or outbound transfers, or both
- `Transfer of file ` *filename.ext* ` complete`

#### Join/Leave

- *Subject Name* ` entered the room.`
- *Subject Name* ` left the room.`
- *Subject Name* ` has left the conversation.`
  - Used only in 1:1 conversations

#### Naming

- *Old Subject Name* ` is now known as ` *New Subject Name* `.`

#### Call Invitations

- *Subject Name* ` has sent you a webcam invite, which is not yet supported.`

#### Message Send Fails

- `Unable to send message: The message is too large.`
- `Message could not be sent because the user is offline:`
  - The unsent message follows on the next line
- `Message could not be sent because a connection error occured:`
  - The unsent message follows on the next line

#### Special Events

- `Logging started. Future messages in this conversation will be logged.`
- `Logging stopped. Future messages in this conversation will not be logged.`


Subjects
--------

- Usually subject is just specified as plain text or simple HTML (with entities but no tags)
- Sometimes, if it is an account name, a hash suffix is added as in this example:

  `account_name@gmail.com/F0D26543`
  
  - Meaning of hash and conditions for its presence are unknown

- Also, sometimes both the screen name and account name are rendered as in the following example:

  `Screen Name [<em>account_name@domain.com</em>]`
  
  or
  
  `Screen Name [<em>account_name@domain.com/1234ABCD</em>]`
  

Filenames
---------

- Usually rendered as-is: `filename.ext`
- Sometimes wrapped by a link tag specifying the complete path:

  `<a href="file:///local/path/to/filename.ext">filename.ext</a>`
