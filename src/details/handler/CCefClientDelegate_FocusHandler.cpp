#include "details/CCefClientDelegate.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::takeFocus(CefRefPtr<CefBrowser>& browser, bool next)
{
  if (!IsValidBrowser(browser))
    return;

  QMetaObject::invokeMethod(pCefViewPrivate_, "onCefWindowLostTabFocus", Q_ARG(bool, next));
}

bool
CCefClientDelegate::setFocus(CefRefPtr<CefBrowser>& browser)
{
  // allow the focus setting action
  return false;
}

void
CCefClientDelegate::gotFocus(CefRefPtr<CefBrowser>& browser)
{
  if (!IsValidBrowser(browser))
    return;

  QMetaObject::invokeMethod(pCefViewPrivate_, "onCefWindowGotFocus");
}
