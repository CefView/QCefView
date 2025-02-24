#ifndef ICEFVIEWRENDERER_H
#define ICEFVIEWRENDERER_H

#pragma once
#include <memory>

#include <include/cef_app.h>

using CefColor = cef_color_t;

/// <summary>
///
/// </summary>
class ICefViewRenderer
{
public:
  /// <summary>
  ///
  /// </summary>
  virtual ~ICefViewRenderer(){};

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  virtual bool isHardware() = 0;

  /// <summary>
  ///
  /// </summary>
  /// <param name="wid"></param>
  /// <param name="width"></param>
  /// <param name="height"></param>
  /// <param name="scale"></param>
  /// <param name="background"></param>
  /// <returns></returns>
  virtual bool initialize(void* wid, int width, int height, float scale, const CefColor& background) = 0;

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
  enum class FrameDataType
  {
    CpuImage = 0,
    GpuTexture = 1,
  };

  /// <summary>
  ///
  /// </summary>
  union FrameData
  {
    struct
    {
      const void* buffer;
      int width;
      int height;
    } image;

    struct
    {
      void* handle;
      int format;

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
  /// <param name="painter"></param>
  virtual void render(void* painter) = 0;
};

using CefViewRendererPtr = std::shared_ptr<ICefViewRenderer>;

#endif
