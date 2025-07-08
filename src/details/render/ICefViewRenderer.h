#ifndef ICEFVIEWRENDERER_H
#define ICEFVIEWRENDERER_H

#pragma once
#include <include/cef_app.h>
#include <include/cef_task.h>

#include <QColor>
#include <QPointer>
#include <QSharedPointer>
#include <QSize>
#include <QWidget>

/// <summary>
/// Interface for the CEF view renderer.
/// </summary>
class ICefViewRenderer : public QObject
{
  Q_OBJECT

protected:
  /// <summary>
  /// Pointer to the widget that the renderer is associated with.
  /// </summary>
  QPointer<QWidget> m_widget;

  /// <summary>
  /// Task for rendering operations.
  /// </summary>
  class RenderTask : public CefTask
  {
    IMPLEMENT_REFCOUNTING(RenderTask);

    /// <summary>
    /// Function that contains the rendering work to be executed.
    /// </summary>
    std::function<void()> work;

  public:
    /// <summary>
    /// Constructor for the rendering task.
    /// </summary>
    RenderTask(std::function<void()> t)
      : work(t)
    {
    }

    /// <summary>
    /// Executes the rendering task.
    /// </summary>
    void Execute() override
    {
      if (work) {
        work();
      }
    }
  };

public:
  /// <summary>
  /// Destructor
  /// </summary>
  virtual ~ICefViewRenderer() {};

  /// <summary>
  /// Checks if the renderer is using hardware acceleration.
  /// </summary>
  /// <returns></returns>
  virtual bool isHardware() = 0;

  /// <summary>
  /// Initializes the renderer with the given widget, width, height, scale, and clear color.
  /// </summary>
  /// <param name="widget"></param>
  /// <param name="width"></param>
  /// <param name="height"></param>
  /// <param name="scale"></param>
  /// <param name="clear"></param>
  /// <returns></returns>
  virtual bool initialize(QWidget* widget, int width, int height, float scale, const QColor& clear) = 0;

  /// <summary>
  /// Uninitializes the renderer, releasing any resources it holds.
  /// </summary>
  virtual void uninitialize() = 0;

  /// <summary>
  /// Resizes the renderer.
  /// </summary>
  /// <param name="width"></param>
  /// <param name="height"></param>
  /// <param name="scale"></param>
  virtual void resize(int width, int height, float scale) = 0;

  /// <summary>
  /// Renders the content.
  /// </summary>
  virtual void render() = 0;

  /// <summary>
  /// Updates the visibility of the popup.
  /// This method is called by CEF from browser main thread when the popup visibility changes.
  /// </summary>
  /// <param name="visible"></param>
  virtual void updatePopupVisibility(bool visible) = 0;

  /// <summary>
  /// Updates the position and size of the popup.
  /// This method is called by CEF from browser main thread when the popup position or size changes.
  /// </summary>
  /// <param name="rect"></param>
  virtual void updatePopupRect(const CefRect& rect) = 0;

  /// <summary>
  /// Gets the scale factor of the widget.
  /// </summary>
  /// <returns></returns>
  qreal widgetScale()
  {
    qreal scale = 1.0f;
    if (m_widget) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
      scale = m_widget->devicePixelRatioF();
#else
      scale = m_widget->devicePixelRatio();
#endif
    }
    return scale;
  }

  /// <summary>
  /// Gets the size of the widget.
  /// </summary>
  /// <returns></returns>
  QSize widgetSize()
  {
    QSize size;
    if (m_widget) {
      size = m_widget->size();
    }
    return size;
  }

  /// <summary>
  /// Gets the background color of the widget.
  /// </summary>
  /// <returns></returns>
  QColor widgetBackground()
  {
    QColor background;
    if (m_widget) {
      background = m_widget->palette().color(m_widget->backgroundRole());
    }
    return background;
  }

  /// <summary>
  /// Frame data types
  /// </summary>
  enum class FrameDataType
  {
    /// <summary>
    /// CPU image data. (Hardware acceleration disabled)
    /// </summary>
    CpuImage = 0,

    /// <summary>
    /// GPU texture data. (Hardware acceleration enabled)
    /// </summary>
    GpuTexture = 1,
  };

  /// <summary>
  /// CEF web content frame data
  /// </summary>
  union FrameData
  {
    /// <summary>
    /// Default constructor for FrameData.
    /// </summary>
    FrameData() {}

    /// <summary>
    /// CPU image data. (Hardware acceleration disabled)
    /// </summary>
    struct
    {
      /// <summary>
      /// Pointer to the image buffer.
      /// </summary>
      const void* buffer = nullptr;

      /// <summary>
      /// Width of the image.
      /// </summary>
      int width = 0;

      /// <summary>
      /// Height of the image.
      /// </summary>
      int height = 0;
    } image;

    /// <summary>
    /// GPU texture data.(Hardware acceleration enabled)
    /// </summary>
    struct
    {
      /// <summary>
      /// Format of the texture.
      /// </summary>
      int format = 0;

      /// <summary>
      /// Handle to the texture.
      /// </summary>
      void* handle = nullptr;

      /// <summary>
      /// Width of the texture.
      /// </summary>
      uint32_t width = 0;

      /// <summary>
      /// Height of the texture.
      /// </summary>
      uint32_t height = 0;

      /// <summary>
      /// Depth of the texture.
      /// </summary>
      uint32_t depth = 0;

      /// <summary>
      /// Size of the texture.
      /// </summary>
      uint64_t size = 0;

      /// <summary>
      /// Modifier of the texture.
      /// </summary>
      uint64_t modifier = 0;
    } texture;
  };

  /// <summary>
  ///
  /// </summary>
  /// <param name="type"></param>
  /// <param name="dirtyRects"></param>
  /// <param name="dataType"></param>
  /// <param name="data"></param>
  virtual void updateFrameData(const CefRenderHandler::PaintElementType& type,
                               const CefRenderHandler::RectList& dirtyRects,
                               const FrameDataType& dataType,
                               const FrameData& data) = 0;
};

/// <summary>
/// Shared pointer type for ICefViewRenderer.
/// </summary>
using CefViewRendererPtr = QSharedPointer<ICefViewRenderer>;

#endif
