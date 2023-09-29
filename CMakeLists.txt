
cmake_minimum_required(VERSION 3.14)


if(NOT DEFINED SQUINTYMONGREL_VERSION)
    set(SQUINTYMONGREL_VERSION "v0.0.0-alpha")
endif()

if(NOT DEFINED SQUINTYMONGREL_GIT_HASH)
    set(SQUINTYMONGREL_GIT_HASH "zyxwvutsrqponmlkjihgfedcba")
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX  ${CMAKE_CURRENT_SOURCE_DIR}/install)
endif(NOT DEFINED CMAKE_INSTALL_PREFIX)

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("SQUINTYMONGREL_VERSION: ${SQUINTYMONGREL_VERSION}")
message("SQUINTYMONGREL_GIT_HASH: ${SQUINTYMONGREL_GIT_HASH}")


project(squintymongrel LANGUAGES CXX)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets WebEngineWidgets REQUIRED)


# download the monaco editor tgz, extract it, and rename the dir from 'package' to 'monaco-editor'
if(NOT EXISTS ${CMAKE_SOURCE_DIR}/resources/monaco-editor-0.30.0.tgz)
file(DOWNLOAD
    https://registry.npmjs.org/monaco-editor/-/monaco-editor-0.30.0.tgz
    ${CMAKE_SOURCE_DIR}/resources/monaco-editor-0.30.0.tgz
)
endif()

if(NOT EXISTS ${CMAKE_SOURCE_DIR}/resources/monaco-editor/)
execute_process(
    COMMAND_ECHO
        STDOUT
    COMMAND 
        tar -xf ${CMAKE_SOURCE_DIR}/resources/monaco-editor-0.30.0.tgz
    RESULT_VARIABLE result
    WORKING_DIRECTORY
        ${CMAKE_SOURCE_DIR}/resources
)
if(result AND NOT result EQUAL 0)
    message( FATAL_ERROR "monaco editor extraction failed!")
endif()

file(RENAME ${CMAKE_SOURCE_DIR}/resources/package ${CMAKE_SOURCE_DIR}/resources/monaco-editor)
endif()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
# logging level for debug
set(CMAKE_MESSAGE_LOG_LEVEL "WARNING")
# silent installation
set(CMAKE_INSTALL_MESSAGE NEVER)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS
#     "${CMAKE_CXX_FLAGS} ${CUSTOM_CXX_WARNING_FLAGS}")


set(SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/window.cpp
    ${PROJECT_SOURCE_DIR}/src/window.h
)

set(QRC_FILES
    ${PROJECT_SOURCE_DIR}/squintymongrel.qrc
)


# define the 'executable' target
add_executable(squintymongrel
    ${SOURCE_FILES}
    ${QRC_FILES}
)

# can set a custom output_name for the binary file produced
set_target_properties(squintymongrel PROPERTIES OUTPUT_NAME squintymongrel)

# include dirs for target
target_include_directories(squintymongrel
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src)

# libs for target
target_link_libraries(squintymongrel PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
)

# compile time definitions for target
# to test how it would work when compiled on a platform with no tray icon support
#target_compile_definitions(squintymongrel PUBLIC QT_NO_SYSTEMTRAYICON=1)

# test passing in configuration define via CMake
#target_compile_definitions(squintymongrel PUBLIC SQUINTYMONGREL_USE_SPLASH_SCREEN=0)



# debug:
# message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
# message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
# message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)


install(TARGETS squintymongrel
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(
    FILES
        resources/squintymongrel.desktop
    DESTINATION
        share/applications
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install(
    FILES
        resources/images/squintymongrel.svg
    DESTINATION
        share/icons/hicolor/256x256/apps
    RENAME
        squintymongrel.png
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)

install(
    DIRECTORY
        resources/monaco-editor
    DESTINATION
        share/squintymongrel/
)

install(
    FILES
        resources/monaco_example.html
    DESTINATION
        share/squintymongrel/
    PERMISSIONS OWNER_WRITE OWNER_READ GROUP_READ WORLD_READ
)
