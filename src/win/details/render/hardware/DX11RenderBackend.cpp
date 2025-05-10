#include "DX11RenderBackend.h"

#include <DirectXMath.h>
#include <d3dcompiler.h>

#define HR_CHECK(exp)                                                                                                  \
  if (!(exp)) {                                                                                                        \
    return false;                                                                                                      \
  }

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
DX11RenderBackend::CreateDeviceAndSwapchain(HWND hWnd, int width, int height)
{
  // use the DirectComposition to render the CefView
  // Create D3D11 device and context
  ComPtr<ID3D11Device> pD3dDevice;
  ComPtr<ID3D11DeviceContext> pD3dContext;
#if WINDOWS_DIRECT_COMPOSITION
  HR_CHECK(S_OK == ::D3D11CreateDevice(nullptr,
                                       D3D_DRIVER_TYPE_HARDWARE,
                                       nullptr,
                                       0,
                                       nullptr,
                                       0,
                                       D3D11_SDK_VERSION,
                                       pD3dDevice.ReleaseAndGetAddressOf(),
                                       nullptr,
                                       pD3dContext.ReleaseAndGetAddressOf()));

  // Get DXGI device
  ComPtr<IDXGIDevice1> pDxgiDevice;
  HR_CHECK(S_OK == pD3dDevice.As(&pDxgiDevice));

  // Get DXGI adapter
  ComPtr<IDXGIAdapter> pDxgiAdapter;
  HR_CHECK(S_OK == pDxgiDevice->GetAdapter(pDxgiAdapter.GetAddressOf()));

  // Get DXGI factory
  ComPtr<IDXGIFactory2> pDxgiFactory2;
  HR_CHECK(S_OK == pDxgiAdapter->GetParent(IID_PPV_ARGS(pDxgiFactory2.ReleaseAndGetAddressOf())));

  // Create swapchain description
  DXGI_SWAP_CHAIN_DESC1 sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount = 2;
  sd.Width = width;
  sd.Height = height;
  sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  sd.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
  sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  // Create swapchain
  ComPtr<IDXGISwapChain1> pSwapChain;
  HR_CHECK(S_OK == pDxgiFactory2->CreateSwapChainForComposition(
                     pD3dDevice.Get(), &sd, nullptr, pSwapChain.ReleaseAndGetAddressOf()));

  // Create DComposition device
  ComPtr<IDCompositionDevice> pDecompositionDevice;
  HR_CHECK(S_OK ==
           ::DCompositionCreateDevice(pDxgiDevice.Get(), IID_PPV_ARGS(pDecompositionDevice.ReleaseAndGetAddressOf())));

  // Create DComposition visual
  ComPtr<IDCompositionVisual> pDCompositionVisual;
  HR_CHECK(S_OK == pDecompositionDevice->CreateVisual(pDCompositionVisual.ReleaseAndGetAddressOf()));

  // Set swapchain to visual
  HR_CHECK(S_OK == pDCompositionVisual->SetContent(pSwapChain.Get()));

  // Create DComposition target
  ComPtr<IDCompositionTarget> pDCompositionTarget;
  HR_CHECK(S_OK ==
           pDecompositionDevice->CreateTargetForHwnd(hWnd, FALSE, pDCompositionTarget.ReleaseAndGetAddressOf()));

  // Set root to target
  HR_CHECK(S_OK == pDCompositionTarget->SetRoot(pDCompositionVisual.Get()));

  // Commit
  HR_CHECK(S_OK == pDecompositionDevice->Commit());

  // Save
  m_dcompositionTarget = pDCompositionTarget;
  m_dcompositionDevice = pDecompositionDevice;
#else
  // create device and swapchain
  DXGI_SWAP_CHAIN_DESC sd = {};
  sd.BufferCount = 2;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  ComPtr<IDXGISwapChain> pSwapChain;
  HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE,
                                               nullptr,
                                               0,
                                               nullptr,
                                               0,
                                               D3D11_SDK_VERSION,
                                               &sd,
                                               pSwapChain.ReleaseAndGetAddressOf(),
                                               pD3dDevice.ReleaseAndGetAddressOf(),
                                               nullptr,
                                               pD3dContext.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }
#endif

  // convert to ID3D11Device1
  Microsoft::WRL::ComPtr<ID3D11Device1> pD3dDevice1;
  HR_CHECK(S_OK == pD3dDevice.As(&pD3dDevice1));

  m_swapChain = pSwapChain;
  m_d3dContext = pD3dContext;
  m_d3dDevice1 = pD3dDevice1;
  m_d3dDevice = pD3dDevice;
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
  ComPtr<ID3D11InputLayout> pInputLayout;
  hr = m_d3dDevice->CreateInputLayout(
    id, ARRAYSIZE(id), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), pInputLayout.ReleaseAndGetAddressOf());
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

  ComPtr<ID3D11PixelShader> pPixelShader;
  hr = m_d3dDevice->CreatePixelShader(
    psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, pPixelShader.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  m_inputLayout = pInputLayout;
  m_pixelShader = pPixelShader;
  return true;
}

bool
DX11RenderBackend::CreateSampler()
{
  D3D11_SAMPLER_DESC desc = {};
  desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  desc.MinLOD = 0.0f;
  desc.MaxLOD = 0.0f;
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
DX11RenderBackend::CreateRenderTargetView()
{
  if (!m_swapChain) {
    return false;
  }

  // get back buffer
  ComPtr<ID3D11Texture2D> pBackBuffer;
  HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.ReleaseAndGetAddressOf()));
  if (FAILED(hr)) {
    return false;
  }
  DXGI_SWAP_CHAIN_DESC swapchainDesc = { 0 };
  m_swapChain->GetDesc(&swapchainDesc);

  // create render target view
  D3D11_RENDER_TARGET_VIEW_DESC vdesc = {};
  vdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  vdesc.Texture2D.MipSlice = 0;
  vdesc.Format = swapchainDesc.BufferDesc.Format;
  ComPtr<ID3D11RenderTargetView> pRenderTargetView;
  hr = m_d3dDevice->CreateRenderTargetView(pBackBuffer.Get(), &vdesc, pRenderTargetView.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return false;
  }

  m_renderTargetView = pRenderTargetView;
  return true;
}

void
DX11RenderBackend::SetupPipeline(int width, int height)
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

  ////////////////////////////////////////////////////////////////////////
  // RS
  // sync viewport
  D3D11_VIEWPORT vpList[] = {
    {
      0.f,             // FLOAT TopLeftX;
      0.f,             // FLOAT TopLeftY;
      (FLOAT)(width),  // FLOAT Width;
      (FLOAT)(height), // FLOAT Height;
      D3D11_MIN_DEPTH, // FLOAT MinDepth;
      D3D11_MAX_DEPTH, // FLOAT MaxDepth;
    } //
  };
  m_d3dContext->RSSetViewports(ARRAYSIZE(vpList), vpList);
}

void
DX11RenderBackend::ResizeResource(int width, int height)
{
  // remove and release current render target
  m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
  m_renderTargetView.Reset();

  // resize the swap chain
  HRESULT hr = m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(hr)) {
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
      return;
    }
  }

  // create new render target
  if (!CreateRenderTargetView()) {
    return;
  }

  // re-bind render target
  ID3D11RenderTargetView* rtvList[] = { m_renderTargetView.Get() };
  m_d3dContext->OMSetRenderTargets(ARRAYSIZE(rtvList), rtvList, nullptr);

  // sync viewport
  D3D11_VIEWPORT vpList[] = {
    {
      0.f,             // FLOAT TopLeftX;
      0.f,             // FLOAT TopLeftY;
      (FLOAT)(width),  // FLOAT Width;
      (FLOAT)(height), // FLOAT Height;
      D3D11_MIN_DEPTH, // FLOAT MinDepth;
      D3D11_MAX_DEPTH, // FLOAT MaxDepth;
    } //
  };
  m_d3dContext->RSSetViewports(ARRAYSIZE(vpList), vpList);
}

void
DX11RenderBackend::HandleDeviceLost()
{
  // recreate everything
  auto size = widgetSize();
  auto scale = widgetScale();
  auto color = widgetBackground();
  initialize(m_widget, size.width(), size.height(), scale, color);
}

#define ScreenToNDC(x, y) XMFLOAT3(2.0f * (x) / viewWidth - 1.0f, 1.0f - 2.0f * (y) / viewHeight, 1.0f)

