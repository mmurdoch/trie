#include <stdint.h>
#include "cutest/CuTest.h"
#include "trie.h"

trie_t* trie_create_checked(CuTest* test) {
    trie_t* trie;

    if (trie_create(&trie) != TRIE_SUCCESS) {
        CuFail(test, "trie_init failed");
    }

    return trie;
}

void trie_add_word_checked(CuTest* test, trie_t* trie, const char* word) {
    if (trie_add_word(trie, word) != TRIE_SUCCESS) {
        CuFail(test, "trie_add_key failed");
    }
}

void trie_contains_word_checked(CuTest* test, trie_t* trie, const char* word,
    bool* contains) {
    if (trie_contains_word(trie, word, contains) != TRIE_SUCCESS) {
        CuFail(test, "trie_contains failed");
    }
}

void trie_destroy_checked(CuTest* test, trie_t* trie) {
    if (trie_destroy(trie) != TRIE_SUCCESS) {
        CuFail(test, "trie_destroy failed");
    }
}

void assert_trie_contains_word(CuTest* test, trie_t* trie, const char* word) {
    bool contains;
    trie_contains_word_checked(test, trie, word, &contains);
    CuAssert(test, word, contains);
}

void assert_trie_does_not_contain_word(CuTest* test, trie_t* trie,
    const char* word) {
    bool contains;
    trie_contains_word_checked(test, trie, word, &contains);
    CuAssertFalse(test, contains);
}

void test_add_null_trie_fails(CuTest* test) {
    trie_result_t add_result = trie_add_word(NULL, "word");

    CuAssertIntEquals(test, TRIE_NULL, add_result);
}

void test_add_null_pointer_fails(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_result_t add_result = trie_add_word(trie, NULL);

    CuAssertIntEquals(test, TRIE_WORD_NULL, add_result);

    trie_destroy_checked(test, trie);
}

void test_add_empty_string_fails(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_result_t add_result = trie_add_word(trie, "");

    CuAssertIntEquals(test, TRIE_WORD_EMPTY, add_result);

    trie_destroy_checked(test, trie);
}

void test_contains_null_trie_fails(CuTest* test) {
    bool contains;
    trie_result_t contains_result = trie_contains_word(NULL, "word", &contains);

    CuAssertIntEquals(test, TRIE_NULL, contains_result);
}

void test_contains_with_no_words(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    assert_trie_does_not_contain_word(test, trie, "hello");

    trie_destroy_checked(test, trie);
}

void test_contains_empty_string_is_false(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    assert_trie_does_not_contain_word(test, trie, "");

    trie_destroy_checked(test, trie);
}

void test_contains_with_single_char_word(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* a = "a";
    trie_add_word_checked(test, trie, a);

    assert_trie_contains_word(test, trie, a);

    trie_destroy_checked(test, trie);
}

void test_contains_with_two_chars_word(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* hi = "hi";
    trie_add_word_checked(test, trie, hi);

    assert_trie_contains_word(test, trie, hi);

    trie_destroy_checked(test, trie);
}

void test_contains_with_two_overlapping_words(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* bone = "bone";
    trie_add_word_checked(test, trie, bone);

    const char* body = "body";
    trie_add_word_checked(test, trie, body);

    assert_trie_contains_word(test, trie, bone);
    assert_trie_contains_word(test, trie, body);

    trie_destroy_checked(test, trie);
}

void test_contains_matches_full_word(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_add_word_checked(test, trie, "a");

    assert_trie_does_not_contain_word(test, trie, "an");

    trie_destroy_checked(test, trie);
}

void test_contains_does_not_match_prefixes(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_add_word_checked(test, trie, "an");

    assert_trie_does_not_contain_word(test, trie, "a");

    trie_destroy_checked(test, trie);
}

void test_contains_matches_prefix_words(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_add_word_checked(test, trie, "an");
    trie_add_word_checked(test, trie, "a");

    assert_trie_contains_word(test, trie, "a");

    trie_destroy_checked(test, trie);
}

void test_get_no_prefix_matches(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    size_t words_length = 1U;
    const char* words[words_length];
    size_t word_count;
    if (trie_get_words_matching_prefix(
        trie, "a", words, words_length, &word_count) != TRIE_SUCCESS) {
        CuFail(test, "trie_get_words_matching_prefix failed");
    }

    CuAssertIntEquals(test, 0U, word_count);

    trie_destroy_checked(test, trie);
}

void test_get_prefix_matches(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_add_word_checked(test, trie, "aardvark");
    trie_add_word_checked(test, trie, "wolf");
    trie_add_word_checked(test, trie, "aardwolf");

    size_t words_length = 2U;
    const char* words[words_length];
    size_t word_count;
    if (trie_get_words_matching_prefix(
        trie, "aard", words, words_length, &word_count) != TRIE_SUCCESS) {
        CuFail(test, "trie_get_words_matching_prefix failed");
    }

    CuAssertIntEquals(test, 2U, word_count);
    CuAssertStrEquals(test, "aardvark", words[0]);
    CuAssertStrEquals(test, "aardwolf", words[1]);

    trie_destroy_checked(test, trie);
}

int64_t currently_allocated_memory;
void memory_allocated() {
    currently_allocated_memory++;
}

void memory_deallocated() {
    currently_allocated_memory--;
}

void set_up_memory_leak_detection() {
    currently_allocated_memory = 0;
    trie_set_memory_allocation_listener(memory_allocated);
    trie_set_memory_deallocation_listener(memory_deallocated);
}

void assert_no_memory_leaks(CuTest* test) {
    CuAssertTrue(test, currently_allocated_memory == 0);
}

void test_destroy_empty_does_not_leak_memory(CuTest* test) {
    set_up_memory_leak_detection();
    trie_t* trie = trie_create_checked(test);

    trie_destroy_checked(test, trie);

    assert_no_memory_leaks(test);
}

void test_destroy_single_word_trie_does_not_leak_memory(CuTest* test) {
    set_up_memory_leak_detection();
    trie_t* trie = trie_create_checked(test);
    trie_add_word_checked(test, trie, "word");

    trie_destroy_checked(test, trie);

    assert_no_memory_leaks(test);
}

void test_destroy_multi_word_trie_does_not_leak_memory(CuTest* test) {
    set_up_memory_leak_detection();
    trie_t* trie = trie_create_checked(test);
    trie_add_word_checked(test, trie, "one");
    trie_add_word_checked(test, trie, "two");

    trie_destroy_checked(test, trie);

    assert_no_memory_leaks(test);
}
