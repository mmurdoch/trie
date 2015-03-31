#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

int main() {
    trie_t* trie;
    trie_result_t create_result = trie_create(&trie);
    if (create_result != TRIE_SUCCESS) {
        printf("trie_create failed\n");
        return create_result;
    }

    FILE* dictionary_file;
    char* line = NULL;
    size_t capacity = 0;
    ssize_t line_length;

    dictionary_file = fopen("/usr/share/dict/words", "r");
    if (dictionary_file == NULL)
       return 1;

    while ((line_length = getline(&line, &capacity, dictionary_file)) != -1) {
        size_t word_length = line_length - 1;
        line[word_length] = '\0';
        trie_result_t add_result = trie_add_word(trie, line);
        if (add_result != TRIE_SUCCESS) {
            printf("trie_add_word failed\n");
            return add_result;
        }
    }

    fclose(dictionary_file);
    free(line);

    const char* prefix = "bar";
    size_t words_length = 6U;
    const char* words[words_length];
    size_t word_count;
    trie_result_t get_result = trie_get_words_matching_prefix(
        trie, prefix, words, words_length, &word_count);
    if (get_result != TRIE_SUCCESS) {
        printf("trie_get_words_matching_prefix failed\n");
        return get_result;
    }

    printf("First %zu words matching prefix %s:\n", word_count, prefix);
    for (size_t i = 0U; i < word_count; i++) {
        printf("%s", words[i]);
        printf("\n");
    }

    trie_result_t destroy_result = trie_destroy(trie);

    return 0;
}
