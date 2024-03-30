# QCefView

## Introduction


QCefView is a Qt based QWidget which encapsulates [CEF](https://bitbucket.org/chromiumembedded/cef). With QCefView you can build your application UI in frontend developing way to boost your project. And what's more you can completely decouple the UI and business logic in your project. The most important thing is to be able to better use CEF in Qt.
### Support platform
| Platform |  Lastest Build Status  | OSR Production Ready  | NCW Production Ready  |
|---|---|---|---|
| Windows-x86_64 | [![Build on Windows](https://github.com/CefView/QCefView/actions/workflows/build-windows-x86_64.yml/badge.svg)](https://github.com/CefView/QCefView/actions/workflows/build-windows-x86_64.yml) | :heavy_check_mark: | :heavy_check_mark: |
| macOS-x86_64 | [![Build on macOS](https://github.com/CefView/QCefView/actions/workflows/build-macos-x86_64.yml/badge.svg)](https://github.com/CefView/QCefView/actions/workflows/build-macos-x86_64.yml)          | :heavy_check_mark: | :heavy_check_mark: |
| Linux-x86_64 | [![Build on Linux](https://github.com/CefView/QCefView/actions/workflows/build-linux-x86_64.yml/badge.svg)](https://github.com/CefView/QCefView/actions/workflows/build-linux-x86_64.yml)         | :heavy_check_mark: | :x: |

+ **OSR** = Offscreen Rendering Mode 

+ **NCW** = Native Child Window Mode
## Quick Start
### Clone this project
```
git clone https://github.com/CefView/QCefView.git
```
The dependency to CefViewCore has been converted to CMake FetchContent, submodule is not needed anymore.
Pleaser refer to: cmake/CefViewCoreConfig.cmake


### Documentation
You can find the latest documentation here:
+ [English Documentation](https://cefview.github.io/QCefView/)
+ [中文文档](https://cefview.github.io/QCefView/zh/)

It can help you quickly understand how to use it.

## Note For Debugging With Xcode

if you want to debug the project with Xcode, you need to take some action to make sure the demo project could load QCefView at correct location.

1. Generate project with cmake
2. Build demo project with xcode, do not run (actually you will encounter errors if you run directly)
3. Go to the **Build Phases** for target QCefViewDemo and perform the actions below
    - remove all items under **Target Dependencies**
    - remove all items under **Link Binary With Libraries**
4. Now you can debug demo project in Xcode without errors

if you re-generated the project, please remember to re-do the instructions above

## About OSR mode

QCefView supports CEF offscreen rendering mode and this feature is enabled by default at runtime, to disable OSR mode you need to set the config item  setWindowlessRenderingEnabled to false
```cpp
  QCefConfig config;

// WindowlessRenderingEnabled is set to true by default, set to false to disable the OSR mode
 config.setWindowlessRenderingEnabled(false)
 ```

QCefView currently uses Qt Software Rasterizer as the backend paint engine for best compatibility, thus the rendering performance is not so good to be applied for all business scenarios. If you want to host WebGL games, video sites or some other Web Apps with high rendering performance, Non OSR mode (Native Child Window mode) is recommended.

## Question
### Why not OpenGL/DirectX/Metal?

QCefView was designed to be a common Qt widget for Desktop application, so the compatibility is the first important requirement. We can switch the backend engine from Software Rasterizer to OpenGL/DirectX/Metal but this will increase the complexity and decrease the compatibility of QCefView. 

For example if we use OpenGL as the backend engine then all the applications with QCefView introduced will be switched to OpenGL backend automatically. I believe not all users want this to happen. 

DirectX/Metal are platform dependent and they are not supported natively by Qt. If we provide QCefView with extra dependencies individually, this makes QCefView so different with other components in Qt Framework.

But if you must use OSR and the rendering performance is vital, you can try to switch the backend with whatever you want.

## Contributing
This project welcomes contributions and suggestions. Make sure to align your code style with the configuration defined by .clang-format file

Making contributions is not a hard thing.

- Solving an issue(maybe just answering a question raised in issues list or gitter)
- Fixing/Issuing a bug
- Improving the documents and even fixing a typo 

are important contributions to QCefView
If you would like to become one of QCefView's maintainers to contribute more (e.g. help merge PR, triage issues), please contact me by email(sheentianshen#gmail.com). I am glad to help you to set the right permission.
