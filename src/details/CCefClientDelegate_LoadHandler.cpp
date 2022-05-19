#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->q_ptr->loadingStateChanged(isLoading, canGoBack, canGoForward, browser->GetIdentifier());
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int transition_type)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->q_ptr->loadStart(browser->GetIdentifier(), frame->GetIdentifier(), frame->IsMain(), transition_type);
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int httpStatusCode)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->q_ptr->loadEnd(httpStatusCode, browser->GetIdentifier(), frame->GetIdentifier(), frame->IsMain());
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              CefRefPtr<CefFrame>& frame,
                              int errorCode,
                              const std::string& errorMsg,
                              const std::string& failedUrl,
                              bool& handled)
{
  if (!IsValidBrowser(browser))
    return;

  auto msg = QString::fromStdString(errorMsg);
  auto url = QString::fromStdString(failedUrl);
  pCefViewPrivate_->q_ptr->loadError(errorCode, msg, url, handled, browser->GetIdentifier(), frame->GetIdentifier(), frame->IsMain());
}
