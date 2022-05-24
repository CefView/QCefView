### QCefView 
################################################################################################
set(CMAKE_GLOBAL_AUTOGEN_TARGET OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB_RECURSE QCefView_INCLUDE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h"
)
source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
  PREFIX Include 
  FILES ${QCefView_INCLUDE_FILES}
)

file(GLOB QCefView_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} 
  PREFIX Source 
  FILES ${QCefView_SRC_FILES}
)

file(GLOB_RECURSE QCefView_details_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/details/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/details/*.cpp"
)
source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR} 
  PREFIX Source 
  FILES ${QCefView_details_SRC_FILES}
)

if (OS_WINDOWS)
  file(GLOB_RECURSE QCefView_Windows_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.cpp"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/win" 
    PREFIX Source 
    FILES ${QCefView_Windows_SRC_FILES}
  )

  if (BUILD_STATIC)
    add_library(QCefView STATIC
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_Windows_SRC_FILES}
    )
  else ()
    add_library(QCefView SHARED
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_Windows_SRC_FILES}
    )
  endif ()

  target_compile_definitions(QCefView PRIVATE
    UNICODE
    _UNICODE
    QCEFVIEW_LIB
  )

  add_custom_command(TARGET QCefView 
  POST_BUILD

  # copy binary files of CefViewCore
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "$<TARGET_FILE_DIR:CefViewWing>"
    "$<TARGET_FILE_DIR:QCefView>"
  )
endif()  # OS_WINDOWS

if (OS_LINUX)
  find_package(Qt${QT_VERSION_MAJOR}Gui ${QT_MIN_VERSION} CONFIG REQUIRED Private)
  
  file(GLOB_RECURSE QCefView_Linux_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/linux/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/linux/*.cpp"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/linux" 
    PREFIX Source 
    FILES ${QCefView_Linux_SRC_FILES}
  )

  if (BUILD_STATIC)
    add_library(QCefView STATIC
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_Linux_SRC_FILES}
    )
  else ()
    add_library(QCefView SHARED
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_Linux_SRC_FILES}
    )
  endif ()

  target_compile_definitions(QCefView 
    PRIVATE
      QCEFVIEW_LIB
  )

  set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
      INSTALL_RPATH "$ORIGIN"
      BUILD_WITH_INSTALL_RPATH TRUE
  )

  target_link_libraries(QCefView 
    PRIVATE
      Qt${QT_VERSION_MAJOR}::GuiPrivate
  )

  add_custom_command(TARGET QCefView 
  POST_BUILD

  # copy binary files of CefViewCore
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "$<TARGET_FILE_DIR:CefViewWing>"
    "$<TARGET_FILE_DIR:QCefView>"
  )
endif()  # OS_LINUX

if (OS_MACOS)
  file(GLOB_RECURSE QCefView_INCLUDE_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    PREFIX Include 
    FILES ${QCefView_INCLUDE_HEADERS}
  )

  file(GLOB_RECURSE QCefView_macOS_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/mac/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/mac/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mac/*.mm"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/mac" 
    PREFIX Source 
    FILES ${QCefView_macOS_SRC_FILES}
  )
  set(QCefView_INFO_PLIST_FILE "${CMAKE_CURRENT_LIST_DIR}/mac/Info.plist")

  if (BUILD_STATIC)
    add_library(QCefView STATIC
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_macOS_SRC_FILES}
      ${QCefView_INFO_PLIST_FILE}
    )
  else ()
    add_library(QCefView SHARED
      ${QCefView_INCLUDE_FILES}
      ${QCefView_SRC_FILES}
      ${QCefView_details_SRC_FILES}
      ${QCefView_macOS_SRC_FILES}
      ${QCefView_INFO_PLIST_FILE}
    )
  endif ()

  set_target_properties(QCefView
    PROPERTIES
      FRAMEWORK TRUE
      COMPILE_FLAGS                                               "-fobjc-arc"
      PUBLIC_HEADER                                               "${QCefView_INCLUDE_HEADERS}"
      MACOSX_FRAMEWORK_INFO_PLIST                                 "${QCefView_INFO_PLIST_FILE}"
      MACOSX_FRAMEWORK_IDENTIFIER                                 "com.cefview.qcefview"
      CLANG_ENABLE_OBJC_ARC                                       "YES"
      XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD                 "gnu++11"     # -std=gnu++11
      XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME                     "NO"          # -fno-objc-link-runtime
      XCODE_ATTRIBUTE_COPY_PHASE_STRIP                            "NO"
      XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING[variant=Release]        "YES"         # -Wl,-dead_strip
      XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD                     "c99"         # -std=c99
  )

  add_custom_command(
    TARGET QCefView
    PRE_BUILD

    #copy the CefViewWing*.app to resource directory
    COMMAND cp -a
    "$<TARGET_BUNDLE_DIR:CefViewWing>/../"
    "$<TARGET_FILE_DIR:QCefView>/Resources/"

    #sign the cef framework
    COMMAND codesign --force --sign - 
    "$<TARGET_BUNDLE_DIR:QCefView>/Resources/Chromium Embedded Framework.framework"
  )
endif()  # OS_MACOS

if (USE_OSR)
  target_compile_definitions(QCefView PRIVATE
    -DCEF_USE_OSR
  )
endif ()

target_include_directories(QCefView PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/../include"
  ${CefViewCore_EXPORT_INCLUDE_PATH}
)

add_dependencies(QCefView
  CefViewCore
)

target_link_libraries(QCefView 
  PRIVATE
    CefViewCore
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# install QCefView files
install(
  TARGETS QCefView
    ARCHIVE DESTINATION "QCefView/lib$<$<CONFIG:Debug>:/Debug>"
    LIBRARY DESTINATION "QCefView/bin$<$<CONFIG:Debug>:/Debug>"
    FRAMEWORK DESTINATION "QCefView/lib$<$<CONFIG:Debug>:/Debug>"
)
if (NOT OS_MACOS)
  install(DIRECTORY "${CMAKE_SOURCE_DIR}/include" DESTINATION "QCefView")
  install(DIRECTORY "$<TARGET_FILE_DIR:QCefView>/" DESTINATION "QCefView/bin$<$<CONFIG:Debug>:/Debug>")
endif()
