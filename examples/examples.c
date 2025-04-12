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
}

void creating_new_ccstring_from_slice()
{
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    ccstring_slice_t* slice = ccstring_slice_new(str, 0, 5);
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
    

    return 0;
}