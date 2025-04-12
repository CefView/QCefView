# ## QCefView
# ###############################################################################################
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

if(USE_SANDBOX AND(OS_WINDOWS OR OS_MACOS))
  add_definitions(-DCEF_USE_SANDBOX)
endif()

if(USE_QT_EVENT_LOOP)
  add_definitions(-DCEF_USE_QT_EVENT_LOOP)
endif()

if(OS_WINDOWS)
  file(GLOB_RECURSE QCefView_Windows_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/win/*.cpp"
  )
  source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/win"
    PREFIX Source
    FILES ${QCefView_Windows_SRC_FILES}
  )

  add_library(QCefView ${QCEFVIEW_LIB_TYPE}
    ${QCefView_INCLUDE_FILES}
    ${QCefView_SRC_FILES}
    ${QCefView_details_SRC_FILES}
    ${QCefView_Windows_SRC_FILES}
  )

  target_compile_definitions(QCefView PRIVATE
    UNICODE
    _UNICODE
    QCEFVIEW_LIB
  )

  if(BUILD_STATIC)
    target_link_options(QCefView 
      PUBLIC
      "/DELAYLOAD:libcef.dll"
    )

    target_link_libraries(QCefView
      PUBLIC
      d3d11
      d3dcompiler
      dcomp
      delayimp
    )
  else()
    target_link_options(QCefView 
      PRIVATE
      "/DELAYLOAD:libcef.dll"
    )

    target_link_libraries(QCefView
      PRIVATE
      d3d11
      d3dcompiler
      dcomp
      delayimp
    )
  endif()

  add_custom_command(TARGET QCefView
    PRE_BUILD

    # copy binary files of CefViewCore
    COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "$<TARGET_FILE_DIR:${CEFVIEW_WING_NAME}>"
    "$<TARGET_FILE_DIR:QCefView>/CefView"

    # gnerate a file contains timestamp
    COMMAND ${CMAKE_COMMAND}
    -E echo "/* Auto Build Trigger */" > "${CMAKE_BINARY_DIR}/auto_rebuild.cpp"
  )
endif() # OS_WINDOWS

if(OS_LINUX)
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

  add_library(QCefView ${QCEFVIEW_LIB_TYPE}
    ${QCefView_INCLUDE_FILES}
    ${QCefView_SRC_FILES}
    ${QCefView_details_SRC_FILES}
    ${QCefView_Linux_SRC_FILES}
  )

  target_compile_definitions(QCefView
    PRIVATE
    QCEFVIEW_LIB
  )

  set_target_properties(${PROJECT_NAME}
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH TRUE
  )

  add_custom_command(TARGET QCefView
    PRE_BUILD

    # copy binary files of CefViewCore
    COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "$<TARGET_FILE_DIR:${CEFVIEW_WING_NAME}>"
    "$<TARGET_FILE_DIR:QCefView>"
  )
endif() # OS_LINUX

if(OS_MACOS)
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

  add_library(QCefView ${QCEFVIEW_LIB_TYPE}
    ${QCefView_INCLUDE_FILES}
    ${QCefView_SRC_FILES}
    ${QCefView_details_SRC_FILES}
    ${QCefView_macOS_SRC_FILES}
    ${QCefView_INFO_PLIST_FILE}
  )

  find_library(METAL_FRAMEWORK Metal)
  find_library(QUARTZCORE_FRAMEWORK QuartzCore)
  if(BUILD_STATIC)
    target_link_libraries(QCefView
      PUBLIC
      ${METAL_FRAMEWORK}
      ${QUARTZCORE_FRAMEWORK}
    )
  else()
    target_link_libraries(QCefView
      PRIVATE
      ${METAL_FRAMEWORK}
      ${QUARTZCORE_FRAMEWORK}
    )
  endif()


  set_target_properties(QCefView
    PROPERTIES
    FRAMEWORK TRUE

    # compiler settings
    COMPILE_FLAGS "-fobjc-arc"
    CLANG_ENABLE_OBJC_ARC "YES"

    # xcode settings
    PUBLIC_HEADER "${QCefView_INCLUDE_HEADERS}"
    MACOSX_FRAMEWORK_INFO_PLIST "${QCefView_INFO_PLIST_FILE}"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.cefview.qcefview"
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "gnu++11" # -std=gnu++11
    XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME "NO" # -fno-objc-link-runtime
    XCODE_ATTRIBUTE_COPY_PHASE_STRIP "NO"
    XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING[variant=Release] "YES" # -Wl,-dead_strip
    XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD "c99" # -std=c99
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Frameworks @loader_path/Frameworks"
  )

  add_custom_command(TARGET QCefView
    POST_BUILD

    # create plugins directory
    COMMAND mkdir -p
    "$<TARGET_BUNDLE_DIR:QCefView>/Resources/CefView/"

    # copy the CefViewCore binaries to resource directory
    COMMAND cp -a
    "$<TARGET_BUNDLE_DIR:CefViewWing>/../"
    "$<TARGET_BUNDLE_DIR:QCefView>/Resources/CefView/"

    # adjust file permission
    COMMAND
    cd "$<TARGET_BUNDLE_DIR:QCefView>/Resources/CefView/Chromium Embedded Framework.framework/Resources" && chmod +rw *.bin

    # sign the cef framework
    COMMAND codesign
    --force
    --sign -
    "$<TARGET_BUNDLE_DIR:QCefView>/Resources/CefView/Chromium Embedded Framework.framework"
  )
endif() # OS_MACOS

target_compile_definitions(QCefView PRIVATE NOMINMAX)

target_include_directories(QCefView
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/../include"
  PRIVATE
  ${CefViewCore_EXPORT_INCLUDE_PATH}
)

add_dependencies(QCefView
  CefViewCore::CefViewCore
)

if(BUILD_STATIC)
  target_link_libraries(QCefView
    PUBLIC
    CefViewCore::CefViewCore
  )
else()
  target_link_libraries(QCefView
    PRIVATE
    CefViewCore::CefViewCore
  )
endif()

target_link_libraries(QCefView
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

if(NOT OS_MACOS)
  install(DIRECTORY "${CMAKE_SOURCE_DIR}/include" DESTINATION "QCefView")
  install(DIRECTORY "$<TARGET_FILE_DIR:QCefView>/" DESTINATION "QCefView/bin$<$<CONFIG:Debug>:/Debug>")
endif()
