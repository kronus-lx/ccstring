#include "ccstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define CCSTRING_NULL_TERMINATER '\0'
#define CCSTRING_SUCCESS 0
#define CCSTRING_FAILURE 1

/* -------------------------------------------------------------------------
 * Internals
 *
 * Every buffer holds capacity + 1 bytes: capacity characters plus the null
 * terminator. length <= capacity always holds, so the terminator written at
 * buffer[length] is always inside the allocation.
 * ---------------------------------------------------------------------- */

/**
 * @brief Report whether ptr points inside the string's own buffer.
 * @param str The string that owns the buffer.
 * @param ptr The pointer to test.
 * @return Non-zero when ptr aliases the buffer, which means a reallocation
 *         would leave it dangling.
 *
 * Comparing pointers into unrelated objects is unspecified rather than
 * undefined, and yields the correct answer on every flat address space.
 */
static int ccstring_aliases_buffer(const ccstring_t* str, const char* ptr)
{
    return ptr != NULL && str->buffer != NULL &&
           ptr >= str->buffer && ptr <= str->buffer + str->capacity;
}

/**
 * @brief Make room for required_length characters plus the null terminator.
 * @param str The string to grow. Its buffer is never shrunk.
 * @param required_length The length the buffer has to accommodate.
 * @param aliased_source Optional address of a source pointer. If it points into
 *        the buffer it is rebased onto the new allocation, because reallocating
 *        frees the memory the caller is about to read from.
 * @return 0 on success, non-zero on failure.
 *
 * Growth is geometric, so repeated appends cost amortised O(1) per character
 * instead of reallocating on every call.
 */
static int ccstring_reserve(ccstring_t* str, size_t required_length, const char** aliased_source)
{
    size_t new_capacity;
    size_t alias_offset = 0;
    int has_alias = 0;
    char* new_buffer;

    if (str->buffer != NULL && required_length <= str->capacity) {
        return CCSTRING_SUCCESS; // Already fits
    }

    if (required_length == SIZE_MAX) {
        return CCSTRING_FAILURE; // No room left for the null terminator
    }

    // Doubling stays below SIZE_MAX, so new_capacity + 1 cannot overflow.
    new_capacity = required_length;
    if (str->capacity <= SIZE_MAX / 2 && str->capacity * 2 > new_capacity) {
        new_capacity = str->capacity * 2;
    }

    if (aliased_source != NULL && ccstring_aliases_buffer(str, *aliased_source)) {
        alias_offset = (size_t)(*aliased_source - str->buffer);
        has_alias = 1;
    }

    new_buffer = (char*)realloc(str->buffer, new_capacity + 1);
    if (new_buffer == NULL) {
        return CCSTRING_FAILURE;
    }

    if (has_alias) {
        *aliased_source = new_buffer + alias_offset;
    }

    str->buffer = new_buffer;
    str->capacity = new_capacity;

    return CCSTRING_SUCCESS;
}

/**
 * @brief Overwrite the string from offset onwards and terminate it.
 * @param str The string to write into.
 * @param offset Where the write starts: 0 for a copy, length for an append.
 * @param source The bytes to write. May point into the string's own buffer.
 * @param size The number of bytes to write.
 * @return 0 on success, non-zero on failure.
 */
static int ccstring_write_at(ccstring_t* str, size_t offset, const char* source, size_t size)
{
    size_t new_length;

    if (source == NULL && size > 0) {
        return CCSTRING_FAILURE; // Nothing to read from
    }

    if (size > SIZE_MAX - offset) {
        return CCSTRING_FAILURE; // new_length would overflow
    }

    new_length = offset + size;

    if (ccstring_reserve(str, new_length, &source) != CCSTRING_SUCCESS) {
        return CCSTRING_FAILURE;
    }

    if (size > 0) {
        memmove(str->buffer + offset, source, size); // Source may overlap the target
    }

    str->length = new_length;
    str->buffer[new_length] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS;
}

/**
 * @brief Allocate an empty string sized to hold capacity characters.
 * @param capacity The number of characters the buffer must accommodate.
 * @return A pointer to the new ccstring_t object, or NULL on failure.
 */
