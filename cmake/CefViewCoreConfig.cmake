include(FetchContent)

# fetch CefViewCore project
FetchContent_Declare(
    CefViewCore
    GIT_REPOSITORY https://github.com/CefView/CefViewCore.git
    GIT_TAG 3d61b7956b14ebd879eb6c5ff7f2ab85d9f780c3
)

# set CEF version to be used
# set(CEF_SDK_VERSION "89.0.12+g2b76680+chromium-89.0.4389.90")     # GOOD
# set(CEF_SDK_VERSION "91.1.23+g04c8d56+chromium-91.0.4472.164")    # GOOD
# set(CEF_SDK_VERSION "95.7.12+g99c4ac0+chromium-95.0.4638.54")     # GOOD
# set(CEF_SDK_VERSION "99.2.15+g71e9523+chromium-99.0.4844.84")     # GOOD
# set(CEF_SDK_VERSION "102.0.10+gf249b2e+chromium-102.0.5005.115")  # GOOD
# set(CEF_SDK_VERSION "104.4.18+g2587cf2+chromium-104.0.5112.81")   # BAD   # crash on macOS - debug mode
# set(CEF_SDK_VERSION "104.4.26+g4180781+chromium-104.0.5112.102")  # BAD   # crash on macOS - debug mode
# set(CEF_SDK_VERSION "105.3.25+g0ca6a9e+chromium-105.0.5195.54")   # BAD   # debugbreak - debug mode
# set(CEF_SDK_VERSION "105.3.28+g002805e+chromium-105.0.5195.54")   # BAD   # debugbreak - debug mode
# set(CEF_SDK_VERSION "107.1.9+g1f0a21a+chromium-107.0.5304.110")   # BAD   # debugbreak - debug mode
# set(CEF_SDK_VERSION "110.0.26+g732747f+chromium-110.0.5481.97")   # BAD   # TO-BE-FIXED
# set(CEF_SDK_VERSION "113.3.1+g525fa10+chromium-113.0.5672.128")   # So far so good - doesn't support Xcode 15
# set(CEF_SDK_VERSION "119.4.7+g55e15c8+chromium-119.0.6045.199")   # GOOD
# set(CEF_SDK_VERSION "120.1.6+gf08b1fd+chromium-120.0.6099.71")    # GOOD
# set(CEF_SDK_VERSION "121.3.15+g4d3b0b4+chromium-121.0.6167.184")  # GOOD
# set(CEF_SDK_VERSION "122.1.13+gde5b724+chromium-122.0.6261.130")  # GOOD
# set(CEF_SDK_VERSION "123.0.13+gfc703fb+chromium-123.0.6312.124")  # NOT-TEST
# set(CEF_SDK_VERSION "124.3.9+g9bd638f+chromium-124.0.6367.207")   # NOT-TEST
# set(CEF_SDK_VERSION "125.0.22+gc410c95+chromium-125.0.6422.142")  # NOT-TEST
# set(CEF_SDK_VERSION "126.2.18+g3647d39+chromium-126.0.6478.183")  # NOT-TEST
set(CEF_SDK_VERSION "127.3.5+g114ea2a+chromium-127.0.6533.120") # GOOD

# set(CEF_SDK_VERSION "128.4.9+g9840ad9+chromium-128.0.6613.120") # BAD   # debugbreak - debug mode

# populate CefViewCore project
FetchContent_MakeAvailable(CefViewCore)

# set sub fodler for all CefViewCore targets
set_target_properties(CefViewCore PROPERTIES FOLDER core)

# set sub-folder for all Helper targets
foreach(CefViewWingTarget ${CefViewCore_HELPER_APP_TARGETS})
    set_target_properties(${CefViewWingTarget} PROPERTIES FOLDER core)
endforeach(CefViewWingTarget)

# set sub-folder for all libcef_dll_wrapper target
set_target_properties(libcef_dll_wrapper PROPERTIES FOLDER core)

# export all CefViewCore variables
if(QCefView_HAS_PARENT_DIRECTORY)
    set(CefViewCore_EXPORT_INCLUDE_PATH ${CefViewCore_EXPORT_INCLUDE_PATH} PARENT_SCOPE)
    set(CefViewCore_HELPER_APP_TARGETS ${CefViewCore_HELPER_APP_TARGETS} PARENT_SCOPE)
    set(CefViewCore_CEF_BINARY_DIR ${CefViewCore_CEF_BINARY_DIR} PARENT_SCOPE)
    set(CefViewCore_CEF_RESOURCE_DIR ${CefViewCore_CEF_RESOURCE_DIR} PARENT_SCOPE)
    set(CefViewCore_CEF_INCLUDE_DIR ${CefViewCore_CEF_INCLUDE_DIR} PARENT_SCOPE)
endif()
