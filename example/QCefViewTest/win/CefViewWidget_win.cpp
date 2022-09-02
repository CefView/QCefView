#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

#include <windows.h>

HRGN
CreateRoundRectRgnEx(int width,
                     int height, //
                     int x1,
                     int y1, //
                     int x2,
                     int y2, //
                     int x3,
                     int y3, //
                     int x4,
                     int y4)
{
  // create main rect region
  HRGN result = ::CreateRectRgn(0, 0, width, height);

  union _TRIAGNEL
  {
    POINT p[3];
    struct
    {
      POINT a;
      POINT b;
      POINT c;
    };
  } triangle;

  // top left
  triangle.a = { 0, 0 };
  triangle.b = { x1, 0 };
  triangle.c = { 0, y1 };
  HRGN tlTriangle = ::CreatePolygonRgn(triangle.p, _countof(triangle.p), ALTERNATE);
  HRGN tlCorner = ::CreateEllipticRgn(0, 0, x1 * 2, y1 * 2);
  ::CombineRgn(tlCorner, tlTriangle, tlCorner, RGN_DIFF);
  ::CombineRgn(result, result, tlCorner, RGN_DIFF);

  // top right
  triangle.a = { width - x2, 0 };
  triangle.b = { width, 0 };
  triangle.c = { width, y2 };
  HRGN trTriangle = ::CreatePolygonRgn(triangle.p, _countof(triangle.p), ALTERNATE);
  HRGN trCorner = ::CreateEllipticRgn(width - x2 * 2, 0, width, 2 * y2);
  ::CombineRgn(trCorner, trTriangle, trCorner, RGN_DIFF);
  ::CombineRgn(result, result, trCorner, RGN_DIFF);

  // bottom right
  triangle.a = { width, height - y3 };
  triangle.b = { width, height };
  triangle.c = { width - x3, height };
  HRGN brTriangle = ::CreatePolygonRgn(triangle.p, _countof(triangle.p), ALTERNATE);
  HRGN brCorner = ::CreateEllipticRgn(width - x3 * 2, height - y3 * 2, width, height);
  ::CombineRgn(brCorner, brTriangle, brCorner, RGN_DIFF);
  ::CombineRgn(result, result, brCorner, RGN_DIFF);

  // bottom left
  triangle.a = { x4, height };
  triangle.b = { 0, height };
  triangle.c = { 0, height - y4 };
  HRGN blTriangle = ::CreatePolygonRgn(triangle.p, _countof(triangle.p), ALTERNATE);
  HRGN blCorner = ::CreateEllipticRgn(0, height - y4 * 2, x4 * 2, height);
  ::CombineRgn(blCorner, blTriangle, blCorner, RGN_DIFF);
  ::CombineRgn(result, result, blCorner, RGN_DIFF);

  return result;
}

void
CefViewWidget::onScreenChanged(QScreen* screen)
{
  if (!m_cefWindow)
    return;

  // initialize the region with rounded-corners for CEF window
  ::SetWindowRgn((HWND)(m_cefWindow->winId()),
                 ::CreateRoundRectRgnEx(size().width() * screen->devicePixelRatio(),
                                        size().height() * screen->devicePixelRatio(),
                                        m_cornerRadius * screen->devicePixelRatio(),
                                        m_cornerRadius * screen->devicePixelRatio(),
                                        0,
                                        0,
                                        m_cornerRadius * screen->devicePixelRatio(),
                                        m_cornerRadius * screen->devicePixelRatio(),
                                        0,
                                        0),
                 TRUE);
}

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_cefWindow = win;
  if (!m_cefWindow)
    return;

  // initialize the region with rounded-corners for CEF window
  ::SetWindowRgn((HWND)(m_cefWindow->winId()),
                 ::CreateRoundRectRgnEx(size().width() * devicePixelRatio(),
                                        size().height() * devicePixelRatio(),
                                        m_cornerRadius * devicePixelRatio(),
                                        m_cornerRadius * devicePixelRatio(),
                                        0,
                                        0,
                                        m_cornerRadius * devicePixelRatio(),
                                        m_cornerRadius * devicePixelRatio(),
                                        0,
                                        0),
                 TRUE);

  connect(window()->windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);

  // update the region with rounded-corners for CEF window
  if (m_cefWindow) {
    ::SetWindowRgn((HWND)(m_cefWindow->winId()),
                   ::CreateRoundRectRgnEx(size().width() * devicePixelRatio(),
                                          size().height() * devicePixelRatio(),
                                          m_cornerRadius * devicePixelRatio(),
                                          m_cornerRadius * devicePixelRatio(),
                                          0,
                                          0,
                                          m_cornerRadius * devicePixelRatio(),
                                          m_cornerRadius * devicePixelRatio(),
                                          0,
                                          0),
                   TRUE);
  }
}
