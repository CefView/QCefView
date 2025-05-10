#include "MetalRenderBackend.h"

#include <objc/objc.h>
#include <simd/simd.h>

#include <CoreFoundation/CoreFoundation.h>
#include <MacTypes.h>

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CGColor.h>
#import <IOSurface/IOSurface.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <QWindow>

struct Vertex
{
  vector_float4 position;
  vector_float2 texcoord;
};

const char* shaderSource = R"(
#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float4 position;
    float2 texCoord;
};

struct VertexOut {
    float4 position [[position]];
    float2 texCoord;
};

vertex VertexOut vertexShader(uint vertexID [[vertex_id]],
                              constant VertexIn *vertexArray [[buffer(0)]]) {
    VertexOut out;
    out.position = vertexArray[vertexID].position;
    out.texCoord = vertexArray[vertexID].texCoord;
    return out;
}

fragment float4 fragmentShader(VertexOut in [[stage_in]],
                               texture2d<float> texture [[texture(0)]]) {
    constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
    return texture.sample(textureSampler, in.texCoord);
}
)";

#define ScreenToNDC(x, y) (2.0f * (x) / width - 1.0f), (1.0f - 2.0f * (y) / height), 0.0f, 1.0f

class MetalRenderBackend::Implementation
{
public:
  NSView* m_view = nil;
  CAMetalLayer* m_metalLayer = nil;
  id<MTLDevice> m_device;
  id<MTLCommandQueue> m_renderQueue;
  id<MTLCommandQueue> m_updateQueue;
  id<MTLFunction> m_vertexFunction;
  id<MTLFunction> m_fragmentFunction;
  id<MTLRenderPipelineState> m_pipelineState;

  bool m_showPopup = false;
  CGRect m_popupRect = CGRectZero;

  MTLTextureDescriptor* m_cefViewTextureDesc = nil;
  id<MTLBuffer> m_cefViewVertexBuffer;
  id<MTLTexture> m_cefViewTexture;

  MTLTextureDescriptor* m_cefPopupTextureDesc = nil;
  id<MTLBuffer> m_cefPopupVertexBuffer;
  id<MTLTexture> m_cefPopupTexture;

  id<MTLBuffer> createQuadVertexBuffer(NSInteger x,
                                       NSInteger y,
                                       NSInteger w,
                                       NSInteger h,
                                       NSInteger width,
                                       NSInteger height)
  {
    //   x      y      z    w      u    v
    // -1.0   1.0   0.0  1.0   0.0  0.0  // LEFT TOP
    //  1.0   1.0   0.0  1.0   1.0  0.0  // RIGHT TOP
    // -1.0  -1.0   0.0  1.0   0.0  1.0  // LEFT BOT
    //  1.0  -1.0   0.0  1.0   1.0  1.0  // RIGHT BOT
    // vertex
    Vertex vertices[] = //
      {
        { { ScreenToNDC(x + 0, y + 0) }, { 0.0f, 0.0f } }, // LEFT TOP
        { { ScreenToNDC(x + w, y + 0) }, { 1.0f, 0.0f } }, // RIGHT TOP
        { { ScreenToNDC(x + 0, y + h) }, { 0.0f, 1.0f } }, // LEFT BOT
        { { ScreenToNDC(x + w, y + h) }, { 1.0f, 1.0f } }  // RIGHT BOT
      };

    return [m_device newBufferWithBytes:vertices length:sizeof(vertices) options:MTLResourceStorageModeShared];
  }
};

MetalRenderBackend::MetalRenderBackend()
  : m_pImpl(std::make_unique<Implementation>())
{
}

MetalRenderBackend::~MetalRenderBackend()
{
  m_pImpl.reset();
}

