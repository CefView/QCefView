#################################################################################
#
# The Qt SDK path
# You can set the evnironment varibale via
# 1. command line
#   Windows: SET QTDIR=PATH/TO/QT
#   Non-Windows: export QTDIR=PATH/TO/QT
# 2. modifying the value below directly 
# 
set(QT_SDK_DIR
  # Change this value to the Qt SDK path of your build environment
  "$ENV{QTDIR}"
)

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!NOTE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# You must make sure the target platform and architecture 
# of QT binaries math the ones of CEF binaries
#  Qt linux-x86       +     CEF linux-x86 
#  Qt linux-x86_64    +     CEF linux-x86_64 
#  Qt windows-x86     +     CEF windows-x86 
#  Qt windows-x86_64  +     CEF windows-x86_64
#  Qt macos-x86_64    +     CEF macos-x86_64
#  Qt macos-arm64     +     CEF macos-arm64
# 