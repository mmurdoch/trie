#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _trie_node_t _trie_node_t;

typedef struct {
    _trie_node_t* head_node;
} _trie_node_list_t;

struct _trie_node_t {
    char ch;
    const char* word;
    _trie_node_list_t children;
    _trie_node_t* next;
};

struct trie_t {
    _trie_node_list_t roots;
};

trie_error_t trie_create(trie_t** trie) {
    trie_t* created = malloc(sizeof(trie_t));
    if (created == NULL) {
        return TRIE_FAIL;
    }

    created->roots.head_node = NULL;

    *trie = created;
    return TRIE_SUCCESS;
}

// Returns the node within node_list which contains key_char or NULL if no node
// exists in the node list for the given key_char
_trie_node_t* _get_node_with_char(_trie_node_list_t* node_list, char ch) {
    _trie_node_t* current_node = node_list->head_node;
    while (current_node != NULL) {
        if (current_node->ch == ch) {
            return current_node;
        }

        current_node = current_node->next;
    }

    return NULL;
}

// Attempts to create a node containing the given character, returning it if
// successful, or NULL if memory allocation fails
_trie_node_t* _create_node(char ch) {
    _trie_node_t* node = malloc(sizeof(_trie_node_t));
    if (node == NULL) {
        return NULL;
    }

    node->ch = ch;
    node->word = NULL;
    node->next = NULL;

    return node;
}

// Inserts the given node into the given node list
void _insert_node(_trie_node_list_t* node_list, _trie_node_t* node) {
    if (node_list->head_node == NULL) {
        node_list->head_node = node;
    }
    else {
        _trie_node_t* previous_node = node_list->head_node;
        while (previous_node->next != NULL) {
            previous_node = previous_node->next;
        }
        previous_node->next = node;
    }
}

trie_error_t trie_add_key(trie_t* trie, const char* key) {
    _trie_node_list_t* current_node_list = &(trie->roots);

    for (size_t i = 0U; i < strlen(key); i++) {
        char current_key_char = key[i];
        _trie_node_t* node_with_char =
            _get_node_with_char(current_node_list, current_key_char);
        if (node_with_char == NULL) {
            node_with_char = _create_node(current_key_char);
            if (node_with_char == NULL) {
                return TRIE_FAIL;
            }

            _insert_node(current_node_list, node_with_char);
        }

        if (i == strlen(key)-1) {
            node_with_char->word = key;
        }

        current_node_list = &(node_with_char->children);
    }

    return TRIE_SUCCESS;
}

trie_error_t trie_contains(trie_t* trie, const char* key, bool* contains) {
    *contains = true;
    _trie_node_list_t* current_node_list = &(trie->roots);

    for (size_t i = 0U; i < strlen(key); i++) {
        _trie_node_t* node_with_char =
            _get_node_with_char(current_node_list, key[i]);
        if (node_with_char == NULL) {
            *contains = false;
            break;
        }

        if (i == strlen(key)-1) {
            if (node_with_char->word == NULL) {
                *contains = false;
            }
        }

        current_node_list = &(node_with_char->children);
    }

    return TRIE_SUCCESS;
}

size_t _get_descendant_words(_trie_node_t* from_node,
    const char** matches, size_t matches_length) {
    size_t match_count = 0U;

    if (from_node->word != NULL) {
        matches[match_count] = from_node->word;
        match_count++;
    }

    _trie_node_t* current_node = from_node->children.head_node;
    while (current_node != NULL && match_count < matches_length) {
        match_count += _get_descendant_words(
            current_node, matches+match_count, matches_length-match_count);
        current_node = current_node->next;
    }

    return match_count;
}

trie_error_t trie_get_prefix_matches(trie_t* trie, const char* prefix,
    const char** matches, size_t matches_length, size_t* match_count) {

    _trie_node_list_t* current_node_list = &(trie->roots);
    _trie_node_t* node_with_char = NULL;

    // Walk the trie a prefix char at a time
    // If the whole of the prefix is not found
    //   set match_count to zero and return
    for (size_t i = 0U; i < strlen(prefix); i++) {
        node_with_char = _get_node_with_char(current_node_list, prefix[i]);
        if (node_with_char == NULL) {
            *match_count = 0U;
            return TRIE_SUCCESS;
        }

        current_node_list = &(node_with_char->children);
    }

    // Otherwise
    //   walk all descendant nodes depth first and capture all
    //   words up to matches_length and return
    *match_count = _get_descendant_words(
        node_with_char, matches, matches_length);

    return TRIE_SUCCESS;
}

trie_error_t trie_destroy(trie_t* trie) {
    free(trie);
    return TRIE_SUCCESS;
}
