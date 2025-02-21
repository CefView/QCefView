#ifndef DX11RENDERBACKEND_H
#define DX11RENDERBACKEND_H

#pragma once

#include <d3d11_2.h>
#include <wrl.h>

#include "details/render/ICefViewRenderer.h"

/// <summary>
///
/// </summary>
class DX11RenderBackend : public ICefViewRenderer
{
private:
  bool m_showPopup = false;
  CefRect m_popupRect;
  CefColor m_backgroundColor = 0;

  HWND m_hWnd = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_viewTexture;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_popupTexture;

public:
  DX11RenderBackend();

  ~DX11RenderBackend();

  bool isHardware() override { return false; }

  void setBackgroundColor(const CefColor& color) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrame(const CefRenderHandler::PaintElementType& type,
                   const CefRenderHandler::RectList& dirtyRects,
                   const void* buffer,
                   const CefSize& size) override;

  void updateTexture(const CefRenderHandler::PaintElementType& type,
                     const CefRenderHandler::RectList& dirtyRects,
                     const void* handle,
                     int format) override;

  void render(void* painter, int width, int height) override;

protected:
  void resize(int width, int height);
};

#endif // DX11RENDERBACKEND_H