static ccstring_t* ccstring_allocate(size_t capacity)
{
    ccstring_t* str = (ccstring_t*)malloc(sizeof(*str));
    if (str == NULL) {
        return NULL;
    }

    str->buffer = NULL;
    str->length = 0;
    str->capacity = 0;

    if (ccstring_reserve(str, capacity, NULL) != CCSTRING_SUCCESS) {
        free(str);
        return NULL;
    }

    str->buffer[0] = CCSTRING_NULL_TERMINATER;

    return str;
}

/* -------------------------------------------------------------------------
 * String creation
 * ---------------------------------------------------------------------- */

ccstring_t* ccstring_new(const char* str, size_t size)
{
    ccstring_t* new_str = ccstring_allocate(size);
    if (new_str == NULL) {
        return NULL;
    }

    if (size > 0) {
        if (str != NULL) {
            memcpy(new_str->buffer, str, size);
        }
        else {
            memset(new_str->buffer, 0, size); // Never expose uninitialised memory
        }

        new_str->length = size;
        new_str->buffer[size] = CCSTRING_NULL_TERMINATER;
    }

    return new_str;
}

ccstring_t* ccstring_auto(const char* str)
{
    if (str == NULL) {
        return NULL;
    }

    return ccstring_new(str, strlen(str));
}

ccstring_t* ccstring_new_add_ref(ccstring_manager_t* mgr, const char* str, size_t size)
{
    ccstring_t* new_str = ccstring_new(str, size);
    if (!new_str) {
        return NULL; // Memory allocation failed
    }

    if (mgr != NULL) {
        if (ccstring_manager_add(mgr, new_str, 0) != CCSTRING_SUCCESS) { // 0 lets the list double
            ccstring_destroy(&new_str); // Clean up if adding to manager fails
            return NULL;
        }
    }

    return new_str;
}

ccstring_t* ccstring_new_from_view(const ccstring_view_t* view)
{
    if (!view) {
        return NULL;
    }

    return ccstring_new(view->buffer, view->length);
}

ccstring_t* ccstring_new_from_slice(const ccstring_slice_t* slice)
{
    if (!slice) {
        return NULL;
    }

    return ccstring_new(slice->buffer, slice->length);
}

ccstring_t* ccstring_new_empty(size_t size)
{
    return ccstring_allocate(size);
}

/* -------------------------------------------------------------------------
 * Accessors, views and slices
 * ---------------------------------------------------------------------- */

const char* ccstring_get(const ccstring_t* str)
{
    if (!str) {
        return NULL;
    }

    return str->buffer;
}

size_t ccstring_length(const ccstring_t* str)
{
    if (!str) {
        return 0;
    }

    return str->length;
}

ccstring_view_t* ccstring_view_new(const ccstring_t* str)
{
    ccstring_view_t* view;

    if (!str || !str->buffer) {
        return NULL;
    }

    view = (ccstring_view_t*)malloc(sizeof(*view));
    if (!view) {
        return NULL;
    }

    view->buffer = str->buffer;
    view->length = str->length;

    return view;
}

ccstring_slice_t* ccstring_slice_new(ccstring_t* str, size_t start, size_t end)
{
    ccstring_slice_t* slice;

    if (!str) {
        return NULL; // Invalid input: str is NULL
    }

    if (start >= str->length || end > str->length || start >= end) {
        return NULL; // Invalid range
    }

    slice = (ccstring_slice_t*)malloc(sizeof(*slice));
    if (!slice) {
        return NULL; // Memory allocation failed
    }

    slice->buffer = str->buffer + start;
    slice->length = end - start;

    return slice;
}

/* -------------------------------------------------------------------------
 * String manipulation
 *
 * Every mutation routes through ccstring_reserve/ccstring_write_at, so the
 * reallocation and aliasing rules live in exactly one place.
 * ---------------------------------------------------------------------- */

