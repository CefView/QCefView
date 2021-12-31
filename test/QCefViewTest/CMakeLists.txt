cmake_minimum_required(VERSION 3.4.1)
project(QCefViewTest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

if (OS_WINDOWS)
  find_program(DEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
elseif (OS_MACOS)
  find_program(DEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
elseif (OS_LINUX)
elseif (OS_MACOS)
else()
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/include
)

file(GLOB_RECURSE _SRC_FILES
  "*.h"
  "*.cpp"
)

file(GLOB_RECURSE _UI_FILES
  "*.ui"
)
source_group("Form Files" ${_UI_FILES})

if (OS_WINDOWS)
  file(GLOB_RECURSE _RES_FILES
    "*.qrc"
    "*.rc"
  )
  source_group("Resource Files" ${_RES_FILES})

  add_executable(${PROJECT_NAME} WIN32 
    ${_SRC_FILES}
    ${_UI_FILES}
    ${_RES_FILES}
  )

  target_compile_definitions(${PROJECT_NAME} PRIVATE
    UNICODE
    _UNICODE
  )

  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY 
      VS_STARTUP_PROJECT ${PROJECT_NAME}
  )

  set_property(TARGET ${PROJECT_NAME} 
    PROPERTY
      FOLDER "Test"
      VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
  )

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Embed the manifest file into the target
    COMMAND mt.exe -manifest \"${CMAKE_CURRENT_SOURCE_DIR}\\${PROJECT_NAME}.manifest\" -inputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\" -outputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\"
    # Copy the TEST HTML file
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/QCefViewTestPage.html $<TARGET_FILE_DIR:${PROJECT_NAME}>
    # Deploy the Qt Application
    COMMAND ${DEPLOYQT_EXECUTABLE} 
    --no-svg 
    --no-translations 
    --no-compiler-runtime 
    $<TARGET_FILE:${PROJECT_NAME}>
  )
endif() # OS_WINDOWS

if (OS_LINUX)
  file(GLOB_RECURSE _RES_FILES
    "*.qrc"
  )
  source_group("Resource Files" ${_RES_FILES})

  add_executable(${PROJECT_NAME} 
    ${_SRC_FILES}
    ${_UI_FILES}
    ${_RES_FILES}
  )

  set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
      INSTALL_RPATH "$ORIGIN"
      BUILD_WITH_INSTALL_RPATH TRUE
  )

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Copy the TEST HTML file
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/QCefViewTestPage.html $<TARGET_FILE_DIR:${PROJECT_NAME}>
    # Deploy the Qt Application
    # COMMAND ${DEPLOYQT_EXECUTABLE} 
    # --no-svg 
    # --no-translations 
    # --no-compiler-runtime 
    # $<TARGET_FILE:${PROJECT_NAME}>
  )
endif() # OS_LINUX

if (OS_MACOS)
  file(GLOB_RECURSE _RES_FILES
    "*.qrc"
  )
  source_group("Resource Files" ${_RES_FILES})

  add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
    ${_SRC_FILES}
    ${_UI_FILES}
    ${_RES_FILES}
  )

  set_property(TARGET ${PROJECT_NAME} 
    PROPERTY
      FOLDER "Test"
  )

  add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
    # Copy the TEST HTML file
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/QCefViewTestPage.html" 
    "$<TARGET_BUNDLE_CONTENT_DIR:${PROJECT_NAME}>/Resources/QCefViewTestPage.html"
  )

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Deploy the Qt Application
    COMMAND ${DEPLOYQT_EXECUTABLE} 
      $<TARGET_BUNDLE_DIR:${PROJECT_NAME}> 
      -codesign="-"
  )
endif()

if (OS_LINUX)
  # On linux platform and debug mode, libcef.so MUST be loaded before libc.so.
  # Gnerally we can achive this by using LD_PRELOAD=libcef.so, but this is not 
  # convenience for debugging. 
  # Thus, here we do a trick to make sure the load order
  target_link_libraries(${PROJECT_NAME}
      PRIVATE
        -Wl,--no-as-needed libcef_lib
        QCefView
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
  )
else()
  target_link_libraries(${PROJECT_NAME}
      PRIVATE
        QCefView
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
  )
endif()
