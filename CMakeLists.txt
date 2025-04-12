cmake_minimum_required(VERSION 3.10)

# Project name
project(ccstring VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    message(STATUS "Configuring for Windows")
    add_definitions(-DPLATFORM_WINDOWS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
elseif(UNIX)
    message(STATUS "Configuring for Linux")
    add_definitions(-DPLATFORM_LINUX)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(CCSTRING_MAIN)
set(CCSTRING_EXAMPLES)

target_include_directories(${CCSTRING_MAIN} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${CCSTRING_MAIN} PRIVATE c_std_99)

target_include_directories(${CCSTRING_EXAMPLES} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(${CCSTRING_EXAMPLES} PRIVATE c_std_99)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(examples)

install(TARGETS ${CCSTRING_MAIN} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    INCLUDES DESTINATION include/${PROJECT_NAME}
)

install(TARGETS ${CCSTRING_EXAMPLES} 
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    INCLUDES DESTINATION include/${PROJECT_NAME}
)