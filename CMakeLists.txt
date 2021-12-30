# 
# The main config file for QCefView
#
cmake_minimum_required(VERSION 3.4.1)
project(QCefView)

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
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

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

# Config the QT package
###############################################################
include(QtConfig.cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})
###############################################################

# Config the CEF
###############################################################
# Disable the sandbox
if ((NOT DEFINED USE_SANDBOX) OR (USE_SANDBOX STREQUAL "") 
    OR (${USE_SANDBOX} MATCHES "(FALSE|false|0|OFF)"))
    option(USE_SANDBOX "Enable CEF Sandbox" OFF)
elseif(${USE_SANDBOX} MATCHES "(TRUE|true|1|null|ON)")
    option(USE_SANDBOX "Enable CEF Sandbox" ON)
else()
    message(FATAL_ERROR "++++++++++ INVALID FLAG USE_SANDBOX=" ${USE_SANDBOX})
endif()
###############################################################

if (OS_WINDOWS)
  add_link_options(/DEBUG)
else()
  add_compile_options(-g)
  add_compile_options(
      "$<$<CONFIG:DEBUG>:-O0>"
      "$<$<CONFIG:RELEASE>:-O3>"
  )
endif()

if (OS_MACOS)
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)
endif()

set(CMAKE_SUPPRESS_REGENERATION TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/$<CONFIG>/lib)

# Include CefViewCore
add_subdirectory(CefViewCore)

# Config QCefView target
###############################################################
add_subdirectory(src)

# Config the Demo project
###############################################################
if ((NOT DEFINED BUILD_DEMO) OR (BUILD_DEMO STREQUAL "") 
    OR (${BUILD_DEMO} MATCHES "(FALSE|false|0|OFF)"))
    option(BUILD_DEMO "Build the demo" OFF)
elseif(${BUILD_DEMO} MATCHES "(TRUE|true|1|null|ON)")
    option(BUILD_DEMO "Build the demo" ON)
else()
    message(FATAL_ERROR "++++++++++ INVALID FLAG BUILD_DEMO=" ${BUILD_DEMO})
endif()
if (BUILD_DEMO)
  add_subdirectory(test/QCefViewTest)
endif()
###############################################################
