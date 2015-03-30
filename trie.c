#include "trie.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

void (*memory_allocation_listener)() = NULL;

void (*memory_deallocation_listener)() = NULL;

void* _allocate_memory(size_t size) {
    void* allocated_memory = malloc(size);

    if (allocated_memory != NULL && memory_allocation_listener != NULL) {
        memory_allocation_listener();
    }

    return allocated_memory;
}

void _deallocate_memory(void* memory) {
    free(memory);

    if (memory_deallocation_listener != NULL) {
        memory_deallocation_listener();
    }
}

trie_result_t trie_create(trie_t** trie) {
    trie_t* created = _allocate_memory(sizeof(trie_t));
    if (created == NULL) {
        return TRIE_MALLOC_FAIL;
    }

    created->roots.head_node = NULL;

    *trie = created;

    return TRIE_SUCCESS;
}

// Returns the node within node_list which contains ch or NULL if no node
// exists in the node list for the given ch
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
    _trie_node_t* node = _allocate_memory(sizeof(_trie_node_t));
    if (node == NULL) {
        return NULL;
    }

    node->ch = ch;
    node->word = NULL;
    node->children.head_node = NULL;
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

trie_result_t trie_add_word(trie_t* trie, const char* word) {
    if (trie == NULL) {
        return TRIE_NULL;
    }

    if (word == NULL) {
        return TRIE_WORD_NULL;
    }

    if (strlen(word) == 0U) {
        return TRIE_WORD_EMPTY;
    }

    _trie_node_list_t* current_node_list = &(trie->roots);

    for (size_t i = 0U; i < strlen(word); i++) {
        char current_char = word[i];
        _trie_node_t* node_with_char =
            _get_node_with_char(current_node_list, current_char);
        if (node_with_char == NULL) {
            node_with_char = _create_node(current_char);
            if (node_with_char == NULL) {
                return TRIE_MALLOC_FAIL;
            }

            _insert_node(current_node_list, node_with_char);
        }

        if (i == strlen(word)-1) {
            node_with_char->word = word;
        }

        current_node_list = &(node_with_char->children);
    }

    return TRIE_SUCCESS;
}

trie_result_t trie_contains_word(trie_t* trie, const char* word,
    bool* contains) {
    if (trie == NULL) {
        return TRIE_NULL;
    }

    if (word == NULL) {
        return TRIE_WORD_NULL;
    }

    if (strlen(word) == 0U) {
        *contains = false;
        return TRIE_SUCCESS;
    }

    *contains = true;
    _trie_node_list_t* current_node_list = &(trie->roots);

    for (size_t i = 0U; i < strlen(word); i++) {
        _trie_node_t* node_with_char =
            _get_node_with_char(current_node_list, word[i]);
        if (node_with_char == NULL) {
            *contains = false;
            break;
        }

        if (i == strlen(word)-1) {
            if (node_with_char->word == NULL) {
                *contains = false;
            }
        }

        current_node_list = &(node_with_char->children);
    }

    return TRIE_SUCCESS;
}

size_t _get_descendant_words(_trie_node_t* from_node,
    const char** words, size_t words_length) {
    size_t word_count = 0U;

    if (from_node->word != NULL) {
        words[word_count] = from_node->word;
        word_count++;
    }

    _trie_node_t* current_node = from_node->children.head_node;
    while (current_node != NULL && word_count < words_length) {
        word_count += _get_descendant_words(
            current_node, words+word_count, words_length-word_count);
        current_node = current_node->next;
    }

    return word_count;
}

trie_result_t trie_get_words_matching_prefix(trie_t* trie, const char* prefix,
    const char** words, size_t words_length, size_t* word_count) {

    _trie_node_list_t* current_node_list = &(trie->roots);
    _trie_node_t* node_with_char = NULL;

    for (size_t i = 0U; i < strlen(prefix); i++) {
        node_with_char = _get_node_with_char(current_node_list, prefix[i]);
        if (node_with_char == NULL) {
            *word_count = 0U;
            return TRIE_SUCCESS;
        }

        current_node_list = &(node_with_char->children);
    }

    *word_count = _get_descendant_words(node_with_char, words, words_length);

    return TRIE_SUCCESS;
}

void _destroy_node_list(_trie_node_list_t* node_list);

void _destroy_node_and_following_siblings(_trie_node_t* node) {
    _trie_node_t* next_node = node->next;
    if (next_node != NULL) {
        _destroy_node_and_following_siblings(next_node);
    }

    _destroy_node_list(&(node->children));
    _deallocate_memory(node);
}

void _destroy_node_list(_trie_node_list_t* node_list) {
    _trie_node_t* head_node = node_list->head_node;
    if (head_node != NULL) {
        _destroy_node_and_following_siblings(head_node);
    }
}

trie_result_t trie_destroy(trie_t* trie) {
    _destroy_node_list(&(trie->roots));
    _deallocate_memory(trie);

    return TRIE_SUCCESS;
}

void trie_set_memory_allocation_listener(void (*listener)()) {
    memory_allocation_listener = listener;
}

void trie_set_memory_deallocation_listener(void (*listener)()) {
    memory_deallocation_listener = listener;
}
