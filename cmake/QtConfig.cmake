# ################################################################################
#
# The Qt SDK path
# You can set the environment variable via
# 1. command line
# Windows: SET QTDIR=PATH/TO/QT
# Non-Windows: export QTDIR=PATH/TO/QT
# 2. modifying the value below directly
#

# Qt build toolchain path not set or doesn't exist
# try to read from environment QTDIR
if(NOT EXISTS ${QT_SDK_DIR})
  message(STATUS "QT_SDK_DIR not found, try to read from environment variable: QTDIR")
  set(QT_SDK_DIR "$ENV{QTDIR}" CACHE STRING "QT_SDK_DIR read from environment variable: QTDIR" FORCE)
endif()

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# You must make sure the target platform and architecture
# of QT binaries math the ones of CEF binaries
# Qt linux-x86       +     CEF linux-x86
# Qt linux-x86_64    +     CEF linux-x86_64
# Qt windows-x86     +     CEF windows-x86
# Qt windows-x86_64  +     CEF windows-x86_64
# Qt macos-x86_64    +     CEF macos-x86_64
# Qt macos-arm64     +     CEF macos-arm64
#

# find required components
message(STATUS "Qt SDK dir: " ${QT_SDK_DIR})
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)