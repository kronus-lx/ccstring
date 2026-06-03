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

    int result = ccstring_resize(&str, 20);
    assert(result == 0);

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

    int result = ccstring_copy(&str, "Goodbye, World!", 15);
    assert(result == 0);

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

    int result = ccstring_copy_view(&target, view);
    assert(result == 0);
    assert(memcmp(ccstring_get(target), "Hello, World!", 13) == 0);
    printf("Copied view: %s, Length: %zu\n", ccstring_get(target), ccstring_length(target));

    result = ccstring_copy_slice(&target, slice);
    assert(result == 0);
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

    int result = ccstring_append(&str, ", World!", 8);
    assert(result == 0);

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

    ccstring_t* str1 = ccstring_new_add_ref(&manager, "Hello", 5);
    ccstring_t* str2 = ccstring_new_add_ref(&manager, "World", 5);
    ccstring_t* str3 = ccstring_new_add_ref(&manager, "!", 1);
    assert(str1 != NULL && str2 != NULL && str3 != NULL);

    printf("Manager count after add_ref: %zu\n", manager.count);
    for (size_t i = 0; i < manager.count; i++) {
        printf("Manager string %zu: %s\n", i, ccstring_get(manager.list[i]));
    }

    // Safe ownership transfer: detach one string from the manager.
    ccstring_t* detached = NULL;
    int result = ccstring_manager_remove(&manager, 1, &detached);
    assert(result == 0);
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

    return EXIT_SUCCESS;
}