/**
 * @file ccstring.h
 * @brief C string manipulation library header file in easy to use containers.
 * @version 1.0.0
 * @date 2025-04-11 
*/

#ifndef CCSTRING_H
#define CCSTRING_H

#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * @brief C string manipulation library header file in easy to use containers.
     * @version 1.0.0
     * @date 2025-04-11 
    */

    #if defined(_WIN32) || defined(_WIN64)
        #define CCSTRING_WINDOWS
    #elif defined(__linux__)
        #define CCSTRING_LINUX
    #elif defined(__APPLE__)
        #define CCSTRING_APPLE
    #elif defined(__unix__)
        #define CCSTRING_UNIX
    #else
        #error "Unknown platform!"
    #endif

    /**
     * Export macro for Windows DLLs
     * This macro is used to export functions from the DLL when building it
    */
    #if defined(_WIN32) || defined(_WIN64)
        #if defined(BUILD_SHARED_LIBS)
            #if defined(CCSTRING_BUILDING)
                #define CCSTRING_API __declspec(dllexport)
            #else
                #define CCSTRING_API __declspec(dllimport)
            #endif
        #else
            #define CCSTRING_API
        #endif
    #else
        #define CCSTRING_API
    #endif

    /**
     * Project Version
    */
    #define CCSTRING_VERSION 1.0
    #define CCSTRING_VERSION_MAJOR 1
    #define CCSTRING_VERSION_MINOR 0
    #define CCSTRING_VERSION_PATCH 0

    #include <stddef.h>

    /**
     * A dynamically allocated, always null-terminated string.
     *
     * buffer holds capacity + 1 bytes and the invariant length <= capacity
     * always holds, so buffer[length] is the terminator and is always inside
     * the allocation. Compare against capacity when asking "does this fit",
     * never against the allocation size.
     */
    typedef struct ccstring {
        char* buffer;       /**< Null-terminated storage of capacity + 1 bytes. */
        size_t length;      /**< Characters in use, excluding the terminator. */
        size_t capacity;    /**< Characters that fit before the buffer has to grow. */
    } ccstring_t;
    
    typedef struct ccstring_view {
        const char* buffer;
        size_t length;
    } ccstring_view_t;
    
    typedef struct ccstring_slice {
        const char* buffer;
        size_t length;
    } ccstring_slice_t;

    typedef struct ccstring_manager {
        ccstring_t** list;
        size_t count;
        size_t capacity; 
    } ccstring_manager_t;

    /**
     * @brief Create a new ccstring_t object from a C string.
     * @param str The C string to copy. If NULL, the buffer is zero-filled instead.
     * @param size The number of bytes to copy (the null terminator is added on top).
     * @return A pointer to the new ccstring_t object, or NULL on failure.
     */
    CCSTRING_API ccstring_t* ccstring_new(const char* str, size_t size);

    /**
     * @brief Create a new empty ccstring_t object that can hold size characters.
     * @param size The number of characters to reserve, excluding the null terminator.
     * @return A pointer to the new ccstring_t object, or NULL on failure.
     */
    CCSTRING_API ccstring_t* ccstring_new_empty(size_t size);

    /**
     * @brief Create a new ccstring_t object from a C string, sizing it with strlen.
     * @param str The null-terminated C string to copy. An empty string yields an empty object.
     * @return A pointer to the new ccstring_t object, or NULL if str is NULL or allocation fails.
     */
    CCSTRING_API ccstring_t* ccstring_auto(const char* str);

    /**
     * @brief Create a new ccstring_t object and hand ownership to a manager.
     * @param mgr The manager that takes ownership. If NULL, the caller owns the result.
     * @param str The C string to copy into the new ccstring_t object.
     * @param size The number of bytes to copy (the null terminator is added on top).
     * @return A pointer to the new ccstring_t object, or NULL on failure.
     * @warning When mgr is non-NULL the returned string is owned by the manager. Do not
     *          call ccstring_destroy on it; use ccstring_manager_remove to take it back,
     *          or ccstring_manager_destroy to free everything the manager holds.
     */
    CCSTRING_API ccstring_t* ccstring_new_add_ref(ccstring_manager_t* mgr, const char* str, size_t size);

    /**
     * @brief Create a new ccstring_t object by copying data from a ccstring_view_t.
     * @param view The ccstring_view_t to copy from.
     * @return A pointer to the new ccstring_t object.
     */
    CCSTRING_API ccstring_t* ccstring_new_from_view(const ccstring_view_t* view);

    /**
     * @brief Create a new ccstring_t object by copying data from a ccstring_slice_t.
     * @param slice The ccstring_slice_t to copy from.
     * @return A pointer to the new ccstring_t object.
     */
    CCSTRING_API ccstring_t* ccstring_new_from_slice(const ccstring_slice_t* slice);

    /**
     * @brief Get the internal C string (null-terminated) from a ccstring_t object.
     * @param str The ccstring_t object.
     * @return A pointer to the internal C string, or NULL if str is NULL.
     */
    CCSTRING_API const char* ccstring_get(const ccstring_t* str);

    /**
     * @brief Get the length of the string (excluding null terminator).
     * @param str The ccstring_t object.
     * @return The length of the string, or 0 if str is NULL.
     */
    CCSTRING_API size_t ccstring_length(const ccstring_t* str);

    /**
     * @brief Create a ccstring_view_t object representing a view into a ccstring_t.
     * @param str The ccstring_t object to view.
     * @return A new ccstring_view_t object pointing to the same data, or NULL on failure.
     * @warning The view does not own its data and does not keep the source alive. Any call
     *          that can reallocate the source buffer (ccstring_resize, ccstring_copy*,
     *          ccstring_append) or destroy it leaves the view dangling.
     */
    CCSTRING_API ccstring_view_t* ccstring_view_new(const ccstring_t* str);

    /**
     * @brief Create a ccstring_slice_t object representing a substring of a ccstring_t.
     * @param str The source ccstring_t object.
     * @param start The starting index (inclusive).
     * @param end The ending index (exclusive).
     * @return A new ccstring_slice_t object, or NULL if the range is invalid.
     * @warning The slice does not own its data and does not keep the source alive. Any call
     *          that can reallocate the source buffer (ccstring_resize, ccstring_copy*,
     *          ccstring_append) or destroy it leaves the slice dangling.
     */
    CCSTRING_API ccstring_slice_t* ccstring_slice_new(ccstring_t* str, size_t start, size_t end);

    /**
     * @brief Resize the internal buffer of a ccstring_t.
     * @param str A pointer to the ccstring_t object pointer to resize.
     * @param new_size The new string length. Growing zero-fills the added bytes.
     * @return 0 on success, non-zero on failure.
     * @note The buffer is never shrunk, so capacity may stay above new_size.
     */
    CCSTRING_API int ccstring_resize(ccstring_t** str, size_t new_size);

    /**
     * @brief Copy a new C string into a ccstring_t object.
     * @param str A pointer to the ccstring_t object pointer to copy into.
     * @param new_str The C string to copy.
     * @param new_size The length of the C string to copy.
     * @return 0 on success, non-zero on failure.
     */
    CCSTRING_API int ccstring_copy(ccstring_t** str, const char* new_str, size_t new_size);

    /**
     * @brief Copy a ccstring_slice_t into a ccstring_t object.
     * @param str A pointer to the ccstring_t object pointer to copy into.
     * @param slice The ccstring_slice_t to copy. May slice the target string itself.
     * @return 0 on success, non-zero on failure.
     */
    CCSTRING_API int ccstring_copy_slice(ccstring_t** str, const ccstring_slice_t* slice);

    /**
     * @brief Copy a ccstring_view_t into a ccstring_t object.
     * @param str A pointer to the ccstring_t object pointer to copy into.
     * @param view The ccstring_view_t to copy.
     * @return 0 on success, non-zero on failure.
     */
    CCSTRING_API int ccstring_copy_view(ccstring_t** str, const ccstring_view_t* view);
    
    /**
     * @brief Compare two ccstring_t objects.
     * @param str1 The first ccstring_t object.
     * @param str2 The second ccstring_t object.
     * @return 0 if equal, negative if str1 < str2, positive if str1 > str2.
     *         Returns a positive value if either argument is invalid, so check your
     *         inputs before relying on the sign.
     */
    CCSTRING_API int ccstring_compare(const ccstring_t* str1, const ccstring_t* str2);

    /**
     * @brief Append a C string to the end of a ccstring_t.
     * @param str A pointer to the ccstring_t object pointer.
     * @param new_str The C string to append.
     * @param new_size The length of the C string to append.
     * @return 0 on success, non-zero on failure.
     * @note new_str may point into str's own buffer. Growth is geometric, so
     *       appending in a loop is amortised O(1) per character.
     */
    CCSTRING_API int ccstring_append(ccstring_t** str, const char* new_str, size_t new_size);

    /**
     * @brief Free the memory used by a ccstring_t object and set the pointer to NULL.
     * @param str A pointer to the ccstring_t object pointer to destroy.
     */
    CCSTRING_API void ccstring_destroy(ccstring_t** str);

    /**
     * @brief Free a ccstring_slice_t object and set the pointer to NULL.
     * @param slice A pointer to the ccstring_slice_t object pointer to destroy.
     */
    CCSTRING_API void ccstring_slice_destroy(ccstring_slice_t** slice);

    /**
     * @brief Free a ccstring_view_t object and set the pointer to NULL.
     * @param view A pointer to the ccstring_view_t object pointer to destroy.
     */
    CCSTRING_API void ccstring_view_destroy(ccstring_view_t** view);

    /**
     * @brief Create a new ccstring_manager_t object.
     * @param initial_capacity The initial capacity of the list.
     * @return A pointer to the new ccstring_manager_t object.
     */
    CCSTRING_API ccstring_manager_t ccstring_manager_new(size_t initial_capacity);

    /**
     * @brief Add a string to the manager and take ownership of it.
     * @param mgr The manager that will own the string.
     * @param str The string to register. It must not already be owned by the manager.
     * @param max_capacity Growth step used when reallocating the internal list.
     *                     Pass 0 to let the list double instead.
     * @return 0 on success, non-zero on failure.
     */
    CCSTRING_API int ccstring_manager_add(ccstring_manager_t* mgr, ccstring_t* str, size_t max_capacity);

    /**
     * @brief Remove and return one string from the manager without freeing it.
     * @param mgr The manager that currently owns the string.
     * @param index The index of the string to detach.
     * @param out_str Output pointer that receives the detached string. Required:
     *                without it the string would be detached with no way to free it.
     * @return 0 on success, non-zero on failure.
     * @note Ownership moves to the caller, who must call ccstring_destroy on it.
     */
    CCSTRING_API int ccstring_manager_remove(ccstring_manager_t* mgr, size_t index, ccstring_t** out_str);

    /**
     * @brief Destroy all strings owned by the manager and release the manager list.
     * @param mgr The manager to destroy.
     */
    CCSTRING_API void ccstring_manager_destroy(ccstring_manager_t* mgr);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CCSTRING_H