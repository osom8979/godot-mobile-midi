cmake_minimum_required (VERSION 3.16.0)

if (DEFINED ANDROID_ABI)
    set (PROJECT_SUFFIX "android-${ANDROID_ABI}")
    set (PLATFORM_SRC gmm-android.c)
    set (PLATFORM_LIB)
    set (BUILD_SHARED_LIBS ON)
elseif (DEFINED IOS)
    find_library (COREMIDI_LIBRARY CoreMIDI)
    if (NOT COREMIDI_LIBRARY)
        message (STATUS "CoreMIDI.framework NOT found!")
    else()
        message (STATUS "CoreMIDI.framework found! ${COREMIDI_LIBRARY}")
    endif()

    set (PROJECT_SUFFIX "ios")
    set (PLATFORM_SRC gmm-ios.c)
    set (PLATFORM_LIB ${COREMIDI_LIBRARY})
    set (BUILD_SHARED_LIBS OFF)
else ()
    set (PROJECT_SUFFIX "linux")
    set (PLATFORM_SRC gmm-linux.c)
    set (PLATFORM_LIB)
    set (BUILD_SHARED_LIBS ON)
endif ()

set (PROJECT_NAME "godot-mobile-midi-${PROJECT_SUFFIX}")
project (${PROJECT_NAME})

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_library (${PROJECT_NAME} main.c gmm.c ${PLATFORM_SRC})

if (PLATFORM_LIB)
    target_link_libraries (${PROJECT_NAME} PRIVATE ${PLATFORM_LIB})
endif ()

target_compile_definitions (${PROJECT_NAME} PRIVATE "GODOT_MOBILE_MIDI")
target_include_directories (${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/godot-headers")
target_compile_options (${PROJECT_NAME} PRIVATE "-fPIC")

set (COMPILE_COMMANDS_NAME cc)
set (COMPILE_COMMANDS_SRC "${CMAKE_BINARY_DIR}/compile_commands.json")
set (COMPILE_COMMANDS_DEST "${CMAKE_SOURCE_DIR}/compile_commands.json")
add_custom_target (${COMPILE_COMMANDS_NAME} DEPENDS ${COMPILE_COMMANDS_DEST})
add_custom_command (
    OUTPUT ${COMPILE_COMMANDS_DEST}
    COMMAND ${CMAKE_COMMAND} -E copy "${COMPILE_COMMANDS_SRC}" "${COMPILE_COMMANDS_DEST}"
    DEPENDS ${PROJECT_NAME} ${COMPILE_COMMANDS_SRC}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

install (TARGETS ${PROJECT_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
