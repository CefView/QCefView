#ifndef CEFVIEWRENDERERFACTORY_H
#define CEFVIEWRENDERERFACTORY_H

#pragma once
#include "ICefViewRenderer.h"

namespace CefViewRendererFactory {

CefViewRendererPtr
createRenderer(bool hardware);
}
#endif
