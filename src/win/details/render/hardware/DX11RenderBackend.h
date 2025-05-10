#ifndef DX11RENDERBACKEND_H
#define DX11RENDERBACKEND_H

#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3d11_2.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <wrl.h>

#if defined(ENABLE_WINDOWS_DIRECT_COMPOSITION)
#if _WIN32_WINNT >= 0x602
#define WINDOWS_DIRECT_COMPOSITION 1
#include <dcomp.h>
#else
#error Windows Direct Composition needs _WIN32_WINNT >= 0x602 (Windows 8)
#endif
#endif

#include "details/render/ICefViewRenderer.h"

#ifndef WINDOWS_DIRECT_COMPOSITION
#define WINDOWS_DIRECT_COMPOSITION 0
#endif

/// <summary>
///
/// </summary>
class DX11RenderBackend : public ICefViewRenderer
{
  Q_OBJECT

private:
  bool m_showPopup = false;
  CefRect m_popupRect;

  // device/context
  Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
  Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice1;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;

#if WINDOWS_DIRECT_COMPOSITION
  Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
  Microsoft::WRL::ComPtr<IDCompositionDevice> m_dcompositionDevice;
  Microsoft::WRL::ComPtr<IDCompositionTarget> m_dcompositionTarget;
#else
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
#endif

  // IA stage
  Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

  // VS stage
  Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

  // PS stage
  Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
  Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

  // OM stage
  Microsoft::WRL::ComPtr<ID3D11BlendState> m_blenderState;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

  // view texture and SRV
  D3D11_TEXTURE2D_DESC m_cefViewTextureDesc = {};
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_cefViewVertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cefViewTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cefViewShaderResourceView;

  // popup texture and SRV
  D3D11_TEXTURE2D_DESC m_cefPopupTextureDesc = {};
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_cefPopupVertexBuffer;
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_cefPopupTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_cefPopupShaderResourceView;

protected:
  bool CreateDeviceAndSwapchain(HWND hWnd, int width, int height);
  bool CreateShaderResource();
  bool CreateSampler();
  bool CreateBlender();
  bool CreateRenderTargetView();
  void SetupPipeline(int width, int height);
  void ResizeResource(int width, int height);

  void HandleDeviceLost();

  bool CreateQuadVertexBuffer(float x,                //
                              float y,                //
                              float w,                //
                              float h,                //
                              int viewWidth,          //
                              int viewHeight,         //
                              ID3D11Buffer** ppBuffer //
  );

public:
  DX11RenderBackend();

  ~DX11RenderBackend();

  bool isHardware() override { return true; }

  bool initialize(QWidget* widget, int width, int height, float scale, const QColor& clear) override;

  void uninitialize() override;

  void resize(int width, int height, float scale) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrameData(const CefRenderHandler::PaintElementType& type,
                       const CefRenderHandler::RectList& dirtyRects,
                       const FrameDataType& dataType,
                       const FrameData& data) override;

  void render() override;
};

#endif // DX11RENDERBACKEND_H