bool
DX11RenderBackend::CreateQuadVertexBuffer(float x,        //
                                          float y,        //
                                          float w,        //
                                          float h,        //
                                          int viewWidth,  //
                                          int viewHeight, //
                                          ID3D11Buffer** ppBuffer)
{
  // vertex
  Vertex vertices[] = //
    {
      { ScreenToNDC(x + 0, y + 0), XMFLOAT2(0.0f, 0.0f) }, // LEFT TOP
      { ScreenToNDC(x + w, y + 0), XMFLOAT2(1.0f, 0.0f) }, // RIGHT TOP
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

DX11RenderBackend::DX11RenderBackend() {}

DX11RenderBackend::~DX11RenderBackend() {}

bool
DX11RenderBackend::initialize(QWidget* widget, int width, int height, float scale, const QColor& clear)
{
  if (!widget || !widget->winId()) {
    return false;
  }
  m_widget = widget;
  HWND hWnd = reinterpret_cast<HWND>(widget->winId());

  m_cefViewTextureDesc = {};
  m_cefViewTexture.Reset();
  m_cefViewVertexBuffer.Reset();
  m_cefViewShaderResourceView.Reset();

  m_cefPopupTextureDesc = {};
  m_cefPopupTexture.Reset();
  m_cefPopupVertexBuffer.Reset();
  m_cefPopupShaderResourceView.Reset();

  // create device/context/swapchain
  if (!CreateDeviceAndSwapchain(hWnd, width * scale, height * scale)) {
    uninitialize();
    return false;
  }

  // create shader resources
  if (!CreateShaderResource()) {
    uninitialize();
    return false;
  }

  // create sampler
  if (!CreateSampler()) {
    uninitialize();
    return false;
  }

  // create blender
  if (!CreateBlender()) {
    uninitialize();
    return false;
  }

  // create render target
  if (!CreateRenderTargetView()) {
    uninitialize();
    return false;
  }

  // setup pipeline
  SetupPipeline(width * scale, height * scale);

  return true;
}

void
DX11RenderBackend::uninitialize()
{
#if _WIN32_WINNT >= 0x602
  m_dcompositionDevice.Reset();
  m_dcompositionTarget.Reset();
#endif

  // Release resources
  m_inputLayout.Reset();
  m_vertexShader.Reset();
  m_samplerState.Reset();
  m_pixelShader.Reset();
  m_blenderState.Reset();
  m_renderTargetView.Reset();

  m_cefViewTextureDesc = {};
  m_cefViewTexture.Reset();
  m_cefViewVertexBuffer.Reset();
  m_cefViewShaderResourceView.Reset();

  m_cefPopupTextureDesc = {};
  m_cefPopupTexture.Reset();
  m_cefPopupVertexBuffer.Reset();
  m_cefPopupShaderResourceView.Reset();

  m_swapChain.Reset();
  m_d3dContext.Reset();
  m_d3dDevice1.Reset();
  m_d3dDevice.Reset();
}

void
DX11RenderBackend::resize(int width, int height, float scale)
{
  if (CefCurrentlyOn(TID_UI)) {
    // resize resource
    ResizeResource(width * scale, height * scale);
    // render immediately
    render();
  } else {
    // perform the resize in render thread
    QPointer<DX11RenderBackend> self = this;
    CefPostTask(TID_UI, new RenderTask([=]() {
                  if (self) {
                    // resize resource
                    self->ResizeResource(width * scale, height * scale);
                    // render immediately
                    self->render();
                  }
                }));
  }
}

void
DX11RenderBackend::updatePopupVisibility(bool visible)
{
  m_showPopup = visible;

  if (!m_showPopup) {
    m_popupRect = CefRect();
    m_cefPopupShaderResourceView.Reset();
  }
}

void
DX11RenderBackend::updatePopupRect(const CefRect& rect)
{
  m_popupRect = rect;
}

void
DX11RenderBackend::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                   const CefRenderHandler::RectList& dirtyRects,
                                   const FrameDataType& dataType,
                                   const FrameData& data)
{
  if (!m_d3dDevice1) {
    return;
  }

  // we only process Texture data
  if (dataType != FrameDataType::GpuTexture) {
    return;
  }

  // open shared texture
  ComPtr<ID3D11Texture2D> pSharedTexture;
  HRESULT hr = m_d3dDevice1->OpenSharedResource1((HANDLE)data.texture.handle,
                                                 IID_PPV_ARGS(pSharedTexture.ReleaseAndGetAddressOf()));
  if (FAILED(hr) || !pSharedTexture) {
    return;
  }

  // get texture description
  D3D11_TEXTURE2D_DESC sharedTextureDesc = {};
  pSharedTexture->GetDesc(&sharedTextureDesc);

  // create shader resource view description
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  srvDesc.Format = sharedTextureDesc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MostDetailedMip = 0;
  srvDesc.Texture2D.MipLevels = 1;

  // create shader resource view
  Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSharedTextureShaderResourceView;
  hr = m_d3dDevice1->CreateShaderResourceView(
    pSharedTexture.Get(), &srvDesc, pSharedTextureShaderResourceView.ReleaseAndGetAddressOf());
  if (FAILED(hr)) {
    return;
  }

  // update
  if (PET_VIEW == type) {
    m_cefViewTexture = pSharedTexture;
    m_cefViewTextureDesc = sharedTextureDesc;
    m_cefViewShaderResourceView = pSharedTextureShaderResourceView;
  } else if (PET_POPUP == type) {
    m_cefPopupTexture = pSharedTexture;
    m_cefPopupTextureDesc = sharedTextureDesc;
    m_cefPopupShaderResourceView = pSharedTextureShaderResourceView;
  } else {
    return;
  }

  // render directly in current thread (CEF UI thread)
  render();
}

void
DX11RenderBackend::render()
{
  auto size = widgetSize();
  auto scale = widgetScale();
  auto color = widgetBackground();

  // clear the back buffer (RGBA)
  float clearColor[4] = {
    static_cast<float>(color.red() / 255.f),
    static_cast<float>(color.green() / 255.f),
    static_cast<float>(color.blue() / 255.f),
    static_cast<float>(color.alpha() / 255.f),
  };
  m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

  // draw cef view
  if (m_cefViewShaderResourceView) {
    // create vertex buffer
    CreateQuadVertexBuffer(0.0f,                                            //
                           0.0f,                                            //
                           static_cast<float>(m_cefViewTextureDesc.Width),  //
                           static_cast<float>(m_cefViewTextureDesc.Height), //
                           size.width() * scale,                            //
                           size.height() * scale,                           //
                           m_cefViewVertexBuffer.ReleaseAndGetAddressOf()   //
    );

    // assembly
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ID3D11Buffer* vertexBufferList[] = { m_cefViewVertexBuffer.Get() };
    m_d3dContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBufferList), vertexBufferList, &stride, &offset);

    // pixel shader
    ID3D11ShaderResourceView* viewSRVList[] = { m_cefViewShaderResourceView.Get() };
    m_d3dContext->PSSetShaderResources(0, ARRAYSIZE(viewSRVList), viewSRVList);
    m_d3dContext->Draw(4, 0);
  }

  // draw cef popup
  if (m_cefPopupShaderResourceView && m_showPopup) {
    // create vertex buffer
    CreateQuadVertexBuffer(m_popupRect.x * scale,                            //
                           m_popupRect.y * scale,                            //
                           static_cast<float>(m_cefPopupTextureDesc.Width),  //
                           static_cast<float>(m_cefPopupTextureDesc.Height), //
                           size.width() * scale,                             //
                           size.height() * scale,                            //
                           m_cefPopupVertexBuffer.ReleaseAndGetAddressOf()   //
    );

    // assembly
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    ID3D11Buffer* vertexBufferList[] = { m_cefPopupVertexBuffer.Get() };
    m_d3dContext->IASetVertexBuffers(0, ARRAYSIZE(vertexBufferList), vertexBufferList, &stride, &offset);

    // pixel shader
    ID3D11ShaderResourceView* popupSRVList[] = { m_cefPopupShaderResourceView.Get() };
    m_d3dContext->PSSetShaderResources(0, ARRAYSIZE(popupSRVList), popupSRVList);
    m_d3dContext->Draw(4, 0);
  }

  // present the back buffer to the screen
  HRESULT hr = m_swapChain->Present(0, 0);
  if (FAILED(hr)) {
    // if the device was removed either by a disconnection or a driver upgrade, we
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
      HandleDeviceLost();
    }
  }
}
