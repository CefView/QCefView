#include "DX11RenderBackend.h"

#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX;
using namespace Microsoft::WRL;

// vertex
struct Vertex
{
  XMFLOAT3 position;
  XMFLOAT2 texcoord;
};

// vertex shader
const char* g_vertexShaderSource = R"(
struct VS_INPUT
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = input.pos;
	output.tex = input.tex;
	return output;
}
)";

// pixel shader
const char* g_pixelShaderSource = R"(
Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(VS_OUTPUT input) : SV_Target
{
	return tex0.Sample(samp0, input.tex);
}
)";

bool
DX11RenderBackend::CreateDeviceAndSwapchain()
{
  // create device and swapchain
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 1;
  sd.BufferDesc.Width = m_width;
  sd.BufferDesc.Height = m_height;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = m_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               nullptr,
                                               0,
                                               nullptr,
                                               0,
                                               D3D11_SDK_VERSION,
                                               &sd,
                                               m_swapChain.ReleaseAndGetAddressOf(),
                                               m_d3dDevice.ReleaseAndGetAddressOf(),
                                               nullptr,
                                               m_d3dContext.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

bool
DX11RenderBackend::CreateShaderResource()
{
  ComPtr<ID3DBlob> errorBlob;

  // compile vertex shader source
  ComPtr<ID3DBlob> vsBlob;
  HRESULT hr = ::D3DCompile(g_vertexShaderSource,
                            strlen(g_vertexShaderSource),
                            nullptr,
                            nullptr,
                            nullptr,
                            "main",
                            "vs_4_0",
                            D3DCOMPILE_ENABLE_STRICTNESS,
                            0,
                            vsBlob.ReleaseAndGetAddressOf(),
                            errorBlob.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  // create vertex shader
  hr = m_d3dDevice->CreateVertexShader(
    vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  // create input layout
  D3D11_INPUT_ELEMENT_DESC id[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };
  hr = m_d3dDevice->CreateInputLayout(
    id, ARRAYSIZE(id), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_inputLayout.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  // create pixel shader
  ComPtr<ID3DBlob> psBlob;
  hr = ::D3DCompile(g_pixelShaderSource,
                    strlen(g_pixelShaderSource),
                    nullptr,
                    nullptr,
                    nullptr,
                    "main",
                    "ps_4_0",
                    D3DCOMPILE_ENABLE_STRICTNESS,
                    0,
                    psBlob.ReleaseAndGetAddressOf(),
                    errorBlob.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  hr = m_d3dDevice->CreatePixelShader(
    psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

bool
DX11RenderBackend::CreateSampler()
{
  D3D11_SAMPLER_DESC desc = {};
  desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  desc.MinLOD = 0.0f;
  desc.MaxLOD = D3D11_FLOAT32_MAX;
  HRESULT hr = m_d3dDevice->CreateSamplerState(&desc, m_samplerState.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

bool
DX11RenderBackend::CreateBlender()
{
  D3D11_BLEND_DESC desc;
  desc.AlphaToCoverageEnable = FALSE;
  desc.IndependentBlendEnable = FALSE;
  for (auto& n : desc.RenderTarget) {
    n.BlendEnable = TRUE;
    n.SrcBlend = D3D11_BLEND_ONE;
    n.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    n.SrcBlendAlpha = D3D11_BLEND_ONE;
    n.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    n.BlendOp = D3D11_BLEND_OP_ADD;
    n.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    n.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  }
  HRESULT hr = m_d3dDevice->CreateBlendState(&desc, m_blenderState.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

bool
DX11RenderBackend::CreateRenderTarget()
{
  // resize the swap chain
  HRESULT hr = m_swapChain->ResizeBuffers(0, m_width, m_height, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
      HandleDeviceLost();
    }
    return false;
  }

  // get back buffer
  ComPtr<ID3D11Texture2D> pBackBuffer;
  hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.ReleaseAndGetAddressOf()));
  if (FAILED(hr)) {
    return false;
  }

  DXGI_SWAP_CHAIN_DESC desc;
  m_swapChain->GetDesc(&desc);

  // create render target view
  D3D11_RENDER_TARGET_VIEW_DESC vdesc = {};
  vdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  vdesc.Texture2D.MipSlice = 0;
  vdesc.Format = desc.BufferDesc.Format;
  hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), &vdesc, m_renderTargetView.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  // create vertex buffer
  return CreateQuadVertexBuffer(0.0f,                                       //
                                0.0f,                                       //
                                m_width,                                    //
                                m_height,                                   //
                                m_viewVertexBuffer.ReleaseAndGetAddressOf() //
  );
}

void
DX11RenderBackend::SetupPipeline()
{
  ////////////////////////////////////////////////////////////////////////
  // IA
  // input assembler
  m_d3dContext->IASetInputLayout(m_inputLayout.Get());
  m_d3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  ////////////////////////////////////////////////////////////////////////
  // VS
  // vertex shader
  m_d3dContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);

  ////////////////////////////////////////////////////////////////////////
  // Rasterizer
  // viewport
  D3D11_VIEWPORT vpList[] = {
    {
      0.f,             // FLOAT TopLeftX;
      0.f,             // FLOAT TopLeftY;
      (FLOAT)m_width,  // FLOAT Width;
      (FLOAT)m_height, // FLOAT Height;
      D3D11_MIN_DEPTH, // FLOAT MinDepth;
      D3D11_MAX_DEPTH, // FLOAT MaxDepth;
    } //
  };
  m_d3dContext->RSSetViewports(ARRAYSIZE(vpList), vpList);

  ////////////////////////////////////////////////////////////////////////
  // PS
  // pixel shader
  m_d3dContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

  // sampler
  ID3D11SamplerState* samplerList[] = { m_samplerState.Get() };
  m_d3dContext->PSSetSamplers(0, ARRAYSIZE(samplerList), samplerList);

  ////////////////////////////////////////////////////////////////////////
  // OM
  // blend state
  float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  m_d3dContext->OMSetBlendState(m_blenderState.Get(), blendFactor, 0xffffffff);

  // render target
  ID3D11RenderTargetView* rtvList[] = { m_renderTargetView.Get() };
  m_d3dContext->OMSetRenderTargets(ARRAYSIZE(rtvList), rtvList, nullptr);
}

void
DX11RenderBackend::HandleDeviceLost()
{
  // recreate everything
  initialize(m_hWnd, m_width, m_height, m_scale, m_backgroundColor);
}

bool
DX11RenderBackend::CreateQuadVertexBuffer(float x,                //
                                          float y,                //
                                          float w,                //
                                          float h,                //
                                          ID3D11Buffer** ppBuffer //
)
{
  auto ScreenToNDC = [this](float px, float py) -> XMFLOAT3 {
    return XMFLOAT3(2.0f * px / m_width - 1.0f,  // X: [0, width] → [-1, 1]
                    1.0f - 2.0f * py / m_height, // Y: [0, height] → [1, -1]
                    1.0f);
  };

  // vertex
  Vertex vertices[] = //
    {
      { ScreenToNDC(x + 0, y - 0), XMFLOAT2(0.0f, 0.0f) }, // LEFT TOP
      { ScreenToNDC(x + w, y - 0), XMFLOAT2(1.0f, 0.0f) }, // RIGHT TOP
      { ScreenToNDC(x + 0, y + h), XMFLOAT2(0.0f, 1.0f) }, // LEFT BOT
      { ScreenToNDC(x + w, y + h), XMFLOAT2(1.0f, 1.0f) }  // RIGHT BOT
    };

  D3D11_BUFFER_DESC desc = {};
  desc.Usage = D3D11_USAGE_DEFAULT;
  desc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
  desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  desc.CPUAccessFlags = 0;

  D3D11_SUBRESOURCE_DATA srd = {};
  srd.pSysMem = vertices;

  HRESULT hr = m_d3dDevice->CreateBuffer(&desc, &srd, ppBuffer);
  if (FAILED(hr)) {
    return false;
  }

  return true;
}

void
DX11RenderBackend::UpdateTextureResource(Microsoft::WRL::ComPtr<ID3D11Texture2D>& pSharedTexture,
                                         Microsoft::WRL::ComPtr<ID3D11Texture2D>& pTargetTexture,
                                         Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pTargetSRV,
                                         D3D11_TEXTURE2D_DESC& targetTextureDesc)
{
  HRESULT hr = E_FAIL;

  // get texture description
  D3D11_TEXTURE2D_DESC sharedTextureDesc = {};
  pSharedTexture->GetDesc(&sharedTextureDesc);

  // need to recreate?
  bool needRecreate = false;
  if (needRecreate                                            //
      || sharedTextureDesc.Width != targetTextureDesc.Width   //
      || sharedTextureDesc.Height != targetTextureDesc.Height //
      || sharedTextureDesc.Format != targetTextureDesc.Format //
  ) {
    // flip flag
    needRecreate = true;

    // recreate texture and copy full texture data
    hr = m_d3dDevice->CreateTexture2D(&sharedTextureDesc, nullptr, pTargetTexture.ReleaseAndGetAddressOf());
    if (FAILED(hr)) {
      return;
    }
  }

  if (!pTargetTexture) {
    return;
  }

  {
    // copy full texture with lock
    std::lock_guard<std::mutex> l(m_d3dContextLock);
    m_d3dContext->CopyResource(pTargetTexture.Get(), pSharedTexture.Get());
  }

  if (needRecreate) {
    // create SRV description
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = sharedTextureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    // update shader resource view
    hr = m_d3dDevice->CreateShaderResourceView(pTargetTexture.Get(), &srvDesc, pTargetSRV.ReleaseAndGetAddressOf());
    if (FAILED(hr)) {
      return;
    }
  }

  // update texture desc
  targetTextureDesc = sharedTextureDesc;
}

void
DX11RenderBackend::ClearTargetView()
{
  // clear the back buffer (RGBA)
  float clearColor[4] = {
    static_cast<float>(CefColorGetR(m_backgroundColor)),
    static_cast<float>(CefColorGetG(m_backgroundColor)),
    static_cast<float>(CefColorGetB(m_backgroundColor)),
    static_cast<float>(CefColorGetA(m_backgroundColor)),
  };
  m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
}

void
DX11RenderBackend::DrawCefView()
{
  // assembly
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  ID3D11Buffer* vertexBufferList[] = { m_viewVertexBuffer.Get() };
  m_d3dContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBufferList), vertexBufferList, &stride, &offset);

  // pixel shader
  ID3D11ShaderResourceView* viewSRVList[] = { m_viewSRV.Get() };
  m_d3dContext->PSSetShaderResources(0, ARRAYSIZE(viewSRVList), viewSRVList);
  m_d3dContext->Draw(4, 0);
}

void
DX11RenderBackend::DrawCefPopup()
{
  if (!m_showPopup) {
    return;
  }

  // assembly
  UINT stride = sizeof(Vertex);
  UINT offset = 0;
  ID3D11Buffer* vertexBufferList[] = { m_popupVertexBuffer.Get() };
  m_d3dContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBufferList), vertexBufferList, &stride, &offset);

  // pixel shader
  ID3D11ShaderResourceView* popupSRVList[] = { m_popupSRV.Get() };
  m_d3dContext->PSSetShaderResources(0, ARRAYSIZE(popupSRVList), popupSRVList);
  m_d3dContext->Draw(4, 0);
}

void
DX11RenderBackend::RenderPresent()
{
  // Present the back buffer to the screen
  HRESULT hr = m_swapChain->Present(1, 0);
  if (FAILED(hr)) {
    // If the device was removed either by a disconnection or a driver upgrade, we
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
      HandleDeviceLost();
    }
  }
}

DX11RenderBackend::DX11RenderBackend() {}

DX11RenderBackend::~DX11RenderBackend() {}

bool
DX11RenderBackend::initialize(void* wid, int width, int height, float scale, const CefColor& background)
{
  m_hWnd = (HWND)wid;
  m_width = width;
  m_height = height;
  m_scale = scale;
  m_backgroundColor = background;

  m_lastViewTextureDesc = {};
  m_viewTexture.Reset();
  m_viewSRV.Reset();

  m_lastPopupTextureDesc = {};
  m_popupTexture.Reset();
  m_popupSRV.Reset();

  // create device/context/swapchain
  if (!CreateDeviceAndSwapchain()) {
    return false;
  }

  // create shader resources
  if (!CreateShaderResource()) {
    return false;
  }

  // create sampler
  if (!CreateSampler()) {
    return false;
  }

  // create blender
  if (!CreateBlender()) {
    return false;
  }

  // create render target
  if (!CreateRenderTarget()) {
    return false;
  }

  // setup pipeline
  SetupPipeline();

  return true;
}

void
DX11RenderBackend::uninitialize()
{
  // Release resources
  m_inputLayout.Reset();
  m_vertexShader.Reset();
  m_samplerState.Reset();
  m_pixelShader.Reset();
  m_blenderState.Reset();
  m_renderTargetView.Reset();

  m_renderTargetView.Reset();

  m_lastPopupTextureDesc = {};
  m_popupSRV.Reset();
  m_popupTexture.Reset();

  m_lastViewTextureDesc = {};
  m_viewSRV.Reset();
  m_viewTexture.Reset();

  m_swapChain.Reset();
  m_d3dContext.Reset();
  m_d3dDevice.Reset();

  m_hWnd = nullptr;
  m_width = 0;
  m_height = 0;
  m_scale = 1.0f;
  m_backgroundColor = 0;
}

void
DX11RenderBackend::resize(int width, int height, float scale)
{
  std::lock_guard<std::mutex> l(m_d3dContextLock);

  // update size
  m_scale = scale;
  m_width = width * scale;
  m_height = height * scale;

  // remove current render target
  m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);

  // release current render target
  m_renderTargetView.Reset();

  // flush context
  m_d3dContext->Flush();

  // create new render target
  if (CreateRenderTarget()) {
    // update pipeline: reset viewport and render target
    // reset viewport
    D3D11_VIEWPORT vpList[] = {
      {
        0.f,             // FLOAT TopLeftX;
        0.f,             // FLOAT TopLeftY;
        (FLOAT)m_width,  // FLOAT Width;
        (FLOAT)m_height, // FLOAT Height;
        D3D11_MIN_DEPTH, // FLOAT MinDepth;
        D3D11_MAX_DEPTH, // FLOAT MaxDepth;
      } //
    };
    m_d3dContext->RSSetViewports(ARRAYSIZE(vpList), vpList);

    // reset render target
    ID3D11RenderTargetView* rtvList[] = { m_renderTargetView.Get() };
    m_d3dContext->OMSetRenderTargets(ARRAYSIZE(rtvList), rtvList, nullptr);
  }
}

