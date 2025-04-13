#include "MetalRenderBackend.h"

#include <CoreFoundation/CoreFoundation.h>
#include <MacTypes.h>
#include <simd/simd.h>

#import <Cocoa/Cocoa.h>
#import <CoreGraphics/CGColor.h>
#import <IOSurface/IOSurface.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

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

@interface MetalRenderer : NSObject

- (bool)initialize:(NSView*)view
          andWidth:(int)width
         andHeight:(int)height
          andScale:(float)scale
     andBackground:(MTLClearColor)background;

- (void)uninitialize;

- (void)resize:(CGSize)size withScale:(float)scale;

- (void)updatePopupVisibility:(Boolean)visible;

- (void)updatePopupRect:(CGRect)rect;

- (void)updateFrameData:(IOSurfaceRef)ioSurface withType:(int)type andColor:(MTLPixelFormat)format;

- (void)render;

@end

@implementation MetalRenderer {
  id<MTLDevice> m_device;
  id<MTLCommandQueue> m_renderQueue;
  id<MTLCommandQueue> m_updateQueue;
  id<MTLRenderPipelineState> m_pipelineState;
  id<MTLFunction> m_vertexFunction;
  id<MTLFunction> m_fragmentFunction;

  CAMetalLayer* m_metalLayer;
  __weak NSView* m_view;

  float m_scale;
  CGSize m_size;

  MTLTextureDescriptor* m_lastViewTextureDesc;
  id<MTLBuffer> m_cefViewVertexBuffer;
  id<MTLTexture> m_cefViewTexture;

  MTLTextureDescriptor* m_lastPopupTextureDesc;
  id<MTLBuffer> m_cefPopupVertexBuffer;
  id<MTLTexture> m_cefPopupTexture;

  bool m_showPopup;
  CGRect m_popupRect;
  MTLClearColor m_backgroundColor;
}

#define ScreenToNDC(xx, yy)                                                                                            \
  (2.0f * (xx) / (float)(m_size.width) - 1.0f), (1.0f - 2.0f * (yy) / (float)(m_size.height)), 0.0f, 1.0f

- (id<MTLBuffer>)createQuadVertexBuffer:(float)x Y:(float)y W:(float)w H:(float)h
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

- (void)copyTextureResourceTo:(__strong id<MTLTexture>&)dstTexture
          withDestTextureDesc:(__strong MTLTextureDescriptor*&)dstTextureDesc
               fromSrcTexture:(id<MTLTexture>)srcTexture
              withTextureDesc:(MTLTextureDescriptor*)srcTextureDesc;
{
  if (!dstTexture                                         //
      || dstTexture.pixelFormat != srcTexture.pixelFormat //
      || dstTexture.width != srcTexture.width             //
      || dstTexture.height != srcTexture.height           //
  ) {
    dstTextureDesc = srcTextureDesc;
    dstTexture = [m_device newTextureWithDescriptor:srcTextureDesc];
  }

  // copy the texture
  // id<MTLCommandQueue> commandQueue = [m_device newCommandQueue];
  id<MTLCommandBuffer> commandBuffer = [m_updateQueue commandBuffer];
  id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
  [blitEncoder copyFromTexture:srcTexture
                   sourceSlice:0
                   sourceLevel:0
                  sourceOrigin:(MTLOrigin){ 0, 0, 0 }
                    sourceSize:(MTLSize){ srcTexture.width, srcTexture.height, 1 }
                     toTexture:dstTexture
              destinationSlice:0
              destinationLevel:0
             destinationOrigin:(MTLOrigin){ 0, 0, 0 }];
  [blitEncoder endEncoding];
  [commandBuffer commit];
}

