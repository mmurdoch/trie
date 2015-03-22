#include "CuTest.h"
#include "trie.h"

trie_t* trie_create_checked(CuTest* test) {
    trie_t* trie;

    if (trie_create(&trie) != TRIE_SUCCESS) {
        CuFail(test, "trie_init failed");
    }

    return trie;
}

void trie_add_key_checked(CuTest* test, trie_t* trie, const char* key) {
    if (trie_add_key(trie, key) != TRIE_SUCCESS) {
        CuFail(test, "trie_add_key failed");
    }
}

void trie_destroy_checked(CuTest* test, trie_t* trie) {
    if (trie_destroy(trie) != TRIE_SUCCESS) {
        CuFail(test, "trie_destroy failed");
    }
}

void assert_trie_contains_key(CuTest* test, trie_t* trie, const char* key) {
    bool contains;
    if (trie_contains(trie, key, &contains) != TRIE_SUCCESS) {
        CuFail(test, "trie_contains failed");
    }
    CuAssert(test, key, contains);
}

void assert_trie_does_not_contain_key(CuTest* test, trie_t* trie, const char* key) {
    bool contains;
    if (trie_contains(trie, key, &contains) != TRIE_SUCCESS) {
        CuFail(test, "trie_contains failed");
    }
    CuAssertFalse(test, contains);
}

void test_contains_with_no_keys(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    assert_trie_does_not_contain_key(test, trie, "hello");

    trie_destroy_checked(test, trie);
}

void test_contains_with_single_char_key(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* key = "a";
    trie_add_key_checked(test, trie, key);

    assert_trie_contains_key(test, trie, key);

    trie_destroy_checked(test, trie);
}

void test_contains_with_two_chars_key(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* key = "hi";
    trie_add_key_checked(test, trie, key);

    assert_trie_contains_key(test, trie, key);

    trie_destroy_checked(test, trie);
}

void test_contains_with_two_overlapping_keys(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    const char* key_one = "bone";
    trie_add_key_checked(test, trie, key_one);

    const char* key_two = "body";
    trie_add_key_checked(test, trie, key_two);

    assert_trie_contains_key(test, trie, key_one);
    assert_trie_contains_key(test, trie, key_two);

    trie_destroy_checked(test, trie);
}

void test_contains_matches_full_key(CuTest* test) {
    trie_t* trie = trie_create_checked(test);

    trie_add_key_checked(test, trie, "a");

    assert_trie_does_not_contain_key(test, trie, "an");

    trie_destroy_checked(test, trie);
}
