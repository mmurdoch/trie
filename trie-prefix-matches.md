What should happen when passing empty string to 'add', 'contains' and
'prefix_matches'?

Do we need to copy the words passed to add_word()? We probably should since we
don't know how the words have been created. For example if a user was adding a
previously unknown word, dynamically allocated memory would likely have been
used to store the word. By not copying the words the calling application will
need to hold onto all these words in a separate data structure to ensure that
they can be retrieved from the trie. This seems like an unnecessary overhead.
