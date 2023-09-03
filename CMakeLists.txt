# Project Info
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
cmake_policy(VERSION 3.18)
project(mapp)
enable_language(C)
enable_language(CXX)

# CMake Settings
set(CMAKE_SUPPRESS_REGENERATION true)
set(DCMAKE_GENERATOR_PLATFORM "x64")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options =====================================================
set(XWIN_API AUTO CACHE STRING "A more specific platform selector to choose from, choose the exact OS API to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP.")
set_property(
    CACHE
    XWIN_API PROPERTY
    STRINGS AUTO WIN32 UWP COCOA UIKIT XCB XLIB MIR WAYLAND ANDROID WASM NOOP
)

set(XWIN_OS AUTO CACHE STRING "Optional: Choose the OS to build for, defaults to AUTO, but can be WINDOWS, MACOS, LINUX, ANDROID, IOS, WASM.") 
set_property(
    CACHE
    XWIN_OS PROPERTY
    STRINGS AUTO WINDOWS MACOS LINUX ANDROID IOS WASM NOOP
)

if( NOT (XWIN_OS STREQUAL "AUTO") AND XWIN_API STREQUAL "AUTO")
    if(XWIN_OS STREQUAL "WINDOWS")
        set(XWIN_API "WIN32")
    elseif(XWIN_OS STREQUAL "MACOS")
        set(XWIN_API "COCOA")
    elseif(XWIN_OS STREQUAL "LINUX")
        set(XWIN_API "XLIB")
    elseif(XWIN_OS STREQUAL "ANDROID")
        set(XWIN_API "ANDROID")
    elseif(XWIN_OS STREQUAL "IOS")
        set(XWIN_API "UIKIT")
    elseif(XWIN_OS STREQUAL "WASM")
        set(XWIN_API "WASM")
    elseif(XWIN_OS STREQUAL "NOOP")
        set(XWIN_API "NOOP")
    else()
        message( SEND_ERROR "XWIN_OS can only be either AUTO, NOOP, WINDOWS, MACOS, LINUX, ANDROID, IOS, or WASM.")
    endif()
endif()

if(XWIN_API STREQUAL "AUTO")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(XWIN_API "WIN32" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
    set(XWIN_API "COCOA" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    set(XWIN_API "XCB" CACHE STRING "A more specific platform selector to choose from, choose the exact OS protocol to use, can be WIN32, UWP, COCOA, UIKIT, XCB, XLIB, MIR, WAYLAND, ANDROID, WASM, NOOP." FORCE)
    endif()
endif()

message( STATUS "Building mapp for " ${XWIN_API} )
# =============================================================

# Sources
file(GLOB_RECURSE FILE_SOURCES RELATIVE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/Common/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/common/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapp/common/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapp/main/main.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/${XWIN_API}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/${XWIN_API}/*.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapp/${XWIN_API}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapp/${XWIN_API}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapp/mapp.hpp
)

# Solution Filters
foreach(source IN LISTS FILE_SOURCES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    string(REPLACE "mapp" "" source_path_final "${source_path_msvc}")
    source_group("${source_path_final}" FILES "${source}")
endforeach()

# Main files
file(GLOB_RECURSE MAIN_SOURCES RELATIVE
    ${CMAKE_HOME_DIRECTORY}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/main/${XWIN_API}_main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mapp/main/${XWIN_API}_main.mm
)

set(XMAIN_SOURCES ${MAIN_SOURCES} CACHE STRING "Global Variable - The source files for the currently active protocol.")

# Functions ===================================================
function(xwin_add_executable targetProject targetSources)
    message("Creating mapp executable:")

    foreach(source IN LISTS XMAIN_SOURCES)
        source_group("" FILES "${source}")
    endforeach()
    set(XWIN_FILES "${XMAIN_SOURCES}" "${targetSources}")

    if(XWIN_API STREQUAL "WIN32" OR XWIN_API STREQUAL "UWP")
        add_executable(
            ${targetProject}
            WIN32
            "${XWIN_FILES}"
        )
    elseif(XWIN_API STREQUAL "COCOA" OR XWIN_API STREQUAL "UIKIT")
        add_executable(
            ${targetProject}
            MACOSX_BUNDLE
            ${XWIN_FILES}

        )
    elseif(XWIN_API STREQUAL "XCB" OR XWIN_API STREQUAL "XLIB")
        add_executable(
            ${targetProject}
            ${XWIN_FILES}
        )
    elseif(XWIN_API STREQUAL "ANDROID")
        add_executable(
            ${targetProject}
            ${XWIN_FILES}
        )
    elseif(XWIN_API STREQUAL "WASM")
        add_executable(
            ${targetProject}
            ${XWIN_FILES}
        )
    elseif(XWIN_API STREQUAL "NOOP")
        add_executable(
            ${targetProject}
            ${XWIN_FILES}
        )
    endif()

    target_compile_definitions(${targetProject} PRIVATE XWIN_${XWIN_API}=1)
endfunction()
# =============================================================

# Finalize Library
add_library(
    ${PROJECT_NAME}
    ${FILE_SOURCES}
    )
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Dependencies ================================================
if(XWIN_API STREQUAL "COCOA")
  add_definitions("-x objective-c++")
  find_library(COCOA_LIBRARY Cocoa)
  find_library(QUARTZ_LIBRARY QuartzCore)
  target_link_libraries(
    ${PROJECT_NAME}
    ${COCOA_LIBRARY}
    ${QUARTZ_LIBRARY}
)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"
)
elseif(XWIN_API STREQUAL "UIKIT")
    add_definitions("-x objective-c++")
    find_library(UIKIT UIKit)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(FOUNDATION Foundation)
    find_library(MOBILECORESERVICES MobileCoreServices)
    find_library(CFNETWORK CFNetwork)
    find_library(SYSTEMCONFIGURATION SystemConfiguration)
    
    target_link_libraries(
    ${PROJECT_NAME}
    ${UIKIT}
    ${METALKIT_LIBRARY}
    ${FOUNDATION}
    ${CFNETWORK}
    ${SYSTEMCONFIGURATION}
    )
    set_target_properties(
    ${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "2"
    )
elseif(XWIN_API STREQUAL "XLIB")
    find_package(X11 REQUIRED)
    if(X11_FOUND)
        message("Found X11 Libraries.")
        message("X11 includes = ${X11_INCLUDE_DIR}")
        message("X11 Libraries = ${X11_LIBRARIES}")
        target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PUBLIC ${X11_INCLUDE_DIR})
    endif()
elseif(XWIN_API STREQUAL "XCB")
    find_package(X11 REQUIRED)
    message("Found XCB Libraries.")
    message("XCB Include Path = ${X11_xcb_INCLUDE_PATH}")
    message("XCB Lib = ${X11_xcb_LIB}")
    target_link_libraries(${PROJECT_NAME} ${X11_xcb_LIB})
    target_include_directories(${PROJECT_NAME} PUBLIC ${X11_xcb_INCLUDE_PATH})
elseif(XWIN_API STREQUAL "WIN32")
    target_compile_options(${PROJECT_NAME} PRIVATE /Zc:strictStrings-)
endif()
# =============================================================

# Preprocessor Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC XWIN_${XWIN_API}=1)
