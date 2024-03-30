include(FetchContent)

FetchContent_Declare(
    CefViewCore
    GIT_REPOSITORY https://github.com/CefView/CefViewCore.git
    GIT_TAG c79fb7f5fe1e697dca3a501570daf30e794ceaeb
)

FetchContent_MakeAvailable(CefViewCore)

set_target_properties(CefViewCore PROPERTIES FOLDER core)
