#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_))
    pCefViewPrivate_->q_ptr->loadingStateChanged(isLoading, canGoBack, canGoForward);
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser)
{
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_))
    pCefViewPrivate_->q_ptr->loadStart();
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, int httpStatusCode)
{
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_))
    pCefViewPrivate_->q_ptr->loadEnd(httpStatusCode);
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              int errorCode,
                              const std::string& errorMsg,
                              const std::string& failedUrl,
                              bool& handled)
{
  if (pCefViewPrivate_ && browser->IsSame(pCefViewPrivate_->pCefBrowser_)) {
    auto msg = QString::fromStdString(errorMsg);
    auto url = QString::fromStdString(failedUrl);
    pCefViewPrivate_->q_ptr->loadError(errorCode, msg, url, handled);
  }
}