bool
MetalRenderBackend::initialize(QWidget* widget, int width, int height, float scale, const QColor& clear)
{
  if (!m_pImpl) {
    return false;
  }

  if (!widget || !widget->winId()) {
    return false;
  }
  m_widget = widget;

  @autoreleasepool {
    auto view = (__bridge NSView*)((void*)(m_widget->winId()));
    auto size = CGSizeMake(width, height);
    auto clearColor = MTLClearColorMake( //
      clear.red() / 255.f,               //
      clear.green() / 255.f,             //
      clear.blue() / 255.f,              //
      clear.alpha() / 255.f              //
    );

    auto device = MTLCreateSystemDefaultDevice();
    if (!device) {
      NSLog(@"Failed to create metal device");
      return false;
    }

    auto renderCommandQueue = [device newCommandQueue];
    if (!renderCommandQueue) {
      NSLog(@"Failed to create render commandQueue");
      return false;
    }

    auto updateCommandQueue = [device newCommandQueue];
    if (!updateCommandQueue) {
      NSLog(@"Failed to create update commandQueue");
      return false;
    }

    NSError* error = nil;
    auto library = [device newLibraryWithSource:[NSString stringWithUTF8String:shaderSource] options:nil error:&error];
    if (error) {
      NSLog(@"Failed to compile shader: %@", error);
      return false;
    }

    // Retrieve the vertexShader from the library
    auto vertexFunction = [library newFunctionWithName:@"vertexShader"];
    if (!vertexFunction) {
      NSLog(@"Failed to retrieve `vertexShader` shader");
      return false;
    }

    // Retrieve the fragmentShader from the library
    auto fragmentFunction = [library newFunctionWithName:@"fragmentShader"];
    if (!fragmentFunction) {
      NSLog(@"Failed to retrieve `fragmentShader` shader");
      return false;
    }

    auto pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

    error = nil;
    auto pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
    if (error) {
      NSLog(@"Failed to create pipeline state: %@", error);
      return false;
    }

    auto metalLayer = [CAMetalLayer layer];
    if (!metalLayer) {
      NSLog(@"Failed to create metal layer");
      return false;
    }
    metalLayer.device = device;
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.framebufferOnly = YES;
    metalLayer.drawableSize = size;
    metalLayer.backgroundColor =
      CGColorCreateGenericRGB(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);

    [view setLayer:metalLayer];
    [view setWantsLayer:YES];

    m_pImpl->m_view = view;
    m_pImpl->m_device = device;
    m_pImpl->m_renderQueue = renderCommandQueue;
    m_pImpl->m_updateQueue = updateCommandQueue;
    m_pImpl->m_vertexFunction = vertexFunction;
    m_pImpl->m_fragmentFunction = fragmentFunction;
    m_pImpl->m_pipelineState = pipelineState;
    m_pImpl->m_metalLayer = metalLayer;

    return true;
  }
}

void
MetalRenderBackend::uninitialize()
{
  @autoreleasepool {
    m_pImpl->m_metalLayer = nil;
    m_pImpl->m_pipelineState = nil;
    m_pImpl->m_updateQueue = nil;
    m_pImpl->m_renderQueue = nil;
    m_pImpl->m_fragmentFunction = nil;
    m_pImpl->m_vertexFunction = nil;
    m_pImpl->m_device = nil;

    m_pImpl->m_cefViewTexture = nil;
    m_pImpl->m_cefViewTextureDesc = nil;
    m_pImpl->m_cefViewVertexBuffer = nil;
    m_pImpl->m_cefPopupTexture = nil;
    m_pImpl->m_cefPopupTextureDesc = nil;
    m_pImpl->m_cefPopupVertexBuffer = nil;

    m_pImpl->m_showPopup = false;
    m_pImpl->m_popupRect = CGRectZero;

    m_pImpl->m_view = nil;
  }
}

void
MetalRenderBackend::resize(int width, int height, float scale)
{
  if (CefCurrentlyOn(TID_UI)) {
    // resize resource
    m_pImpl->m_metalLayer.drawableSize = CGSizeMake(width * scale, height * scale);
    // render immediately
    render();
  } else {
    // perform the resize in render thread
    QPointer<MetalRenderBackend> self = this;
    CefPostTask(TID_UI, new RenderTask([=]() {
                  if (self) {
                    // resize resource
                    m_pImpl->m_metalLayer.drawableSize = CGSizeMake(width * scale, height * scale);
                    // render immediately
                    self->render();
                  }
                }));
  }
}

