#pragma once
#include "CommonUtils.h"

#if defined(Q_OS_WINDOWS)
#include <Windows.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
HRGN
QRectToHRGN(const QRect& rc)
{
  return ::CreateRectRgn(rc.left(), rc.top(), rc.right() + 1, rc.bottom() + 1);
}

HRGN
QRegionToHRGN(const QRegion& region)
{
  if (region.isNull() || region.rectCount() == 0) {
    return 0;
  }

  HRGN resultRgn = 0;
  QVector<QRect> rects = region.rects();
  resultRgn = QRectToHRGN(rects.at(0));
  const int size = rects.size();
  for (int i = 1; i < size; i++) {
    HRGN tmpRgn = QRectToHRGN(rects.at(i));
    int err = ::CombineRgn(resultRgn, resultRgn, tmpRgn, RGN_OR);
    if (err == ERROR)
      qWarning("Error combining HRGNs.");
    ::DeleteObject(tmpRgn);
  }
  return resultRgn;
}
#endif
#endif

#if defined(Q_OS_LINUX)
#include <X11/Xlib.h>
#include <qpa/qplatformnativeinterface.h>

Display*
X11GetDisplay(QWidget* widget)
{
  Q_ASSERT_X(widget, "X11GetDisplay", "Invalid parameter widget");
  if (!widget) {
    qWarning("Invalid parameter widget");
    return nullptr;
  }

  auto platformInterface = QApplication::platformNativeInterface();
  Q_ASSERT_X(platformInterface, "X11GetDisplay", "Failed to get platform native interface");
  if (!platformInterface) {
    qWarning("Failed to get platform native interface");
    return nullptr;
  }

  auto screen = widget->window()->windowHandle()->screen();
  Q_ASSERT_X(screen, "X11GetDisplay", "Failed to get screen");
  if (!screen) {
    qWarning("Failed to get screen");
    return nullptr;
  }

  return (Display*)platformInterface->nativeResourceForScreen("display", screen);
}
#endif

void
UpdateCefWindowMask(QWindow* w, const QRegion& r)
{
  if (w) {
    // this doesn't work
    // ncw.qBrowserWindow_->setMask(q->mask());
#if defined(Q_OS_WINDOWS)
    // for Windows
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    ::SetWindowRgn((HWND)w->winId(), QRegionToHRGN(r), true);
#else
    ::SetWindowRgn((HWND)w->winId(), r.toHRGN(), true);
#endif
#elif defined(Q_OS_MACOS)
    // for macOS
#elif defined(Q_OS_LINUX)
    // for Linux
#endif
  }
}

void
XRemapWindow(QWidget* widget, QWindow* window)
{
#if defined(Q_OS_LINUX)
  if (::XMapWindow(X11GetDisplay(widget), window->winId()) <= 0)
    qWarning() << "Failed to move input focus";
#endif
}
