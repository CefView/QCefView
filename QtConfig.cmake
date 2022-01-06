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
      "C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake"
    )
  elseif (OS_LINUX)
    set(QT_SDK_DIR
      # Change this value to the Qt SDK path of your build environment
      "$ENV{HOME}/Qt/6.2/gcc_64/lib/cmake"
    )
  elseif(OS_MACOS)
    set(QT_SDK_DIR
      # Change this value to the Qt SDK path of your build environment
      "$ENV{HOME}/Qt/6.1/clang_64/lib/cmake"
    )
  else()
    message(FATAL_ERROR "Unsupported plaftorm")
  endif()
endif()