void
MetalRenderBackend::updatePopupVisibility(bool visible)
{
  m_pImpl->m_showPopup = visible;
}

void
MetalRenderBackend::updatePopupRect(const CefRect& rect)
{
  m_pImpl->m_popupRect = CGRectMake(rect.x, rect.y, rect.width, rect.height);
}

void
MetalRenderBackend::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                    const CefRenderHandler::RectList& dirtyRects,
                                    const FrameDataType& dataType,
                                    const FrameData& data)
{
  // we only process Texture data
  if (dataType != FrameDataType::GpuTexture) {
    return;
  }

  id<MTLTexture> pSharedTexture;
  MTLTextureDescriptor* sharedTextureDesc;
  @autoreleasepool {
    IOSurfaceRef ioSurface = (IOSurfaceRef)(data.texture.handle);
    // create shared texture
    sharedTextureDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
                                                                           width:IOSurfaceGetWidth(ioSurface)
                                                                          height:IOSurfaceGetHeight(ioSurface)
                                                                       mipmapped:NO];
    pSharedTexture = [m_pImpl->m_device newTextureWithDescriptor:sharedTextureDesc iosurface:ioSurface plane:0];
  }

  // update
  if ((int)CefRenderHandler::PaintElementType::PET_VIEW == type) {
    m_pImpl->m_cefViewTexture = pSharedTexture;
    m_pImpl->m_cefViewTextureDesc = sharedTextureDesc;
  } else if ((int)CefRenderHandler::PaintElementType::PET_POPUP == type) {
    m_pImpl->m_cefPopupTexture = pSharedTexture;
    m_pImpl->m_cefPopupTextureDesc = sharedTextureDesc;
  } else {
    return;
  }

  render();
}

void
MetalRenderBackend::render()
{
  @autoreleasepool {
    auto size = widgetSize();
    auto scale = widgetScale();
    auto clear = widgetBackground();
    auto clearColor = MTLClearColorMake( //
      clear.red() / 255.f,               //
      clear.green() / 255.f,             //
      clear.blue() / 255.f,              //
      clear.alpha() / 255.f              //
    );

    auto drawable = [m_pImpl->m_metalLayer nextDrawable];
    if (!drawable) {
      return;
    }

    auto passDescriptor = [[MTLRenderPassDescriptor alloc] init];
    passDescriptor.colorAttachments[0].texture = drawable.texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = clearColor;

    auto commandBuffer = [m_pImpl->m_renderQueue commandBuffer];
    auto renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    auto width = size.width() * scale;
    auto height = size.height() * scale;
    [renderEncoder setViewport:(MTLViewport){ 0.0, 0.0, width, height, -1.0, 1.0 }];
    [renderEncoder setRenderPipelineState:m_pImpl->m_pipelineState];

    // draw view
    m_pImpl->m_cefViewVertexBuffer = m_pImpl->createQuadVertexBuffer( //
      0,                                                              //
      0,                                                              //
      m_pImpl->m_cefViewTextureDesc.width,                            //
      m_pImpl->m_cefViewTextureDesc.height,                           //
      width,                                                          //
      height                                                          //
    );
    [renderEncoder setVertexBuffer:m_pImpl->m_cefViewVertexBuffer offset:0 atIndex:0];
    [renderEncoder setFragmentTexture:m_pImpl->m_cefViewTexture atIndex:0];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];

    // draw popup
    if (m_pImpl->m_showPopup) {
      m_pImpl->m_cefPopupVertexBuffer = m_pImpl->createQuadVertexBuffer( //
        m_pImpl->m_popupRect.origin.x * scale,                           //
        m_pImpl->m_popupRect.origin.y * scale,                           //
        m_pImpl->m_cefPopupTextureDesc.width,                            //
        m_pImpl->m_cefPopupTextureDesc.height,                           //
        width,                                                           //
        height                                                           //
      );

      [renderEncoder setVertexBuffer:m_pImpl->m_cefPopupVertexBuffer offset:0 atIndex:0];
      [renderEncoder setFragmentTexture:m_pImpl->m_cefPopupTexture atIndex:0];
      [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
    }

    [renderEncoder endEncoding];
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
  }
}