- (bool)initialize:(NSView*)view
          andWidth:(int)width
         andHeight:(int)height
          andScale:(float)scale
     andBackground:(MTLClearColor)background
{
  m_view = view;
  m_scale = scale;
  m_size.width = width;
  m_size.height = height;
  m_backgroundColor = background;

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
  id<MTLLibrary> library = [device newLibraryWithSource:[NSString stringWithUTF8String:shaderSource]
                                                options:nil
                                                  error:&error];
  if (error) {
    NSLog(@"Failed to compile shader: %@", error);
    return false;
  }

  // Retrieve the vertexShader from the library
  id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertexShader"];
  if (!vertexFunction) {
    NSLog(@"Failed to retrieve `vertexShader` shader");
    return false;
  }

  // Retrieve the fragmentShader from the library
  id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragmentShader"];
  if (!fragmentFunction) {
    NSLog(@"Failed to retrieve `fragmentShader` shader");
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
  metalLayer.drawableSize = CGSizeMake(width, height);
  metalLayer.backgroundColor =
    CGColorCreateGenericRGB(background.red, background.green, background.blue, background.alpha);

  MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDescriptor.vertexFunction = vertexFunction;
  pipelineDescriptor.fragmentFunction = fragmentFunction;
  pipelineDescriptor.colorAttachments[0].pixelFormat = metalLayer.pixelFormat;

  error = nil;
  auto pipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
  if (error) {
    NSLog(@"Failed to create pipeline state: %@", error);
    return false;
  }

  [view setWantsLayer:YES];
  [view setLayer:metalLayer];

  m_device = device;
  m_renderQueue = renderCommandQueue;
  m_updateQueue = updateCommandQueue;
  m_vertexFunction = vertexFunction;
  m_fragmentFunction = fragmentFunction;
  m_metalLayer = metalLayer;
  m_pipelineState = pipelineState;

  m_cefViewVertexBuffer = [self createQuadVertexBuffer:0 Y:0 W:width H:height];

  return true;
}

- (void)uninitialize
{
  m_metalLayer = nil;
  m_pipelineState = nil;
  m_updateQueue = nil;
  m_renderQueue = nil;
  m_fragmentFunction = nil;
  m_vertexFunction = nil;
  m_device = nil;
  m_device = nil;
}

- (void)resize:(CGSize)size withScale:(float)scale
{
  // update size
  m_scale = scale;
  m_size.width = size.width * scale;
  m_size.height = size.height * scale;

  m_metalLayer.drawableSize = m_size;
}

- (void)updatePopupVisibility:(Boolean)visible
{
  m_showPopup = visible;
}

- (void)updatePopupRect:(CGRect)rect
{
  CGRect newRect = rect;
  newRect.origin.x = newRect.origin.x * m_scale;
  newRect.origin.y = newRect.origin.y * m_scale;
  newRect.size.width = newRect.size.width * m_scale;
  newRect.size.height = newRect.size.height * m_scale;

  if (CGRectEqualToRect(newRect, m_popupRect)) {
    return;
  }

  // update popup rect
  m_popupRect = newRect;

  m_cefPopupVertexBuffer = [self createQuadVertexBuffer:m_popupRect.origin.x
                                                      Y:m_popupRect.origin.y
                                                      W:m_popupRect.size.width
                                                      H:m_popupRect.size.height];
}

- (void)updateFrameData:(IOSurfaceRef)ioSurface withType:(int)type andColor:(MTLPixelFormat)format;
{
  // create shared texture
  MTLTextureDescriptor* sharedTextureDesc =
    [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm
                                                       width:IOSurfaceGetWidth(ioSurface)
                                                      height:IOSurfaceGetHeight(ioSurface)
                                                   mipmapped:NO];
  id<MTLTexture> sharedTexture = [m_device newTextureWithDescriptor:sharedTextureDesc iosurface:ioSurface plane:0];

  // update
  if ((int)CefRenderHandler::PaintElementType::PET_VIEW == type) {
    [self copyTextureResourceTo:m_cefViewTexture
            withDestTextureDesc:m_lastViewTextureDesc
                 fromSrcTexture:sharedTexture
                withTextureDesc:sharedTextureDesc];
  } else if ((int)CefRenderHandler::PaintElementType::PET_POPUP == type) {
    [self copyTextureResourceTo:m_cefPopupTexture
            withDestTextureDesc:m_lastPopupTextureDesc
                 fromSrcTexture:sharedTexture
                withTextureDesc:sharedTextureDesc];
  } else {
    return;
  }
}

- (void)render
{
  id<CAMetalDrawable> drawable = [m_metalLayer nextDrawable];
  if (!drawable) {
    return;
  }

  m_view.layer = m_metalLayer;
  m_view.wantsLayer = TRUE;

  MTLRenderPassDescriptor* passDescriptor = [[MTLRenderPassDescriptor alloc] init];
  passDescriptor.colorAttachments[0].texture = drawable.texture;
  passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
  passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
  passDescriptor.colorAttachments[0].clearColor = m_backgroundColor;

  id<MTLCommandBuffer> commandBuffer = [m_renderQueue commandBuffer];
  id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
  [renderEncoder setViewport:(MTLViewport){ 0.0, 0.0, m_size.width, m_size.height, -1.0, 1.0 }];
  [renderEncoder setRenderPipelineState:m_pipelineState];

  // draw view
  [renderEncoder setVertexBuffer:m_cefViewVertexBuffer offset:0 atIndex:0];
  [renderEncoder setFragmentTexture:m_cefViewTexture atIndex:0];
  [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];

  // draw popup
  if (m_showPopup) {
    [renderEncoder setVertexBuffer:m_cefPopupVertexBuffer offset:0 atIndex:0];
    [renderEncoder setFragmentTexture:m_cefPopupTexture atIndex:0];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangleStrip vertexStart:0 vertexCount:4];
  }

  [renderEncoder endEncoding];
  [commandBuffer presentDrawable:drawable];
  [commandBuffer commit];
}

