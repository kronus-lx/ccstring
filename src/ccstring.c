#include "ccstring.h"

#include <stdlib.h>
#include <string.h>

#define CCSTRING_NULL_TERMINATER '\0'
#define CCSTRING_SUCCESS 0
#define CCSTRING_FAILURE -1

ccstring_t* ccstring_new(const char* str, size_t size)
{
    ccstring_t* new_str = (ccstring_t*)malloc(sizeof(ccstring_t));
    if (!new_str) {
        return NULL; // Memory allocation failed
    }

    new_str->length = size;
    new_str->capacity = size + 1; // +1 for null terminator
    new_str->buffer = (char*)malloc(new_str->capacity);
    if (!new_str->buffer) {
        free(new_str);
        return NULL; // Memory allocation failed
    }

    memcpy(new_str->buffer, str, size);
    new_str->buffer[size] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return new_str;
}

ccstring_t* ccstring_new_from_view(const ccstring_view_t* view)
{
    return ccstring_new(view->buffer, view->length);
}

ccstring_t* ccstring_new_from_slice(const ccstring_slice_t* slice)
{
    return ccstring_new(slice->buffer, slice->length);
}

ccstring_t* ccstring_new_empty(size_t size)
{
    ccstring_t* new_str = (ccstring_t*)malloc(sizeof(ccstring_t));
    if (!new_str) {
        return NULL; // Memory allocation failed
    }

    new_str->length = 0;
    new_str->capacity = size + 1; // +1 for null terminator
    new_str->buffer = (char*)malloc(new_str->capacity);
    if (!new_str->buffer) {
        free(new_str);
        return NULL; // Memory allocation failed
    }

    new_str->buffer[0] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return new_str;
}

char* ccstring_get(const ccstring_t* str)
{
    return str->buffer;
}

size_t ccstring_length(const ccstring_t* str)
{
    return str->length;
}

ccstring_view_t* ccstring_view_new(ccstring_t* str)
{
    ccstring_view_t* view = (ccstring_view_t*)malloc(sizeof(ccstring_view_t));
    if (!view) {
        return NULL; // Memory allocation failed
    }

    view->buffer = str->buffer;
    view->length = str->length;

    return view;
}

ccstring_slice_t* ccstring_slice_new(ccstring_t* str, size_t start, size_t end)
{
    if (!str) {
        return NULL; // Invalid input: str is NULL
    }
    if (start >= str->length || end > str->length || start >= end) {
        return NULL; // Invalid range
    }

    ccstring_slice_t* slice = (ccstring_slice_t*)malloc(sizeof(ccstring_slice_t));
    if (!slice) {
        return NULL; // Memory allocation failed
    }

    slice->buffer = str->buffer + start;
    slice->length = end - start;

    return slice;
}

int ccstring_resize(ccstring_t** str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    ccstring_t* old_str = *str;
    if (new_size > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, new_size + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; // Memory allocation failed
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_size + 1; // +1 for null terminator
    }

    old_str->length = new_size;
    old_str->buffer[new_size] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return CCSTRING_SUCCESS; // Success
}

int ccstring_copy(ccstring_t** str, const char* new_str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    ccstring_t* old_str = *str;
    if (new_size > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, new_size + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; // Memory allocation failed
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_size + 1; // +1 for null terminator
    }

    memcpy(old_str->buffer, new_str, new_size);
    old_str->length = new_size;
    old_str->buffer[new_size] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return CCSTRING_SUCCESS; // Success
}

int ccstring_copy_slice(ccstring_t** str, const ccstring_slice_t* slice)
{
    if (!str || !*str || !slice) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    ccstring_t* old_str = *str;
    if (slice->length > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, slice->length + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; // Memory allocation failed
        }
        old_str->buffer = new_buffer;
        old_str->capacity = slice->length + 1; // +1 for null terminator
    }

    memcpy(old_str->buffer, slice->buffer, slice->length);
    old_str->length = slice->length;
    old_str->buffer[slice->length] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return CCSTRING_SUCCESS; // Success
}

int ccstring_copy_view(ccstring_t** str, const ccstring_view_t* view)
{
    if (!str || !*str || !view) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    ccstring_t* old_str = *str;
    if (view->length > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, view->length + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; // Memory allocation failed
        }
        old_str->buffer = new_buffer;
        old_str->capacity = view->length + 1; // +1 for null terminator
    }

    memcpy(old_str->buffer, view->buffer, view->length);
    old_str->length = view->length;
    old_str->buffer[view->length] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return CCSTRING_SUCCESS; // Success
}

int ccstring_compare(const ccstring_t* str1, const ccstring_t* str2)
{
    if (!str1 || !str2) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    size_t min_length = str1->length < str2->length ? str1->length : str2->length;
    int cmp = memcmp(str1->buffer, str2->buffer, min_length);
    if (cmp != 0) {
        return CCSTRING_FAILURE; // Strings are not equal
    }

    // If they are equal up to the length of the shorter string, compare lengths
    if (str1->length != str2->length) {
        return CCSTRING_FAILURE; // Strings are not equal due to differing lengths
    }
    return CCSTRING_SUCCESS; // Strings are equal
}

int ccstring_append(ccstring_t** str, const char* new_str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    ccstring_t* old_str = *str;
    size_t new_length = old_str->length + new_size;

    if (new_length > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, new_length + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; // Memory allocation failed
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_length + 1; // +1 for null terminator
    }

    memcpy(old_str->buffer + old_str->length, new_str, new_size);
    old_str->length = new_length;
    old_str->buffer[new_length] = CCSTRING_NULL_TERMINATER; // Null-terminate the string

    return CCSTRING_SUCCESS; // Success
}

void ccstring_destroy(ccstring_t** str)
{
    if (str && *str) {
        free((*str)->buffer);
        free(*str);
        *str = NULL; // Set the pointer to NULL
    }
}

void ccstring_slice_destroy(ccstring_slice_t** slice)
{
    if (slice && *slice) {
        free(*slice);
        *slice = NULL; // Set the pointer to NULL
    }
}

void ccstring_view_destroy(ccstring_view_t** view)
{
    if (view && *view) {
        free(*view);
        *view = NULL; // Set the pointer to NULL
    }
}