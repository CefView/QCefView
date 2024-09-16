#ifndef CEFVIEWRENDERERFACTORY_H
#define CEFVIEWRENDERERFACTORY_H

#pragma once

#include <memory>

#include "ICefViewRenderer.h"

class CefViewRendererFactory
{
public:
  static std::shared_ptr<ICefViewRenderer> createRenderer(bool hardware);
};

#endif
