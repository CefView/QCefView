#include "details/CCefClientDelegate.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::takeFocus(CefRefPtr<CefBrowser>& browser, bool next)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread(              //
    [pCefViewPrivate, next]() { //
      pCefViewPrivate->onCefWindowLostTabFocus(next);
    });
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread(        //
    [pCefViewPrivate]() { //
      pCefViewPrivate->onCefWindowGotFocus();
    });
}
