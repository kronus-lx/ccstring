#include "ccstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CCSTRING_NULL_TERMINATER '\0'
#define CCSTRING_SUCCESS 0
#define CCSTRING_FAILURE 1

ccstring_t* ccstring_new(const char* str, size_t size)
{
    ccstring_t* new_str = (ccstring_t*)malloc(sizeof(*new_str));
    if (!new_str) {
        return NULL;
    }

    new_str->length = size;
    new_str->capacity = size + 1;
    new_str->buffer = (char*)malloc(new_str->capacity);
    if (!new_str->buffer) {
        free(new_str);
        return NULL;
    }

    if (str != NULL && size > 0) {
        memcpy(new_str->buffer, str, size);
    }
    new_str->buffer[size] = CCSTRING_NULL_TERMINATER;

    return new_str;
}

ccstring_t* ccstring_auto(const char* str)
{
    if (str == NULL) {
        return NULL;
    }

    ccstring_t* new_str = (ccstring_t*)malloc(sizeof(*new_str));
    if (!new_str) {
        return NULL;
    }

    size_t str_size = strlen(str);
    if (str_size == 0) {
        free(new_str);
        return NULL;
    }

    new_str->length = str_size;
    new_str->capacity = str_size + 1;
    new_str->buffer = (char*)malloc(new_str->capacity);
    if (new_str->buffer == NULL) {
        free(new_str);
        return NULL;
    }

    memcpy(new_str->buffer, str, str_size);
    new_str->buffer[new_str->length] = CCSTRING_NULL_TERMINATER;
    return new_str;
}

ccstring_t* ccstring_new_add_ref(ccstring_manager_t* mgr, const char* str, size_t size)
{
    ccstring_t* new_str = ccstring_new(str, size);
    if (!new_str) {
        return NULL; // Memory allocation failed
    }

    if (mgr != NULL){
        if (ccstring_manager_add(mgr, new_str, size) != 0) {
            ccstring_destroy(&new_str); // Clean up if adding to manager fails
            return NULL; // Memory allocation failed
        }
    }
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
    ccstring_t* new_str = (ccstring_t*)malloc(sizeof(*new_str));
    if (!new_str) {
        return NULL;
    }

    new_str->length = 0;
    new_str->capacity = size + 1;
    new_str->buffer = (char*)malloc(new_str->capacity);
    if (!new_str->buffer) {
        free(new_str);
        return NULL;
    }

    new_str->buffer[0] = CCSTRING_NULL_TERMINATER;

    return new_str;
}

const char* ccstring_get(const ccstring_t* str)
{
    return str->buffer;
}

size_t ccstring_length(const ccstring_t* str)
{
    return str->length;
}

ccstring_view_t* ccstring_view_new(const ccstring_t* str)
{
    ccstring_view_t* view = (ccstring_view_t*)malloc(sizeof(*view));
    if (!view) {
        return NULL;
    }

    if (!str || !str->buffer) {
        free(view);
        return NULL;
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
            return CCSTRING_FAILURE;
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_size + 1;
    }

    old_str->length = new_size;
    old_str->buffer[new_size] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS; // Success
}

int ccstring_copy(ccstring_t** str, const char* new_str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; 
    }

    ccstring_t* old_str = *str;
    if (new_size > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, new_size + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; 
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_size + 1;
    }

    memcpy(old_str->buffer, new_str, new_size);
    old_str->length = new_size;
    old_str->buffer[new_size] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS; 
}

int ccstring_copy_slice(ccstring_t** str, const ccstring_slice_t* slice)
{
    if (!str || !*str || !slice) {
        return CCSTRING_FAILURE;
    }

    ccstring_t* old_str = *str;
    if (slice->length > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, slice->length + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE; 
        }
        old_str->buffer = new_buffer;
        old_str->capacity = slice->length + 1; 
    }

    memcpy(old_str->buffer, slice->buffer, slice->length);
    old_str->length = slice->length;
    old_str->buffer[slice->length] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS;
}

int ccstring_copy_view(ccstring_t** str, const ccstring_view_t* view)
{
    if (!str || !*str || !view) {
        return CCSTRING_FAILURE;
    }

    ccstring_t* old_str = *str;
    if (view->length > old_str->capacity) {
        char* new_buffer = (char*)realloc(old_str->buffer, view->length + 1);
        if (!new_buffer) {
            return CCSTRING_FAILURE;
        }
        old_str->buffer = new_buffer;
        old_str->capacity = view->length + 1;
    }

    memcpy(old_str->buffer, view->buffer, view->length);
    old_str->length = view->length;
    old_str->buffer[view->length] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS; // Success
}

int ccstring_compare(const ccstring_t* str1, const ccstring_t* str2)
{
    if (!str1 || !str2 || !str1->buffer || !str2->buffer) {
        return CCSTRING_FAILURE;
    }

    int cmp = memcmp(str1->buffer, str2->buffer, str1->length < str2->length ? str1->length : str2->length);
    
    if (cmp != 0) {
        return cmp;
    }

    else if (str1->length < str2->length) {
        return 1;
    }

    else if (str1->length > str2->length) {
        return 2;
    }
    
    return CCSTRING_SUCCESS;
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
            return CCSTRING_FAILURE;
        }
        old_str->buffer = new_buffer;
        old_str->capacity = new_length + 1;
    }

    memcpy(old_str->buffer + old_str->length, new_str, new_size);
    old_str->length = new_length;
    old_str->buffer[new_length] = CCSTRING_NULL_TERMINATER;

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
        *view = NULL;
    }
}

ccstring_manager_t ccstring_manager_new(size_t initial_capacity)
{
    ccstring_manager_t mgr = {0};
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }

    mgr.list = (ccstring_t**)malloc(initial_capacity * sizeof(*mgr.list));
    if (mgr.list != NULL) {
        mgr.capacity = initial_capacity;
    }

    return mgr;
}

int ccstring_manager_add(ccstring_manager_t* mgr, ccstring_t* str, size_t max_capacity)
{
    size_t i;

    if (!mgr || !str) {
        return CCSTRING_FAILURE;
    }

    for (i = 0; i < mgr->count; ++i) {
        if (mgr->list[i] == str) {
            return CCSTRING_FAILURE;
        }
    }

    if (mgr->count >= mgr->capacity) {
        size_t new_capacity = mgr->capacity + (max_capacity > 0 ? max_capacity : mgr->capacity);
        ccstring_t** temp = (ccstring_t**)realloc(mgr->list, new_capacity * sizeof(*temp));
        if (!temp) {
            return CCSTRING_FAILURE;
        }
        mgr->list = temp;
        mgr->capacity = new_capacity;
    }

    mgr->list[mgr->count++] = str;
    return CCSTRING_SUCCESS;
}

int ccstring_manager_remove(ccstring_manager_t* mgr, size_t index, ccstring_t** out_str)
{
    if (!mgr || !mgr->list || index >= mgr->count) {
        return CCSTRING_FAILURE;
    }

    if (out_str) {
        *out_str = mgr->list[index];
    }

    for (size_t i = index + 1; i < mgr->count; ++i) {
        mgr->list[i - 1] = mgr->list[i];
    }

    mgr->list[--mgr->count] = NULL;
    return CCSTRING_SUCCESS;
}

void ccstring_manager_destroy(ccstring_manager_t* mgr)
{
    if (!mgr || !mgr->list) return;

    for (size_t i = 0; i < mgr->count; i++) {
        if (mgr->list[i]) {
            ccstring_destroy(&mgr->list[i]);
        }
    }

    free(mgr->list);
    mgr->list = NULL;
    mgr->count = 0;
    mgr->capacity = 0;
}