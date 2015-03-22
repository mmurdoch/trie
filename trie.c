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

trie_error_t trie_add_key(trie_t* trie, const char* key) {
    _trie_node_list_t* current = &(trie->roots);

    for (size_t i = 0U; i < strlen(key); i++) {
        current->head_node = malloc(sizeof(_trie_node_t));
        if (current->head_node == NULL) {
            return TRIE_FAIL;
        }

        current->head_node->ch = key[i];
        current->head_node->next = NULL;
        current->head_node->children.head_node = NULL;
        current = &(current->head_node->children);
    }

    return TRIE_SUCCESS;
}

trie_error_t trie_contains(trie_t* trie, const char* key, bool* contains) {
    *contains = false;
    _trie_node_list_t current = trie->roots;

    for (size_t i = 0U; i < strlen(key); i++) {
        _trie_node_t* current_node = current.head_node;

        bool found = false;
        while (current_node != NULL) {
            char current_char = current_node->ch;

            if (current_node->ch == key[i]) {
                current = current_node->children;
                found = true;
                break;
            }
            current_node = current_node->next;
        }

        if (current.head_node == NULL && found == true) {
            *contains = true;
        }
    }

    return TRIE_SUCCESS;
}

trie_error_t trie_destroy(trie_t* trie) {
    free(trie);
    return TRIE_SUCCESS;
}
