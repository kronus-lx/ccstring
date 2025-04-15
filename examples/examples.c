#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ccstring.h"

void creating_new_ccstring()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);
    
    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);
    
    assert(buffer != NULL);
    assert(length == 13);
    assert(strcmp(buffer, "Hello, World!") == 0);

    printf("String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL); // Ensure str is NULL after destruction
}

void creating_new_ccstring_from_view()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    ccstring_view_t* view = ccstring_view_new(str);
    assert(view != NULL);

    ccstring_t* new_str = ccstring_new_from_view(view);
    assert(new_str != NULL);

    char* buffer = ccstring_get(new_str);
    size_t length = ccstring_length(new_str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(strcmp(buffer, "Hello, World!") == 0);

    printf("String from view: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&new_str);
    ccstring_view_destroy(&view);
    assert(new_str == NULL); // Ensure new_str is NULL after destruction
    assert(view == NULL); // Ensure view is NULL after destruction
}

void creating_new_ccstring_from_empty()
{
    ccstring_t* str = ccstring_new_empty(0);
    assert(str != NULL);

    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 0);
    assert(strcmp(buffer, "") == 0);

    printf("Empty String: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&str);
    assert(str == NULL); // Ensure str is NULL after destruction
}

void creating_new_ccstring_from_slice()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    ccstring_slice_t* slice = ccstring_slice_new(str, 0, 3);
    assert(slice != NULL);

    ccstring_t* new_str = ccstring_new_from_slice(slice);
    assert(new_str != NULL);

    char* buffer = ccstring_get(new_str);
    size_t length = ccstring_length(new_str);

    assert(buffer != NULL);
    assert(length == 5);
    assert(strcmp(buffer, "Hello") == 0);

    printf("String from slice: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&new_str);
    ccstring_slice_destroy(&slice);

    assert(new_str == NULL); // Ensure new_str is NULL after destruction
    assert(slice == NULL); // Ensure slice is NULL after destruction
}

void resize_string_example()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    printf("Original String: %s\n", ccstring_get(str));

    // Resize the string
    int result = ccstring_resize(&str, 20);
    assert(result == 0);

    // Check the resized string
    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 20);

    printf("Resized String: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&str);
    assert(str == NULL); // Ensure str is NULL after destruction
}

void copy_string_example()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    // Copy a new string into the existing ccstring
    int result = ccstring_copy(&str, "Goodbye, World!", 15);
    assert(result == 0);

    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 15);
    assert(strcmp(buffer, "Goodbye, World!") == 0);

    printf("Copied String: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&str);
    assert(str == NULL); // Ensure str is NULL after destruction
}

void append_string_example()
{
    ccstring_t* str = ccstring_new("Hello", 5);
    assert(str != NULL);

    // Append a new string to the existing ccstring
    int result = ccstring_append(&str, ", World!", 8);
    assert(result == 0);

    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(strcmp(buffer, "Hello, World!") == 0);

    printf("Appended String: %s, Length: %zu\n", buffer, length);
    
    ccstring_destroy(&str);

    assert(str == NULL); // Ensure str is NULL after destruction
}

void compare_strings()
{
    ccstring_t* str1 = ccstring_new("Hello", 5);
    assert(str1 != NULL);

    ccstring_t* str2 = ccstring_new("Hello", 5);
    assert(str2 != NULL);

    // Compare the two strings
    int result = ccstring_compare(str1, str2);
    assert(result == 0);

    printf("Strings are equal: %d\n", result );
    printf("result: %d\n", result );

    ccstring_destroy(&str1);
    ccstring_destroy(&str2);

    assert(str1 == NULL); // Ensure str1 is NULL after destruction
    assert(str2 == NULL); // Ensure str2 is NULL after destruction

    ccstring_t* str3 = ccstring_new("Hello", 5);
    assert(str3 != NULL);

    ccstring_t* str4 = ccstring_new("Goodbye", 7); // Correct length for "Goodbye"
    assert(str4 != NULL);

    int result2 = ccstring_compare(str3, str4);
    printf("result: %d\n", result2 );
    
    // Assert the expected result for unequal strings (-1)
    assert(result2 == -1);
}

void using_ccstring_manger()
{
    ccstring_manager_t mgr = ccstring_manager_new(2); // start with 2 slots

    ccstring_t* a = ccstring_new("Hello", 50);
    ccstring_t* b = ccstring_new("From", 50);
    ccstring_t* c = ccstring_new("Ccstring", 50);

    ccstring_manager_add(&mgr, a, 20);
    ccstring_manager_add(&mgr, b, 20);
    ccstring_manager_add(&mgr, c, 20);

    for (size_t i = 0; i < mgr.count; i++) {
        printf("String %zu: %s\n", i, ccstring_get(mgr.list[i]));
    }
    ccstring_manager_destroy(&mgr); 
}

int main(int argc, char* argv[])
{
    // Test creating a new ccstring
    creating_new_ccstring();
    // Test creating a new ccstring from a view
    creating_new_ccstring_from_view();
    // Test creating a new ccstring from an empty string
    creating_new_ccstring_from_empty();
    // Test creating a new ccstring from a slice
    creating_new_ccstring_from_slice();
    // Test resizing a ccstring
    resize_string_example();
    // Test copying a ccstring
    copy_string_example();
    // Test appending to a ccstring
    append_string_example();
    // Test comparing two ccstrings
    compare_strings();
    // Test using ccstring manager
    using_ccstring_manger();

    return 0;
}