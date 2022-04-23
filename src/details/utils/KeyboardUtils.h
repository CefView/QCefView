#if defined(CEF_USE_OSR)
#pragma once
#include <QKeyEvent>

uint32_t
GetPlatformKeyboardCode(QKeyEvent* event);

uint32_t
GetPlatformKeyboardModifiers(QKeyEvent* event);

#endif
