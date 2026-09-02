#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ccstring.h"

static void example_create_new_ccstring(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(memcmp(buffer, "Hello, World!", 13) == 0);

    printf("String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_auto_create_ccstring(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_auto("Hello, World!");
    assert(str != NULL);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(memcmp(buffer, "Hello, World!", 13) == 0);

    printf("Auto-created String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_create_ccstring_from_view(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* original = ccstring_new("Hello, World!", 13);
    assert(original != NULL);

    ccstring_view_t* view = ccstring_view_new(original);
    assert(view != NULL);

    ccstring_t* copy = ccstring_new_from_view(view);
    assert(copy != NULL);

    const char* buffer = ccstring_get(copy);
    size_t length = ccstring_length(copy);

    assert(buffer != NULL);
    assert(length == 13);
    assert(memcmp(buffer, "Hello, World!", 13) == 0);

    printf("String from view: %s, Length: %zu\n", buffer, length);

    ccstring_destroy(&copy);
    ccstring_view_destroy(&view);
    ccstring_destroy(&original);
}

static void example_create_ccstring_from_empty(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_new_empty(0);
    assert(str != NULL);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 0);
    assert(memcmp(buffer, "", 0) == 0);

    printf("Empty String: '%s', Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_create_ccstring_from_slice(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* original = ccstring_new("Hello, World!", 13);
    assert(original != NULL);

    ccstring_slice_t* slice = ccstring_slice_new(original, 7, 12);
    assert(slice != NULL);

    ccstring_t* subset = ccstring_new_from_slice(slice);
    assert(subset != NULL);

    const char* buffer = ccstring_get(subset);
    size_t length = ccstring_length(subset);

    assert(buffer != NULL);
    assert(length == 5);
    assert(memcmp(buffer, "World", 5) == 0);

    printf("String from slice: %s, Length: %zu\n", buffer, length);

    ccstring_destroy(&subset);
    ccstring_slice_destroy(&slice);
    ccstring_destroy(&original);
}

static void example_resize_ccstring(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    printf("Original String: %s\n", ccstring_get(str));

    assert(ccstring_resize(&str, 20) == 0);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 20);
    assert(strncmp(buffer, "Hello, World!", 13) == 0);

    printf("Resized String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_copy_ccstring(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    assert(ccstring_copy(&str, "Goodbye, World!", 15) == 0);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 15);
    assert(memcmp(buffer, "Goodbye, World!", 15) == 0);

    printf("Copied String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_copy_view_and_slice(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* original = ccstring_auto("Hello, World!");
    assert(original != NULL);

    ccstring_view_t* view = ccstring_view_new(original);
    assert(view != NULL);

    ccstring_slice_t* slice = ccstring_slice_new(original, 7, 12);
    assert(slice != NULL);

    ccstring_t* target = ccstring_new_empty(1);
    assert(target != NULL);

    assert(ccstring_copy_view(&target, view) == 0);
    assert(memcmp(ccstring_get(target), "Hello, World!", 13) == 0);
    printf("Copied view: %s, Length: %zu\n", ccstring_get(target), ccstring_length(target));

    assert(ccstring_copy_slice(&target, slice) == 0);
    assert(memcmp(ccstring_get(target), "World", 5) == 0);
    printf("Copied slice: %s, Length: %zu\n", ccstring_get(target), ccstring_length(target));

    ccstring_destroy(&target);
    ccstring_slice_destroy(&slice);
    ccstring_view_destroy(&view);
    ccstring_destroy(&original);
}

static void example_append_ccstring(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str = ccstring_new("Hello", 5);
    assert(str != NULL);

    assert(ccstring_append(&str, ", World!", 8) == 0);

    const char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(memcmp(buffer, "Hello, World!", 13) == 0);

    printf("Appended String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL);
}

static void example_compare_ccstrings(void)
{
    printf("------------------------------------------------------\n");
    ccstring_t* str1 = ccstring_new("Hello", 5);
    ccstring_t* str2 = ccstring_new("Hello", 5);
    assert(str1 != NULL && str2 != NULL);

    int result = ccstring_compare(str1, str2);
    assert(result == 0);
    printf("Compare equal strings result: %d\n", result);

    ccstring_destroy(&str1);
    ccstring_destroy(&str2);

    ccstring_t* str3 = ccstring_new("Hello", 5);
    ccstring_t* str4 = ccstring_new("Goodbye", 7);
    assert(str3 != NULL && str4 != NULL);

    int result2 = ccstring_compare(str3, str4);
    printf("Compare different strings result: %d\n", result2);
    assert(result2 != 0);

    ccstring_destroy(&str3);
    ccstring_destroy(&str4);
}

static void example_manager_safe_use(void)
{
    printf("------------------------------------------------------\n");
    ccstring_manager_t manager = ccstring_manager_new(2);

    assert(ccstring_new_add_ref(&manager, "Hello", 5) != NULL);
    assert(ccstring_new_add_ref(&manager, "World", 5) != NULL);
    assert(ccstring_new_add_ref(&manager, "!", 1) != NULL);
    assert(manager.count == 3);

    printf("Manager count after add_ref: %zu\n", manager.count);
    for (size_t i = 0; i < manager.count; i++) {
        printf("Manager string %zu: %s\n", i, ccstring_get(manager.list[i]));
    }

    // Safe ownership transfer: detach one string from the manager.
    ccstring_t* detached = NULL;
    assert(ccstring_manager_remove(&manager, 1, &detached) == 0);
    assert(detached != NULL);
    printf("Detached string: %s\n", ccstring_get(detached));

    // Destroy detached string manually because ownership was transferred out of the manager.
    ccstring_destroy(&detached);
    assert(detached == NULL);

    printf("Manager count after remove: %zu\n", manager.count);
    for (size_t i = 0; i < manager.count; i++) {
        printf("Manager string %zu: %s\n", i, ccstring_get(manager.list[i]));
    }

    // Destroy manager and all remaining owned strings.
    ccstring_manager_destroy(&manager);
}

static void example_capacity_boundaries(void)
{
    printf("------------------------------------------------------\n");

    // Filling the buffer exactly must still leave room for the null terminator.
    ccstring_t* str = ccstring_new("Hello", 5);
    assert(str != NULL);

    assert(ccstring_append(&str, "!", 1) == 0);
    assert(ccstring_length(str) == 6);
    assert(memcmp(ccstring_get(str), "Hello!", 7) == 0);

    printf("Boundary append: %s, Length: %zu\n", ccstring_get(str), ccstring_length(str));
    ccstring_destroy(&str);

    // Growing zero-fills the added bytes instead of exposing stale heap contents.
    ccstring_t* grown = ccstring_new("abc", 3);
    assert(grown != NULL);

    assert(ccstring_resize(&grown, 8) == 0);
    assert(ccstring_length(grown) == 8);
    assert(memcmp(ccstring_get(grown), "abc\0\0\0\0\0", 8) == 0);

    printf("Resized String: %s, Length: %zu\n", ccstring_get(grown), ccstring_length(grown));
    ccstring_destroy(&grown);

    // A NULL source yields a defined, zero-filled buffer.
    ccstring_t* blank = ccstring_new(NULL, 4);
    assert(blank != NULL);
    assert(memcmp(ccstring_get(blank), "\0\0\0\0", 4) == 0);
    ccstring_destroy(&blank);

    // The accessors tolerate NULL like the rest of the API.
    assert(ccstring_get(NULL) == NULL);
    assert(ccstring_length(NULL) == 0);

    // ccstring_auto accepts the empty string, matching ccstring_new(str, 0).
    ccstring_t* empty = ccstring_auto("");
    assert(empty != NULL);
    assert(ccstring_length(empty) == 0);
    ccstring_destroy(&empty);
}

static void example_self_referencing_operations(void)
{
    printf("------------------------------------------------------\n");

    // Appending a string to itself: the source lives in the buffer being reallocated.
    ccstring_t* str = ccstring_new("ab", 2);
    assert(str != NULL);

    assert(ccstring_append(&str, ccstring_get(str), ccstring_length(str)) == 0);
    assert(ccstring_length(str) == 4);
    assert(memcmp(ccstring_get(str), "abab", 4) == 0);

    printf("Self-appended String: %s, Length: %zu\n", ccstring_get(str), ccstring_length(str));

    // Copying a slice of a string back over that same string: the regions overlap.
    ccstring_slice_t* slice = ccstring_slice_new(str, 1, 3);
    assert(slice != NULL);

    assert(ccstring_copy_slice(&str, slice) == 0);
    assert(ccstring_length(str) == 2);
    assert(memcmp(ccstring_get(str), "ba", 2) == 0);

    printf("Self-sliced String: %s, Length: %zu\n", ccstring_get(str), ccstring_length(str));

    // The slice points at stale content now, so it is only destroyed from here on.
    ccstring_slice_destroy(&slice);
    ccstring_destroy(&str);
}

static void example_manager_zero_initialised(void)
{
    printf("------------------------------------------------------\n");

    // A manager that never allocated a list still has to grow safely.
    ccstring_manager_t manager = {0};

    assert(ccstring_new_add_ref(&manager, "one", 3) != NULL);
    assert(ccstring_new_add_ref(&manager, "two", 3) != NULL);
    assert(ccstring_new_add_ref(&manager, "three", 5) != NULL);
    assert(manager.count == 3);

    for (size_t i = 0; i < manager.count; i++) {
        printf("Manager string %zu: %s\n", i, ccstring_get(manager.list[i]));
    }

    ccstring_manager_destroy(&manager);
    assert(manager.list == NULL);
    assert(manager.count == 0);
    assert(manager.capacity == 0);
}

static void example_capacity_invariant(void)
{
    printf("------------------------------------------------------\n");

    // capacity counts characters, not bytes allocated, so length never exceeds it.
    ccstring_t* str = ccstring_new("Hello", 5);
    assert(str != NULL);
    assert(str->length == 5);
    assert(str->length <= str->capacity);

    // Repeated appends grow geometrically instead of reallocating every call.
    size_t reallocations = 0;
    size_t previous_capacity = str->capacity;

    for (size_t i = 0; i < 1000; i++) {
        assert(ccstring_append(&str, "x", 1) == 0);
        assert(str->length <= str->capacity);

        if (str->capacity != previous_capacity) {
            reallocations++;
            previous_capacity = str->capacity;
        }
    }

    assert(ccstring_length(str) == 1005);
    assert(reallocations < 20); // Doubling means ~log2(1005), not one per append

    printf("1000 appends caused %zu reallocations, final capacity %zu\n",
           reallocations, str->capacity);

    ccstring_destroy(&str);
}

int main(void)
{
    example_create_new_ccstring();
    example_auto_create_ccstring();
    example_create_ccstring_from_view();
    example_create_ccstring_from_empty();
    example_create_ccstring_from_slice();
    example_resize_ccstring();
    example_copy_ccstring();
    example_copy_view_and_slice();
    example_append_ccstring();
    example_compare_ccstrings();
    example_manager_safe_use();
    example_capacity_boundaries();
    example_self_referencing_operations();
    example_manager_zero_initialised();
    example_capacity_invariant();

    return EXIT_SUCCESS;
}