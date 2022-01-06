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
  if(OS_WINDOWS)
    set(QT_SDK_DIR
      # Change this value to the Qt SDK path of your build environment
      "C:\\Qt\\5.15.2\\msvc2019_64"
    )
  elseif (OS_LINUX)
    set(QT_SDK_DIR
      # Change this value to the Qt SDK path of your build environment
      "$HOME/Qt/6.2"
    )
  elseif(OS_MACOS)
    set(QT_SDK_DIR
      # Change this value to the Qt SDK path of your build environment
      "$HOME/Qt/6.1"
    )
  else()
    message(FATAL_ERROR "Unsupported plaftorm")
  endif()
endif()