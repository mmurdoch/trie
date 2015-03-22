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

trie_error_t trie_add_key(trie_t* trie, const char* key);

trie_error_t trie_contains(trie_t* trie, const char* key, bool* contains);

trie_error_t trie_destroy(trie_t* trie);

#endif /* TRIE_H */