@end

class MetalRenderBackend::Implementation
{
public:
  MetalRenderer* renderer;
};

MetalRenderBackend::MetalRenderBackend()
  : m_pImpl(std::make_unique<Implementation>())
{
  @autoreleasepool {
    m_pImpl->renderer = [MetalRenderer alloc];
  }
}

MetalRenderBackend::~MetalRenderBackend()
{
  @autoreleasepool {
    m_pImpl.reset();
  }
}

bool
MetalRenderBackend::initialize(void* wid, int width, int height, float scale, const CefColor& background)
{
  if (!m_pImpl->renderer) {
    return false;
  }

  MTLClearColor bgColor = MTLClearColorMake( //
    CefColorGetR(background),                //
    CefColorGetG(background),                //
    CefColorGetB(background),                //
    CefColorGetA(background)                 //
  );

  @autoreleasepool {
    return [m_pImpl->renderer initialize:(__bridge NSView*)wid
                                andWidth:width
                               andHeight:height
                                andScale:scale
                           andBackground:bgColor];
  }
}

void
MetalRenderBackend::uninitialize()
{
  @autoreleasepool {
    [m_pImpl->renderer uninitialize];
  }
}

void
MetalRenderBackend::resize(int width, int height, float scale)
{
  CGSize sz = CGSizeMake(width, height);
  @autoreleasepool {
    [m_pImpl->renderer resize:sz withScale:scale];
  }
}

void
MetalRenderBackend::updatePopupVisibility(bool visible)
{
  @autoreleasepool {
    [m_pImpl->renderer updatePopupVisibility:visible];
  }
}

void
MetalRenderBackend::updatePopupRect(const CefRect& rect)
{
  CGRect rc = CGRectMake(rect.x, rect.y, rect.width, rect.height);
  @autoreleasepool {
    [m_pImpl->renderer updatePopupRect:rc];
  }
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

  @autoreleasepool {
    [m_pImpl->renderer updateFrameData:(IOSurfaceRef)(data.texture.handle)
                              withType:type
                              andColor:MTLPixelFormatBGRA8Unorm];
  }
}

void
MetalRenderBackend::render(void* painter)
{
  @autoreleasepool {
    [m_pImpl->renderer render];
  }
}
