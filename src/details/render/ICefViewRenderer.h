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
///
/// </summary>
class ICefViewRenderer : public QObject
{
  Q_OBJECT

protected:
  QPointer<QWidget> m_widget;

  class RenderTask : public CefTask
  {
    IMPLEMENT_REFCOUNTING(RenderTask);

    std::function<void()> work;

  public:
    RenderTask(std::function<void()> t)
      : work(t)
    {
    }

    void Execute() override
    {
      if (work) {
        work();
      }
    }
  };

public:
  /// <summary>
  ///
  /// </summary>
  virtual ~ICefViewRenderer() {};

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  virtual bool isHardware() = 0;

  /// <summary>
  ///
  /// </summary>
  /// <param name="widget"></param>
  /// <param name="width"></param>
  /// <param name="height"></param>
  /// <param name="scale"></param>
  /// <param name="clear"></param>
  /// <returns></returns>
  virtual bool initialize(QWidget* widget, int width, int height, float scale, const QColor& clear) = 0;

  /// <summary>
  ///
  /// </summary>
  virtual void uninitialize() = 0;

  /// <summary>
  ///
  /// </summary>
  /// <param name="width"></param>
  /// <param name="height"></param>
  /// <param name="scale"></param>
  virtual void resize(int width, int height, float scale) = 0;

  /// <summary>
  ///
  /// </summary>
  /// <param name="visible"></param>
  virtual void updatePopupVisibility(bool visible) = 0;

  /// <summary>
  ///
  /// </summary>
  /// <param name="rect"></param>
  virtual void updatePopupRect(const CefRect& rect) = 0;

  /// <summary>
  ///
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
  ///
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
  ///
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
  ///
  /// </summary>
  enum class FrameDataType
  {
    CpuImage = 0,
    GpuTexture = 1,
  };

  /// <summary>
  /// CEF web content frame data
  /// </summary>
  union FrameData
  {
    FrameData() {}

    /// <summary>
    /// CPU image data. (Hardware acceleration disabled)
    /// </summary>
    struct
    {
      /// <summary>
      ///
      /// </summary>
      const void* buffer = nullptr;

      /// <summary>
      ///
      /// </summary>
      int width = 0;

      /// <summary>
      ///
      /// </summary>
      int height = 0;
    } image;

    /// <summary>
    /// GPU texture data.(Hardware acceleration enabled)
    /// </summary>
    struct
    {
      /// <summary>
      ///
      /// </summary>
      int format = 0;

      /// <summary>
      ///
      /// </summary>
      void* handle = nullptr;

      /// <summary>
      ///
      /// </summary>
      uint32_t width = 0;

      /// <summary>
      ///
      /// </summary>
      uint32_t height = 0;

      /// <summary>
      ///
      /// </summary>
      uint64_t size = 0;

      /// <summary>
      ///
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

  /// <summary>
  ///
  /// </summary>
  virtual void render() = 0;
};

/// <summary>
///
/// </summary>
using CefViewRendererPtr = QSharedPointer<ICefViewRenderer>;

#endif