int ccstring_resize(ccstring_t** str, size_t new_size)
{
    ccstring_t* target;

    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    target = *str;

    if (ccstring_reserve(target, new_size, NULL) != CCSTRING_SUCCESS) {
        return CCSTRING_FAILURE;
    }

    if (new_size > target->length) {
        // Growing must not expose whatever the allocator happened to hand back.
        memset(target->buffer + target->length, 0, new_size - target->length);
    }

    target->length = new_size;
    target->buffer[new_size] = CCSTRING_NULL_TERMINATER;

    return CCSTRING_SUCCESS;
}

int ccstring_copy(ccstring_t** str, const char* new_str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    return ccstring_write_at(*str, 0, new_str, new_size);
}

int ccstring_copy_slice(ccstring_t** str, const ccstring_slice_t* slice)
{
    if (!str || !*str || !slice) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    return ccstring_write_at(*str, 0, slice->buffer, slice->length);
}

int ccstring_copy_view(ccstring_t** str, const ccstring_view_t* view)
{
    if (!str || !*str || !view) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    return ccstring_write_at(*str, 0, view->buffer, view->length);
}

int ccstring_append(ccstring_t** str, const char* new_str, size_t new_size)
{
    if (!str || !*str) {
        return CCSTRING_FAILURE; // Invalid pointer
    }

    return ccstring_write_at(*str, (*str)->length, new_str, new_size);
}

int ccstring_compare(const ccstring_t* str1, const ccstring_t* str2)
{
    int cmp;

    if (!str1 || !str2 || !str1->buffer || !str2->buffer) {
        return CCSTRING_FAILURE;
    }

    cmp = memcmp(str1->buffer, str2->buffer,
                 str1->length < str2->length ? str1->length : str2->length);

    if (cmp != 0) {
        return cmp;
    }

    if (str1->length < str2->length) {
        return -1;
    }

    if (str1->length > str2->length) {
        return 1;
    }

    return CCSTRING_SUCCESS;
}

/* -------------------------------------------------------------------------
 * Memory management
 * ---------------------------------------------------------------------- */

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

/* -------------------------------------------------------------------------
 * Manager
 * ---------------------------------------------------------------------- */

ccstring_manager_t ccstring_manager_new(size_t initial_capacity)
{
    ccstring_manager_t mgr = {0};

    if (initial_capacity == 0) {
        initial_capacity = 1;
    }

    if (initial_capacity > SIZE_MAX / sizeof(*mgr.list)) {
        return mgr; // Allocation size would overflow; leave the manager empty
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
            return CCSTRING_FAILURE; // Already owned; adding it twice would double free
        }
    }

    if (mgr->count >= mgr->capacity) {
        // Caller-supplied step, else double, else seed a list with no capacity yet.
        size_t growth = max_capacity;
        size_t new_capacity;
        ccstring_t** temp;

        if (growth == 0) {
            growth = (mgr->capacity > 0) ? mgr->capacity : 1;
        }

        if (growth > SIZE_MAX - mgr->capacity) {
            return CCSTRING_FAILURE; // new_capacity would overflow
        }

        new_capacity = mgr->capacity + growth;
        if (new_capacity > SIZE_MAX / sizeof(*mgr->list)) {
            return CCSTRING_FAILURE; // Allocation size would overflow
        }

        temp = (ccstring_t**)realloc(mgr->list, new_capacity * sizeof(*temp));
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
    size_t i;

    // out_str is required: without it the string would be detached from the
    // manager with no handle left to free it.
    if (!mgr || !mgr->list || !out_str || index >= mgr->count) {
        return CCSTRING_FAILURE;
    }

    *out_str = mgr->list[index];

    for (i = index + 1; i < mgr->count; ++i) {
        mgr->list[i - 1] = mgr->list[i];
    }

    mgr->list[--mgr->count] = NULL;

    return CCSTRING_SUCCESS;
}

void ccstring_manager_destroy(ccstring_manager_t* mgr)
{
    if (!mgr) {
        return;
    }

    if (mgr->list) {
        for (size_t i = 0; i < mgr->count; i++) {
            if (mgr->list[i]) {
                ccstring_destroy(&mgr->list[i]);
            }
        }

        free(mgr->list);
    }

    mgr->list = NULL;
    mgr->count = 0;
    mgr->capacity = 0;
}
