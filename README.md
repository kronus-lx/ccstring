# ccstring

**Easy String Allocation and Deallocation for C Programmers**

## Overview

The `ccstring` library provides an abstraction for managing dynamically allocated strings in C with additional support for string views and slices. This library is designed to simplify string manipulation and memory management while offering extended features for working with substrings and read-only views of strings.

### Key Features

- **Dynamic String Allocation**: Easily create, copy, resize, and append strings.  
- **String Views**: Read-only access to a portion of a string without duplicating memory.  
- **String Slices**: Create substrings that point to specific ranges within a string buffer.  

---

## Example Usage

### Creating a New ccstring

```c
#include "ccstring.h"
#include <stdio.h>
#include <assert.h>

void creating_new_ccstring() {
    ccstring_t* str = ccstring_new("Hello, World!", 13);
    assert(str != NULL);

    char* buffer = ccstring_get(str);
    size_t length = ccstring_length(str);

    assert(buffer != NULL);
    assert(length == 13);
    assert(strcmp(buffer, "Hello, World!") == 0);

    printf("String: %s, Length: %zu\n", buffer, length);
    ccstring_destroy(&str);
    assert(str == NULL); // Ensure the string is destroyed
}
```
# Library Functions
## String Creation

    ccstring_new(const char* str, size_t size): Create a new string from a given buffer.

    ccstring_new_empty(size_t size): Create an empty string with a specified capacity.

## String Manipulation

    ccstring_resize(ccstring_t** str, size_t new_size): Resize a string buffer.

    ccstring_append(ccstring_t** str, const char* new_str, size_t new_size): Append a buffer to an existing string.

## Views and Slices

    ccstring_view_new(ccstring_t* str): Create a read-only view of a string.

    ccstring_slice_new(ccstring_t* str, size_t start, size_t end): Create a slice (substring) from a specified range.

## Memory Management

    ccstring_destroy(ccstring_t** str): Free the memory allocated for a string.

    ccstring_slice_destroy(ccstring_slice_t** slice): Free the memory allocated for a slice.

## Intended Function of the Library

The ccstring library aims to abstract dynamic string allocation and management in C. By providing features such as views and slices, it allows developers to work with substrings efficiently, without incurring the overhead of duplicating data. This abstraction simplifies the process of managing memory for strings and reduces the likelihood of common issues like buffer overflows and memory leaks.