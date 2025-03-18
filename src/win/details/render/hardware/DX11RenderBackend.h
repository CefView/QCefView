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

#ifndef WINDOWS_DIRECT_COMPOSITION
#define WINDOWS_DIRECT_COMPOSITION 0
#endif

#include <mutex>

#include "details/render/ICefViewRenderer.h"

/// <summary>
///
/// </summary>
class DX11RenderBackend : public ICefViewRenderer
{
private:
  HWND m_hWnd = nullptr;
  bool m_showPopup = false;
  CefRect m_popupRect;
  CefColor m_backgroundColor = 0;

  float m_scale = 1.0f;
  int m_width = 800;
  int m_height = 600;

  // lock
  std::mutex m_d3dContextLock;

  // device/context
  Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
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
  D3D11_TEXTURE2D_DESC m_lastViewTextureDesc = {};
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_viewTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_viewSRV;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_viewVertexBuffer;

  // popup texture and SRV
  D3D11_TEXTURE2D_DESC m_lastPopupTextureDesc = {};
  Microsoft::WRL::ComPtr<ID3D11Texture2D> m_popupTexture;
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_popupSRV;
  Microsoft::WRL::ComPtr<ID3D11Buffer> m_popupVertexBuffer;

protected:
  bool CreateDeviceAndSwapchain();

  bool CreateShaderResource();
  bool CreateSampler();
  bool CreateBlender();
  bool CreateRenderTarget();

  void SetupPipeline();
  void HandleDeviceLost();

  bool CreateQuadVertexBuffer(float x,                //
                              float y,                //
                              float w,                //
                              float h,                //
                              ID3D11Buffer** ppBuffer //
  );

  void UpdateTextureResource(Microsoft::WRL::ComPtr<ID3D11Texture2D>& pSharedTexture,
                             Microsoft::WRL::ComPtr<ID3D11Texture2D>& pTargetTexture,
                             Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pTargetSRV,
                             D3D11_TEXTURE2D_DESC& targetTextureDesc);

  void SetTargetView();
  void ClearTargetView();
  void DrawCefView();
  void DrawCefPopup();
  void RenderPresent();

public:
  DX11RenderBackend();

  ~DX11RenderBackend();

  bool isHardware() override { return true; }

  bool initialize(void* wid, int width, int height, float scale, const CefColor& background) override;

  void uninitialize() override;

  void resize(int width, int height, float scale) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrameData(const CefRenderHandler::PaintElementType& type,
                       const CefRenderHandler::RectList& dirtyRects,
                       const FrameDataType& dataType,
                       const FrameData& data) override;

  void render(void* painter) override;
};

#endif // DX11RENDERBACKEND_H
