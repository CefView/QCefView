# 
# The main config file for QCefView
#
cmake_minimum_required(VERSION 3.4.1)
project(QCefView)

option(BUILD_DEMO       "Build the demo"                      OFF)
option(BUILD_STATIC     "Build QCefView as static library"    OFF)
option(USE_SANDBOX      "Enable CEF Sandbox"                  OFF)
option(STATIC_CRT       "Use MultiThreaded linkage for MSVC"  OFF)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set proejct architecture
# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()

  if(OS_MACOSX)
    # PROJECT_ARCH should be specified on Mac OS X.
    message(WARNING "No PROJECT_ARCH value specified, using ${PROJECT_ARCH}")
  endif()
endif()


# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING=1)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOS 1)
  set(OS_POSIX 1)
  add_definitions(-DOS_MACOS=1 -DOS_POSIX=1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
  add_definitions(-DOS_LINUX=1 -DOS_POSIX=1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
  add_definitions(-DOS_WINDOWS=1)
endif()

if (BUILD_STATIC)
  add_definitions(-DQCEFVIEW_STATIC=1)
endif ()

# Config the QT package
###############################################################
include(QtConfig.cmake)
message(STATUS "Qt SKD dir: " ${QT_SDK_DIR})
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)
###############################################################


# Config the CEF
###############################################################
if (OS_WINDOWS)
  add_link_options(/DEBUG)
  # Disable the sandbox on Windows, because the sandbox.lib is MT which 
  # is conflict with Qt
  set(USE_SANDBOX OFF CACHE BOOL "Disable sandbox on Windows" FORCE)
  add_compile_options("/M$<IF:$<BOOL:${STATIC_CRT}>,T,D>$<$<CONFIG:Debug>:d>")
else()
  add_compile_options(
    "-g"
    "$<$<CONFIG:DEBUG>:-O0>"
    "$<$<CONFIG:RELEASE>:-O3>"
  )
endif()

if (OS_MACOS)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/lib)

# Include CefViewCore
add_subdirectory(CefViewCore)
set_target_properties(CefViewCore PROPERTIES FOLDER core)
set_target_properties(CefViewWing PROPERTIES FOLDER core)
set_target_properties(libcef_dll_wrapper PROPERTIES FOLDER core)

# Config QCefView target
###############################################################
add_subdirectory(src)

# Config the Demo project
###############################################################
if (BUILD_DEMO)
  add_subdirectory(example/QCefViewTest)
endif()
###############################################################
