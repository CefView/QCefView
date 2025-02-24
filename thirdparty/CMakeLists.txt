include(FetchContent)

if(NOT CEFVIEW_CORE_VERSION)
    set(DEFAULT_CEFVIEW_CORE_VERSION "ca9f2c009a6a248a13ff0a2e33bbf46e7a044581")
    set(CEFVIEW_CORE_VERSION ${DEFAULT_CEFVIEW_CORE_VERSION})
    message(STATUS "CEFVIEW_CORE_VERSION not set, use default value:${CEFVIEW_CORE_VERSION}")
endif()

# fetch CefViewCore project
FetchContent_Declare(
    CefViewCore
    GIT_REPOSITORY https://github.com/CefView/CefViewCore.git
    GIT_TAG ${CEFVIEW_CORE_VERSION}
)

# set CEF version to be used
# https://cef-builds.spotifycdn.com/index.html
# https://bitbucket.org/chromiumembedded/cef/wiki/BranchesAndBuilding.md
# set(CEF_SDK_VERSION "89.0.18+gb36241d+chromium-89.0.4389.114")    # GOOD
# set(CEF_SDK_VERSION "91.1.23+g04c8d56+chromium-91.0.4472.164")    # GOOD
# set(CEF_SDK_VERSION "92.0.27+g274abcf+chromium-92.0.4515.159")    # GOOD
# set(CEF_SDK_VERSION "93.1.14+gf38ce34+chromium-93.0.4577.82")     # GOOD
# set(CEF_SDK_VERSION "94.4.11+gc4d96f0+chromium-94.0.4606.81")     # GOOD
# set(CEF_SDK_VERSION "95.7.18+g0d6005e+chromium-95.0.4638.69")     # GOOD
# set(CEF_SDK_VERSION "96.0.18+gfe551e4+chromium-96.0.4664.110")    # GOOD
# set(CEF_SDK_VERSION "97.1.9+ga00bca5+chromium-97.0.4692.99")      # GOOD
# set(CEF_SDK_VERSION "98.2.1+g29d6e22+chromium-98.0.4758.109")     # GOOD
# set(CEF_SDK_VERSION "99.2.15+g71e9523+chromium-99.0.4844.84")     # IME poisition incorrect
# set(CEF_SDK_VERSION "100.0.24+g0783cf8+chromium-100.0.4896.127")  # IME poisition incorrect
# set(CEF_SDK_VERSION "101.0.18+g367b4a0+chromium-101.0.4951.67")   # IME poisition incorrect
# set(CEF_SDK_VERSION "102.0.10+gf249b2e+chromium-102.0.5005.115")  # IME poisition incorrect
# set(CEF_SDK_VERSION "103.0.12+g8eb56c7+chromium-103.0.5060.134")  # IME poisition incorrect
# set(CEF_SDK_VERSION "104.4.26+g4180781+chromium-104.0.5112.102")  # GOOD
# set(CEF_SDK_VERSION "105.3.39+g2ec21f9+chromium-105.0.5195.127")  # GOOD
# set(CEF_SDK_VERSION "106.1.1+g5891c70+chromium-106.0.5249.119")   # GOOD
# set(CEF_SDK_VERSION "107.1.12+g65b79a6+chromium-107.0.5304.122")  # GOOD
# set(CEF_SDK_VERSION "108.4.13+ga98cd4c+chromium-108.0.5359.125")  # GOOD
# set(CEF_SDK_VERSION "109.1.18+gf1c41e4+chromium-109.0.5414.120")  # GOOD
# set(CEF_SDK_VERSION "110.0.32+g291f1df+chromium-110.0.5481.180")  # GOOD
# set(CEF_SDK_VERSION "111.2.7+gebf5d6a+chromium-111.0.5563.148")   # GOOD
# set(CEF_SDK_VERSION "112.3.0+gb09c4ca+chromium-112.0.5615.165")   # GOOD
# set(CEF_SDK_VERSION "113.3.1+g525fa10+chromium-113.0.5672.128")   # GOOD
# set(CEF_SDK_VERSION "119.4.7+g55e15c8+chromium-119.0.6045.199")   # GOOD
# set(CEF_SDK_VERSION "120.1.6+gf08b1fd+chromium-120.0.6099.71")    # GOOD
# set(CEF_SDK_VERSION "121.3.15+g4d3b0b4+chromium-121.0.6167.184")  # GOOD
# set(CEF_SDK_VERSION "122.1.13+gde5b724+chromium-122.0.6261.130")  # GOOD
# set(CEF_SDK_VERSION "123.0.13+gfc703fb+chromium-123.0.6312.124")  # NOT TEST
# set(CEF_SDK_VERSION "124.3.9+g9bd638f+chromium-124.0.6367.207")   # NOT TEST
# set(CEF_SDK_VERSION "125.0.22+gc410c95+chromium-125.0.6422.142")  # NOT TEST
set(CEF_SDK_VERSION "126.2.18+g3647d39+chromium-126.0.6478.183") # NOT TEST

# set(CEF_SDK_VERSION "127.3.5+g114ea2a+chromium-127.0.6533.120") # GOOD

# set(CEF_SDK_VERSION "128.4.9+g9840ad9+chromium-128.0.6613.120")   # BAD   # debugbreak - debug mode

# set folder
set(CMAKE_FOLDER "QCefView/Core")

# populate CefViewCore project
FetchContent_MakeAvailable(CefViewCore)

configure_file(
    "${CefViewCore_SOURCE_DIR}/include/CefVersion.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/CefVersion.h"
    COPYONLY
)