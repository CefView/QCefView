#if defined(CEF_USE_OSR)
#pragma once
#include <QKeyEvent>

quint32
QtKeyToWindowsVirtualKey(int k);

quint32
QtKeyToMacOSVirtualKey(int k);

uint32_t
GetPlatformKeyboardModifiers(QKeyEvent* event);

#endif
