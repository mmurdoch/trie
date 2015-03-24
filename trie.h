#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stddef.h>

/**
 * A trie used for retrieving words matching a prefix. Useful, for example, in
 * predictive typing applications.
 */
typedef struct trie_t trie_t;

/**
 * The result of a call to a trie function.
 */
typedef enum {
    TRIE_SUCCESS,
    TRIE_FAIL
} trie_result_t;

/**
 * Creates an empty trie. To prevent resource leakage, each call to this
 * function must be matched by a call to trie_destroy().
 *
 * @param trie (out) set to the created trie
 * @return whether or not the creation was successful
 */
trie_result_t trie_create(trie_t** trie);

/**
 * Adds a word to a trie.
 *
 * @param trie trie to which to add the word
 * @param word word to add
 * @return whether or not the addition was successful
 */
trie_result_t trie_add_word(trie_t* trie, const char* word);

/**
 * Determines whether or not a trie contains a specified word.
 *
 * @param trie trie to check
 * @param word word for which to search
 * @param contains (out) set to true if the word was found, false otherwise
 * @return whether or not the search was successfull
 */
trie_result_t trie_contains_word(trie_t* trie, const char* word, bool* contains);

/**
 * Retrieves words contained within a trie which start with the specified
 * prefix. The number of words retrieved is bounded by the length of the
 * specified output array.
 *
 * @param trie trie to search
 * @param prefix the prefix for which to search
 * @param words (out) an array into which to write the retrieved words
 * @param words_length the length of the words array
 * @param word_count (out) set to the number of words retrieved. This will
 *        never be greater than words_length
 * @return whether or not the retrieval was successful
 */
trie_result_t trie_get_words_matching_prefix(trie_t* trie, const char* prefix,
    const char** words, size_t words_length, size_t* word_count);

/**
 * Destroys a trie created by a call to trie_create().
 *
 * @param trie the trie to destroy
 * @return whether or not the destruction was successful
 */
trie_result_t trie_destroy(trie_t* trie);

#endif /* TRIE_H */
