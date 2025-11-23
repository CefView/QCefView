# set folder
set(CMAKE_FOLDER "QCefView/Core")

include(FetchContent)

if(NOT CEFVIEW_CORE_VERSION)
    set(DEFAULT_CEFVIEW_CORE_VERSION "afedf01fe440a7ead4d4fe31887fb71eb78345df")
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
# set(CEF_SDK_VERSION "89.0.18+gb36241d+chromium-89.0.4389.114")
# set(CEF_SDK_VERSION "90.6.7+g19ba721+chromium-90.0.4430.212")
# set(CEF_SDK_VERSION "91.1.23+g04c8d56+chromium-91.0.4472.164")
# set(CEF_SDK_VERSION "92.0.27+g274abcf+chromium-92.0.4515.159")
# set(CEF_SDK_VERSION "93.1.14+gf38ce34+chromium-93.0.4577.82")
# set(CEF_SDK_VERSION "94.4.11+gc4d96f0+chromium-94.0.4606.81")
# set(CEF_SDK_VERSION "95.7.18+g0d6005e+chromium-95.0.4638.69")
# set(CEF_SDK_VERSION "96.0.18+gfe551e4+chromium-96.0.4664.110")
# set(CEF_SDK_VERSION "97.1.9+ga00bca5+chromium-97.0.4692.99")
# set(CEF_SDK_VERSION "98.2.1+g29d6e22+chromium-98.0.4758.109")
# set(CEF_SDK_VERSION "99.2.15+g71e9523+chromium-99.0.4844.84")     # BAD:IME poisition incorrect
# set(CEF_SDK_VERSION "100.0.24+g0783cf8+chromium-100.0.4896.127")  # BAD:IME poisition incorrect
# set(CEF_SDK_VERSION "101.0.18+g367b4a0+chromium-101.0.4951.67")   # BAD:IME poisition incorrect
# set(CEF_SDK_VERSION "102.0.10+gf249b2e+chromium-102.0.5005.115")  # BAD:IME poisition incorrect
# set(CEF_SDK_VERSION "103.0.12+g8eb56c7+chromium-103.0.5060.134")  # BAD:IME poisition incorrect
# set(CEF_SDK_VERSION "104.4.26+g4180781+chromium-104.0.5112.102")
# set(CEF_SDK_VERSION "105.3.39+g2ec21f9+chromium-105.0.5195.127")
# set(CEF_SDK_VERSION "106.1.1+g5891c70+chromium-106.0.5249.119")
# set(CEF_SDK_VERSION "107.1.12+g65b79a6+chromium-107.0.5304.122")
# set(CEF_SDK_VERSION "108.4.13+ga98cd4c+chromium-108.0.5359.125")
# set(CEF_SDK_VERSION "109.1.18+gf1c41e4+chromium-109.0.5414.120")
# set(CEF_SDK_VERSION "110.0.32+g291f1df+chromium-110.0.5481.180")
# set(CEF_SDK_VERSION "111.2.7+gebf5d6a+chromium-111.0.5563.148")
# set(CEF_SDK_VERSION "112.3.0+gb09c4ca+chromium-112.0.5615.165")
# set(CEF_SDK_VERSION "113.3.1+g525fa10+chromium-113.0.5672.128")
# set(CEF_SDK_VERSION "114.2.13+g6792e13+chromium-114.0.5735.200")
# set(CEF_SDK_VERSION "115.3.15+g21130e0+chromium-115.0.5790.173")
# set(CEF_SDK_VERSION "116.0.27+gd8c85ac+chromium-116.0.5845.190")
# set(CEF_SDK_VERSION "117.2.5+gda4c36a+chromium-117.0.5938.152")
# set(CEF_SDK_VERSION "118.7.1+g99817d2+chromium-118.0.5993.119")
# set(CEF_SDK_VERSION "119.4.7+g55e15c8+chromium-119.0.6045.199")
# set(CEF_SDK_VERSION "120.1.6+gf08b1fd+chromium-120.0.6099.71")
# set(CEF_SDK_VERSION "121.3.15+g4d3b0b4+chromium-121.0.6167.184")
# set(CEF_SDK_VERSION "122.1.13+gde5b724+chromium-122.0.6261.130")
# set(CEF_SDK_VERSION "123.0.13+gfc703fb+chromium-123.0.6312.124")
# set(CEF_SDK_VERSION "124.3.9+g9bd638f+chromium-124.0.6367.207")
# set(CEF_SDK_VERSION "125.0.22+gc410c95+chromium-125.0.6422.142")
# set(CEF_SDK_VERSION "126.2.18+g3647d39+chromium-126.0.6478.183")
# set(CEF_SDK_VERSION "127.3.5+g114ea2a+chromium-127.0.6533.120")
# set(CEF_SDK_VERSION "128.4.9+g9840ad9+chromium-128.0.6613.120")   # BAD:debugbreak - debug mode
# set(CEF_SDK_VERSION "129.0.12+gf09539f+chromium-129.0.6668.101")
# set(CEF_SDK_VERSION "130.1.16+g5a7e5ed+chromium-130.0.6723.117")
# set(CEF_SDK_VERSION "131.4.1+g437feba+chromium-131.0.6778.265")
# set(CEF_SDK_VERSION "132.3.2+g4997b2f+chromium-132.0.6834.161")
# set(CEF_SDK_VERSION "133.4.8+g99a2ab1+chromium-133.0.6943.142")
# set(CEF_SDK_VERSION "134.3.11+g7c94248+chromium-134.0.6998.178")
# set(CEF_SDK_VERSION "135.0.22+g442c600+chromium-135.0.7049.115")
# set(CEF_SDK_VERSION "135.0.22+g442c600+chromium-135.0.7049.115")
# set(CEF_SDK_VERSION "136.1.6+g1ac1b14+chromium-136.0.7103.114")
# set(CEF_SDK_VERSION "137.0.19+g8a1c4ce+chromium-137.0.7151.121")
# set(CEF_SDK_VERSION "138.0.49+g5ae25f5+chromium-138.0.7204.261")
# set(CEF_SDK_VERSION "139.0.40+g465474a+chromium-139.0.7258.139")
# set(CEF_SDK_VERSION "140.1.20+g49f5d09+chromium-140.0.7339.214")
# set(CEF_SDK_VERSION "141.0.11+g7e73ac4+chromium-141.0.7390.123")
set(CEF_SDK_VERSION "142.0.15+g6dfdb28+chromium-142.0.7444.176")

# populate CefViewCore project
FetchContent_MakeAvailable(CefViewCore)

configure_file(
    "${CefViewCore_SOURCE_DIR}/include/CefVersion.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/CefVersion.h"
    COPYONLY
)