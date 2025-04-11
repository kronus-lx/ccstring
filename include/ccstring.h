/**
 * @file ccstring.h
 * @brief C string manipulation library header file in eay to use containers.
 * @version 1.0.0
 * @date 2025-04-11 
*/

#ifndef __CCSTRING_H__
#define __CCSTRING_H__

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Export macro for Windows DLLs
     * This macro is used to export functions from the DLL when building it
     */
    #ifdef __Win32
        #define CCSTRING_API __declspec(dllexport)
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

    /* Forward Declare ccstring structure*/
    struct ccstring;
    /* Forward Declare ccstring_view structure*/
    struct ccstring_view;
    /* Forward Declare ccstring_slice structure*/
    struct ccstring_slice;

    /**
     * @brief ccstring_t is a type that represents a string in the C string library.
    */
    typedef struct ccstring ccstring_t;
    /**
     * @brief ccstring_view_t is a type that represents a string view in the C string library.
    */
    typedef struct ccstring_view ccstring_view_t;
    /**
     * @brief ccstring_slice_t is a type that represents a string slice in the C string library.
    */
    typedef struct ccstring_slice ccstring_slice_t;
    /**
     * @brief Create a new ccstring_t object from a C string.
     * @param str The C string to copy into the new ccstring_t object.
     * @param buffer_size The size of the internal buffer to allocate (including null terminator).
     * @return A pointer to the new ccstring_t object.
     */
    CCSTRING_API ccstring_t* ccstring_new(const char* str, size_t buffer_size);

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
     * @return A pointer to the internal C string.
     */
    CCSTRING_API char* ccstring_get(const ccstring_t* str);

    /**
     * @brief Get the length of the string (excluding null terminator).
     * @param str The ccstring_t object.
     * @return The length of the string.
     */
    CCSTRING_API size_t ccstring_length(const ccstring_t* str);

    /**
     * @brief Create a ccstring_view_t object representing a view into a ccstring_t.
     * @param str The ccstring_t object to view.
     * @return A new ccstring_view_t object pointing to the same data.
     */
    CCSTRING_API ccstring_view_t* ccstring_view_new(ccstring_t* str);

    /**
     * @brief Create a ccstring_slice_t object representing a substring of a ccstring_t.
     * @param str The source ccstring_t object.
     * @param start The starting index (inclusive).
     * @param end The ending index (exclusive).
     * @return A new ccstring_slice_t object.
     */
    CCSTRING_API ccstring_slice_t* ccstring_slice_new(ccstring_t* str, size_t start, size_t end);

    /**
     * @brief Resize the internal buffer of a ccstring_t.
     * @param str A pointer to the ccstring_t object pointer to resize.
     * @param new_size The new buffer size.
     * @return 0 on success, non-zero on failure.
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
     * @brief Copy a ccstring_view_t into a ccstring_t object.
     * @param str A pointer to the ccstring_t object pointer to copy into.
     * @param view The ccstring_view_t to copy.
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
     */
    CCSTRING_API int ccstring_compare(const ccstring_t* str1, const ccstring_t* str2);

    /**
     * @brief Append a C string to the end of a ccstring_t.
     * @param str A pointer to the ccstring_t object pointer.
     * @param new_str The C string to append.
     * @param new_size The length of the C string to append.
     * @return 0 on success, non-zero on failure.
     */
    CCSTRING_API int ccstring_append(ccstring_t** str, const char* new_str, size_t new_size);

    /**
     * @brief Free the memory used by a ccstring_t object and set the pointer to NULL.
     * @param str A pointer to the ccstring_t object pointer to destroy.
     * @return The number of bytes freed.
     */
    CCSTRING_API size_t ccstring_destroy(ccstring_t** str);

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


#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __CCSTRING_H__