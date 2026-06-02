cmake_minimum_required(VERSION 3.15)

project(ccstring VERSION 1.0.0 LANGUAGES C)

include(GNUInstallDirs)

option(CCSTRING_BUILD_EXAMPLES "Build example executable" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

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

# Create library and optional example executable
add_library(ccstring "src/ccstring.c")

if(CCSTRING_BUILD_EXAMPLES)
    add_executable(ccstring_examples "examples/examples.c")
endif()

# Include directories and compile features
target_include_directories(ccstring PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ccstring>
)

if(CCSTRING_BUILD_EXAMPLES)
    target_include_directories(ccstring_examples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_features(ccstring_examples PRIVATE c_std_11)
    target_compile_options(ccstring_examples PRIVATE
        $<$<C_COMPILER_ID:MSVC>:/W4 /WX /GS /guard:cf>
        $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -fstack-protector-strong>
    )
    target_link_libraries(ccstring_examples PRIVATE ccstring)
endif()

target_compile_features(ccstring PRIVATE c_std_11)
target_compile_options(ccstring PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W4 /WX /GS /guard:cf>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -fstack-protector-strong>
)

# Export/import symbols correctly on Windows and keep the library build definition internal.
target_compile_definitions(ccstring PRIVATE CCSTRING_BUILDING)

install(TARGETS ccstring
    EXPORT ccstringTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(CCSTRING_BUILD_EXAMPLES)
    install(TARGETS ccstring_examples
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ccstring
    FILES_MATCHING PATTERN "*.h"
)
