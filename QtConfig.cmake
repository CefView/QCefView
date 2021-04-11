#################################################################################
#
# The Qt SDK path
#
set(QT_SDK_DIR
  # Change this value to the Qt SDK path of your build environment
  "$ENV{QTDIR}"
)

#
# For CI system
#
if (DEFINED ENV{APPVEYOR})
  set(QT_SDK_DIR
    # Change this value to the Qt SDK path of your build environment
    "C:\\Qt\\5.15.2\\msvc2019_64"
  )
endif()