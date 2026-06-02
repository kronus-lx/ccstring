# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(ccstring VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Status Messages
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C compiler flags: ${CMAKE_C_FLAGS}")
message(STATUS "C standard: ${CMAKE_C_STANDARD}")
message(STATUS "C standard required: ${CMAKE_C_STANDARD_REQUIRED}")
message(STATUS "C extensions: ${CMAKE_C_EXTENSIONS}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

# Platform-specific settings
if(WIN32 OR WIN64)
    message(STATUS "Configuring for Windows")
    add_compile_definitions(PLATFORM_WINDOWS _CRT_SECURE_NO_WARNINGS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    add_compile_definitions(PLATFORM_LINUX)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Create library and executable
add_library(ccstring SHARED "src/ccstring.c")
add_executable(ccstring_examples "examples/examples.c")

# Include directories and compile features
target_include_directories(ccstring PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ccstring_examples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(ccstring PRIVATE c_std_11)
target_compile_features(ccstring_examples PRIVATE c_std_11)

# Harden compiler settings for memory safety and platform correctness.
target_compile_options(ccstring PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W4 /WX /GS /guard:cf>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -fstack-protector-strong>
)
target_compile_options(ccstring_examples PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W4 /WX /GS /guard:cf>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -fstack-protector-strong>
)

# Link libraries
target_link_libraries(ccstring_examples PRIVATE ccstring)

# Export/import symbols correctly on Windows and keep the library build definition internal.
target_compile_definitions(ccstring PRIVATE CCSTRING_BUILDING)

# Install rules
install(TARGETS ccstring 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# Install rules
install(TARGETS ccstring_examples 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# Install additional headers (if needed)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)