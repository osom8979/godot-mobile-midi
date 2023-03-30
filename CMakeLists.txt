cmake_minimum_required (VERSION 3.16.0)

if (DEFINED ANDROID_ABI)
    set (PROJECT_NAME "godot-mobile-midi-android-${ANDROID_ABI}")
else ()
    set (PROJECT_NAME "godot-mobile-midi-linux")
endif ()

project (${PROJECT_NAME})

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_library (${PROJECT_NAME} SHARED main.c)
target_compile_definitions (${PROJECT_NAME} PRIVATE "GODOT_MOBILE_MIDI")
target_include_directories (${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/godot-headers")
target_compile_options (${PROJECT_NAME} PRIVATE "-fPIC")

# target_link_libraries (${PROJECT_NAME} PRIVATE "-rdynamic")

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