void
DX11RenderBackend::updatePopupVisibility(bool visible)
{
  m_showPopup = visible;
}

void
DX11RenderBackend::updatePopupRect(const CefRect& rect)
{
  CefRect newRect = rect;
  newRect.x *= m_scale;
  newRect.y *= m_scale;
  newRect.width *= m_scale;
  newRect.height *= m_scale;
  if (newRect == m_popupRect) {
    return;
  }

  // update popup rect
  m_popupRect = newRect;

  // create vertex buffer
  CreateQuadVertexBuffer(m_popupRect.x,                               //
                         m_popupRect.y,                               //
                         m_popupRect.width,                           //
                         m_popupRect.height,                          //
                         m_popupVertexBuffer.ReleaseAndGetAddressOf() //
  );
}

void
DX11RenderBackend::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                   const CefRenderHandler::RectList& dirtyRects,
                                   const FrameDataType& dataType,
                                   const FrameData& data)
{
  // we only process Texture data
  if (dataType != FrameDataType::GpuTexture) {
    return;
  }

  // convert to ID3D11Device1
  Microsoft::WRL::ComPtr<ID3D11Device1> pD3DDevice1;
  HRESULT hr = m_d3dDevice.As(&pD3DDevice1);
  if (FAILED(hr) || !pD3DDevice1) {
    return;
  }

  // Open shared texture handle
  ComPtr<ID3D11Texture2D> pSharedTexture;
  hr = pD3DDevice1->OpenSharedResource1((HANDLE)data.texture.handle,
                                        IID_PPV_ARGS(pSharedTexture.ReleaseAndGetAddressOf()));
  if (FAILED(hr) || !pSharedTexture) {
    return;
  }

  // update
  if (PET_VIEW == type) {
    UpdateTextureResource(pSharedTexture, m_viewTexture, m_viewSRV, m_lastViewTextureDesc);
  } else if (PET_POPUP == type) {
    UpdateTextureResource(pSharedTexture, m_popupTexture, m_popupSRV, m_lastPopupTextureDesc);
  } else {
    return;
  }
}

void
DX11RenderBackend::render(void* painter)
{
  std::lock_guard<std::mutex> l(m_d3dContextLock);

  ClearTargetView();

  DrawCefView();

  DrawCefPopup();

  RenderPresent();
}
