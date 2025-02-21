#include "DX11RenderBackend.h"

using namespace Microsoft::WRL;

DX11RenderBackend::DX11RenderBackend() {}

DX11RenderBackend::~DX11RenderBackend() {}

void
DX11RenderBackend::setBackgroundColor(const CefColor& color)
{
  m_backgroundColor = color;
}

void
DX11RenderBackend::updatePopupVisibility(bool visible)
{
  m_showPopup = visible;
}

void
DX11RenderBackend::updatePopupRect(const CefRect& rect)
{
  m_popupRect = rect;
}

void
DX11RenderBackend::updateFrame(const CefRenderHandler::PaintElementType& type,
                               const CefRenderHandler::RectList& dirtyRects,
                               const void* buffer,
                               const CefSize& size)
{
}

void
DX11RenderBackend::updateTexture(const CefRenderHandler::PaintElementType& type,
                                 const CefRenderHandler::RectList& dirtyRects,
                                 const void* handle,
                                 int format)
{
  // Open shared texture handle
  ComPtr<ID3D11Texture2D> pSharedTexture;
  HRESULT hr = m_d3dDevice->OpenSharedResource((HANDLE)handle, __uuidof(ID3D11Texture2D), (LPVOID*)&pSharedTexture);
  if (FAILED(hr)) {
    return;
  }

  if (PET_VIEW == type) {
    m_d3dContext->CopyResource(m_viewTexture.Get(), pSharedTexture.Get());
  } else if (PET_POPUP == type) {
    m_d3dContext->CopyResource(m_popupTexture.Get(), pSharedTexture.Get());
  } else {
  }
}

void
DX11RenderBackend::render(void* painter, int width, int height)
{
  // 1. Clear the back buffer
  // RGBA
  float clearColor[4] = {
    CefColorGetR(m_backgroundColor),
    CefColorGetG(m_backgroundColor),
    CefColorGetB(m_backgroundColor),
    CefColorGetA(m_backgroundColor),
  };
  m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

  ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

  // 2. Render view
  pShaderResourceView.Reset();
  HRESULT hr = m_d3dDevice->CreateShaderResourceView(m_viewTexture.Get(), NULL, &pShaderResourceView);
  if (!FAILED(hr) && pShaderResourceView) {
    m_d3dContext->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());
  }

  // 3. Render popup
  if (m_showPopup) {
    pShaderResourceView.Reset();
    HRESULT hr = m_d3dDevice->CreateShaderResourceView(m_popupTexture.Get(), NULL, &pShaderResourceView);
    if (!FAILED(hr) && pShaderResourceView) {
      m_d3dContext->PSSetShaderResources(0, 1, pShaderResourceView.GetAddressOf());
    }
  }

  // Present the back buffer to the screen
  m_swapChain->Present(0, 0);
}

void
DX11RenderBackend::resize(int width, int height)
{
  if (m_d3dContext) {
    m_d3dContext->OMSetRenderTargets(0, 0, 0);
  }

  // Release all outstanding references to the swap chain's buffers.
  m_renderTargetView.Reset();
  m_d3dContext->Flush();

  // Resize the swap chain
  HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    return;
  }

  // Recreate the render target view
  ComPtr<ID3D11Texture2D> pBackBuffer;
  hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
  if (FAILED(hr)) {
    return;
  }

  hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &m_renderTargetView);
  if (FAILED(hr)) {
    return;
  }

  m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  m_d3dContext->RSSetViewports(1, &vp);
}
