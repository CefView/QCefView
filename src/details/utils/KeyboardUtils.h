#if defined(CEF_USE_OSR)
#pragma once
#include <QKeyEvent>

quint32
QKeyEventToWindowsVirtualKey(QKeyEvent* ke);

quint32
QKeyEventToMacOSVirtualKey(QKeyEvent* ke);

uint32_t
GetPlatformKeyboardModifiers(QKeyEvent* event);

#endif
