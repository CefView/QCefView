#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::takeFocus(CefRefPtr<CefBrowser>& browser, bool next)
{
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_)) {
    QMetaObject::invokeMethod(pCefViewPrivate_, "onCefWindowLostTabFocus", Q_ARG(bool, next));
  }
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
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_)) {
    QMetaObject::invokeMethod(pCefViewPrivate_, "onCefWindowGotFocus");
  }
}
