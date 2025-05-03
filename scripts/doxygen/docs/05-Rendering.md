# Rendering
[TOC]

CEF supports native child window mode and OSR mode, in QCefView both modes are implemented.

## NCW mode

When using NCW mode, CEF handles the creation and management of a separate native window for the browser. QCefView then obtains a handle to this window and embeds it within a Qt widget. This allows the browser content to be displayed as part of the Qt application's user interface.

-   **Advantages:**
    -   Simpler to implement compared to OSR mode.
    -   Generally better performance for basic web content rendering, as the rendering is handled directly by CEF within its own window.
    -   Leverages the native windowing system for event handling and input management.

-   **Disadvantages:**
    -   Less flexibility in terms of customizing the rendering process.
    -   Potential limitations in scenarios requiring advanced integration with other graphics APIs or custom rendering pipelines.
    -   Can be problematic when trying to overlay Qt widgets on top of the browser window due to the z-ordering of native windows.

-   **Integration with Qt:** QCefView uses platform-specific APIs to obtain the native window handle from CEF and embed it within a QWidget. This allows the browser content to be seamlessly integrated into the Qt application's layout and user interface.

## OSR mode

When using OSR mode, CEF renders the web page content into a buffer (either in CPU memory or as a shared texture in GPU memory). QCefView then takes this buffer and displays it within a Qt widget. This gives you more control over how the web content is rendered and integrated into your application.

-   **Advantages:**
    -   Greater flexibility in customizing the rendering process.
    -   Enables advanced features such as custom rendering, compositing, and integration with other graphics APIs.
    -   Allows for more seamless integration of web content with Qt widgets, as you can directly manipulate the rendered output.

-   **Disadvantages:**
    -   More complex to implement compared to NCW mode.
    -   Can be more CPU-intensive, especially if hardware acceleration is not enabled or not functioning correctly.
    -   Requires careful management of memory and rendering resources.

-   **Integration with Qt:** QCefView uses Qt's rendering APIs (such as QPainter or QOpenGLWidget) to display the buffer provided by CEF. This allows you to integrate the web content seamlessly into your Qt application's user interface and apply custom transformations, effects, and overlays.

## Setting OSR Mode in QCefView

To enable OSR mode in QCefView, you need to configure it using `QCefConfig`

```cpp
QCefConfig config;
config.setWindowlessRenderingEnabled(true); // Enable OSR mode
```

By setting `config.setWindowlessRenderingEnabled(true)`, you instruct QCefView to use OSR (Off-Screen Rendering) mode. This configuration must be set before the QCefView instance is created.

> [!note]
> QCefConfig works in the application scope, so you can only choose to use NCW or OSR exclusively for all QCefView instances. This means that once you set `QCefConfig::setWindowlessRenderingEnabled` to `true` or `false`, all QCefView instances in your application will use the same rendering mode.


## Hardware Acceleration

With OSR mode enabled, you can choose to use hardware acceleration.

When hardware acceleration is enabled, CEF renders the browser content to a shared texture in the GPU. QCefView then uses device-dependent technologies (DirectX, Metal, or OpenGL) to render the browser content to the user. This typically results in significantly better performance, especially for complex web content and animations.

When hardware acceleration is disabled, CEF provides a CPU memory buffer containing image color data to QCefView. QCefView then renders this buffer using Qt's software rasterizer. This mode can be useful in environments where hardware acceleration is not available or is causing issues.

To control hardware acceleration, you can use the following settings in `QCefSetting`:

```cpp
  QCefSetting setting;
  setting.setBackgroundColor(Qt::gray);
  setting.setWindowlessFrameRate(1000);

  // enable hardware acceleration
  setting.setHardwareAcceleration(true);
```

By default, CEF attempts to use hardware acceleration if it is available. However, explicitly setting the `setHardwareAcceleration` provides more control.

> [!tip]
> Enabling hardware acceleration is generally recommended for the best performance. However, if you encounter rendering issues or compatibility problems, disabling it can provide a fallback option. And this feature only works in OSR mode.