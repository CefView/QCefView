#include "CCefClientDelegate.h"

#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut)
{
  return false;
}

bool
CCefClientDelegate::onKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
  return false;
}
