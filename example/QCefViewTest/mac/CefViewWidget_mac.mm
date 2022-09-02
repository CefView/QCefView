#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

#import <Cocoa/Cocoa.h>

void
CefViewWidget::onScreenChanged(QScreen* screen)
{

}

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_cefWindow = win;
  if (!m_cefWindow)
    return;

  // set layer with rounded corners
  @autoreleasepool {
    NSView* view = (__bridge NSView*)(m_cefWindow->winId());
    view.wantsLayer = YES;
    view.layer.masksToBounds = YES;
    view.layer.cornerRadius = m_cornerRadius;
  }
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);

  // no need to update for rounded corners since it is a persistent configuration
}
