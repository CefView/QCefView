# Build and Config

## Build

This guide provides detailed instructions on how to build the QCefView project from source.

### Prerequisites

Before you begin, ensure you have the following software installed and configured:

#### 1. CMake

QCefView uses [CMake](https://cmake.org/) to manage the build process.  CMake is a cross-platform, open-source build system generator.

*   **Installation:** Download and install CMake from the [official CMake website](https://cmake.org/download/). The minimum supported version is 3.19.1, but we recommend using the latest version for optimal compatibility and features.

#### 2. Qt Framework

QCefView is built upon the Qt framework, offering support for both Qt 5.x and 6.x.

*   **Installation:** Download and install the Qt framework from the [Qt website](https://www.qt.io/download).
*   **Environment Variable:** After installation, set the `QTDIR` environment variable to point to your Qt installation directory. This allows CMake to locate the necessary Qt files.

    **Examples:**

    *   **Windows:**

        ```bat
        set QTDIR=C:\Qt\6.2.2\msvc2019_64
        ```

    *   **macOS:**

        ```bash
        export QTDIR=/usr/local/Cellar/qt5/5.4.1/clang_64
        ```

    *   **Linux:**

        ```bash
        export QTDIR=/opt/Qt/6.2.2/gcc_64 # Example path, adjust accordingly
        ```

    **Note:** Adjust the paths in the examples above to match your actual Qt installation location.

### Build QCefView

1.  **Clone the Repository:**

    Clone the QCefView repository from GitHub, including its submodules:

    ```bash
    git clone --recursive https://github.com/CefView/QCefView.git
    cd QCefView
    ```

    The `--recursive` flag ensures that the necessary `CefViewCore` submodule is also initialized and cloned. If you forgot to use `--recursive`, you can initialize the submodule manually:

    ```bash
    git submodule init
    git submodule update
    ```

2.  **Generate Build Files:**

    Use the appropriate script for your operating system to generate the build files.

    *   **Windows (Visual Studio):**

        ```bash
        generate-win-x86_64.bat
        ```

    *   **macOS (Xcode):**

        ```bash
        ./generate-mac-x86_64.sh
        ```

    *   **Linux (Unix Makefiles):**

        ```bash
        ./generate-linux-x86_64.sh
        ```

3.  **Build the Project:**

    Use CMake to build the project.

    *   **Windows:**

        ```bash
        cmake --build .build/windows.x86_64 --config Release # Or Debug
        ```

    *   **macOS:**

        ```bash
        cmake --build .build/macos.x86_64 --config Release # Or Debug
        ```

    *   **Linux:**

        ```bash
        cmake --build .build/linux.x86_64
        ```

4.  **Locate the Project File:**

    The generated project files will be located in the `.build` directory, within a subdirectory specific to your operating system and architecture (e.g., `.build/windows.x86_64` or `.build/macos.x86_64`).

    *   **Windows:** You can open the generated Visual Studio solution file (`.sln`) and build the project from within Visual Studio.
    *   **macOS:** You can open the generated Xcode project file (`.xcodeproj`) and build the project from within Xcode.
    *   **Linux:** Qt Creator is the recommended IDE for Linux development. You can import the CMake project into Qt Creator.

### Additional Notes

*   **Build Configurations:**  The CMake build command includes a `--config` option.  You can specify `Release` for an optimized build or `Debug` for a build with debugging symbols.
*   **Qt Creator (Linux):**  When using Qt Creator on Linux, select "Import Existing Project" and choose the top-level `CMakeLists.txt` file.

By following these instructions, you should be able to successfully build the QCefView project on your platform. If you encounter any issues, please consult the project's documentation or community forums for assistance.


## Config

QCefView offers several CMake options to customize your build. These options allow you to tailor the build process to your specific needs and environment.

### CMake Configuration Options

Here's a detailed overview of the available CMake configuration options:

#### `QT_SDK_DIR`

*   **Description:** Specifies the directory containing the Qt SDK.
*   **Values:**  The absolute path to your Qt installation directory (e.g., `C:/Qt/6.2.4/msvc2019_64` on Windows, `/opt/Qt/6.2.4/gcc_64` on Linux).
*   **Default:** CMake attempts to automatically detect the Qt SDK.
*   **Usage:**  Set this option if CMake fails to locate your Qt installation, or if you want to use a specific Qt version.

#### `CEF_SDK_VERSION`

*   **Description:** Specifies the version of the Chromium Embedded Framework (CEF) SDK to use.
*   **Values:**  A string representing the CEF SDK version (e.g., `"112.3.0+gb09c4ca+chromium-112.0.5615.165"`).
*   **Default:**  The version defined in the root `CMakeLists.txt` file.
*   **Usage:**  Set this option if you need to use a specific CEF SDK version that differs from the default.
> **⚠ Find the pre-compiled CEF SDK from [https://cef-builds.spotifycdn.com/index.html](https://cef-builds.spotifycdn.com/index.html) (Powered by Spotify)**

#### `CUSTOM_CEF_SDK_DIR`

*   **Description:** Specifies the directory containing a custom-built CEF SDK.
*   **Values:** The absolute path to your custom CEF SDK directory.
*   **Default:**  Empty.  If not set, the build process will attempt to download a pre-built CEF SDK based on `CEF_SDK_VERSION`.
*   **Usage:**  Set this option if you have built your own CEF SDK and want to use it instead of the pre-built version.  This is useful for advanced users who need to customize CEF.

#### `BUILD_DEMO`

*   **Description:** Enables or disables the building of the QCefView demo application.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `ON`
*   **Usage:** Disable this option if you only need the QCefView library and not the demo application.

#### `BUILD_STATIC`

*   **Description:** Configures QCefView to be built as a static library.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `OFF`
*   **Usage:** Enable this option if you want to link QCefView statically into your application.  Note that this might require additional configuration depending on your project setup.

#### `STATIC_CRT`

*   **Description:**  Links the C runtime library statically.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `OFF`
*   **Usage:**  Enable this option to avoid dependencies on the Visual C++ runtime DLLs.  This is primarily relevant on Windows.  Using this option can increase the size of your executable.

#### `USE_SANDBOX`

*   **Description:** Enables or disables the CEF sandbox.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `ON`
*   **Usage:** Disabling the sandbox increases performance but reduces security.  Only disable this option if you fully understand the security implications.

#### `USE_WIN_DCOMPOSITION`

*   **Description:** Enables or disables the use of Windows DirectComposition for rendering.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `ON` on Windows, `OFF` on other platforms.
*   **Usage:**  DirectComposition can improve rendering performance on Windows.  Disable this option if you encounter compatibility issues.

#### `USE_QT_EVENT_LOOP`

*   **Description:**  Enables the use of Qt's event loop integration for CEF.
*   **Values:** `ON` (enable) or `OFF` (disable).
*   **Default:** `ON`
*   **Usage:**  This option integrates CEF's message loop with Qt's event loop, providing better responsiveness and compatibility.  In rare cases, disabling this option might resolve specific event loop conflicts.

### Setting CMake Options

You can set these CMake options when you generate the build files.  The method depends on your build environment:

*   **CMake GUI:**  Use the CMake GUI to configure the options before generating the project files.
*   **Command Line:**  Use the `-D` flag when running CMake from the command line.  For example:

    ```bash
    cmake -DCEF_SDK_VERSION="106.0.5249.61" -DBUILD_DEMO=OFF ..
    ```

*   **CMakePresets.json:** If you are using CMakePresets.json, you can set the options in the `cacheVariables` section.

By understanding and utilizing these configuration options, you can optimize the QCefView build process for your specific application and platform.

