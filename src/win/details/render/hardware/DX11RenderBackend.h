#ifndef DX11RENDERBACKEND_H
#define DX11RENDERBACKEND_H

#pragma once

#include <d3d11_2.h>
#include <wrl.h>

/// <summary>
///
/// </summary>
class DX11RenderBackend
{
private:
  HWND m_hWnd = nullptr;
  Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
  Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
  Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

public:
  DX11RenderBackend();

  ~DX11RenderBackend();

  bool initialize();

  void shutdown();
};

#endif // DX11RENDERBACKEND_H
