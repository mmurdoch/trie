#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct trie_t trie_t;

typedef enum {
    TRIE_SUCCESS,
    TRIE_FAIL
} trie_error_t;

trie_error_t trie_create(trie_t** trie);

trie_error_t trie_add_word(trie_t* trie, const char* word);

trie_error_t trie_contains_word(trie_t* trie, const char* word, bool* contains);

trie_error_t trie_get_words_matching_prefix(trie_t* trie, const char* prefix,
    const char** words, size_t words_length, size_t* word_count);

trie_error_t trie_destroy(trie_t* trie);

#endif /* TRIE_H */
