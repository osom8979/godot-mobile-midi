cmake_minimum_required (VERSION 3.3.0)

set (PROJECT_NAME "godot-mobile-midi")
set (PROJECT_SOURCE_FILES src/godot_mobile_midi.c)
set (PROJECT_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set (PROJECT_DEPENDENCIES)
set (PROJECT_DEFINITIONS)
set (PROJECT_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/godot-headers")
set (PROJECT_CFLAGS)
set (PROJECT_LDFLAGS)

## Build type.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (PROJECT_DEBUG 1)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set (PROJECT_RELEASE 1)
elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set (PROJECT_RELWITHDEBINFO 1)
elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set (PROJECT_MINSIZEREL 1)
endif ()

## Check compiler.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set (PROJECT_CLANG 1)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Apple")
    set (PROJECT_CLANG 1)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set (PROJECT_GCC 1)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    if (MSVC)
        set (PROJECT_MSVC_INTEL 1)
    else ()
        set (PROJECT_INTEL 1)
    endif ()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set (PROJECT_MSVC 1)
    if (MSVC_VERSION EQUAL 1400)
        set (PROJECT_MSVC_VERSION 8)
    elseif (MSVC_VERSION EQUAL 1500)
        set (PROJECT_MSVC_VERSION 9)
    elseif (MSVC_VERSION EQUAL 1600)
        set (PROJECT_MSVC_VERSION 10)
    elseif (MSVC_VERSION EQUAL 1700)
        set (PROJECT_MSVC_VERSION 11)
    elseif (MSVC_VERSION EQUAL 1800)
        set (PROJECT_MSVC_VERSION 12)
    elseif (MSVC_VERSION EQUAL 1900)
        set (PROJECT_MSVC_VERSION 14)
    endif()
else ()
    message (FATAL_ERROR "Compiler '${CMAKE_CXX_COMPILER_ID}' not recognized")
endif ()

## Check Platform.
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set (PROJECT_WINDOWS 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set (PROJECT_UNIX 1)
    if (ANDROID)
        set (PROJECT_ANDROID 1)
    else ()
        set(PROJECT_LINUX 1)
    endif ()
elseif (CMAKE_SYSTEM_NAME MATCHES "^k?FreeBSD$")
    set (PROJECT_FREEBSD 1)
elseif (CMAKE_SYSTEM_NAME MATCHES "^OpenBSD$")
    set (PROJECT_OPENBSD 1)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    if (IOS)
        set (PROJECT_IOS 1)
    else ()
        set (PROJECT_MACOSX 1)
    endif ()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    set (PROJECT_ANDROID 1)
elseif (CYGWIN)
    set (PROJECT_CYGWIN 1)
endif ()

project (${PROJECT_NAME})

## Enable output of `compile_commands.json` during generation.
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

## CMake modules to be loaded by the `include()` or `find_package()`
## commands before checking the default modules that come with CMake.
list (APPEND CMAKE_MODULE_PATH ${PROJECT_MODULE_PATH})

## Add a dependency between top-level targets.
add_dependencies (${PROJECT_NAME} ${PROJECT_DEPENDENCIES})

## Add compile definitions to a target.
target_compile_definitions (${PROJECT_NAME} PRIVATE ${PROJECT_DEFINITIONS})

## Add include directories to a target.
target_include_directories (${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE_DIRECTORIES})

## Add compile options to a target.
if ("${CMAKE_VERSION}" VERSION_GREATER "3.3.0")
    target_compile_options (${PROJECT_NAME} PRIVATE $<$<COMPILE_LANGUAGE:C>:${PROJECT_CFLAGS}>)
else ()
    target_compile_options (${PROJECT_NAME} PRIVATE ${PROJECT_CFLAGS})
endif ()

## Specify libraries or flags to use when linking a given target and/or its dependents.
target_link_libraries (${PROJECT_NAME} PRIVATE ${PROJECT_LDFLAGS})

## Add a library to the project using the specified source files.
add_library (${PROJECT_NAME} STATIC ${SOURCE_FILES})
#add_library (${PROJECT_NAME} SHARED ${SOURCE_FILES})
#add_library (${PROJECT_NAME} MODULE ${SOURCE_FILES})

