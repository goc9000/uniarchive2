Design Notes
============

(work in progress)

- requires:
  - C++11
  - Qt4
  - libtclap-dev

- Caveats and surprises:
  - It is possible to have conversations between accounts that both belong to you

- RawConversationSource = extracts any number of RawConversations from some source (filesystem, database, network etc.)
  Crucial to its definition:
    - does not attempt to infer missing data, only represent in a common format all the data in the convo
    - does not attempt to integrate with other sources and/or compare
    - somewhat like the Map phase in MapReduce
- RawConversation = conversation in a unified format, but with possibly incomplete, ambiguous or undetermined info.
- Integrator (?) = takes RawConversations and uses its global view of all conversations (as well as hints, heuristics,
  etc.) to resolve all ambiguities.

