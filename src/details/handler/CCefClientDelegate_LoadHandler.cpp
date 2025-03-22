#include "details/CCefClientDelegate.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  if (!IsValidBrowser(browser))
    return;

  emit pCefViewPrivate_->q_ptr->loadingStateChanged(browser->GetIdentifier(), isLoading, canGoBack, canGoForward);
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int transitionType)
{
  if (!IsValidBrowser(browser))
    return;

  emit pCefViewPrivate_->q_ptr->loadStart(
    browser->GetIdentifier(), ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), frame->IsMain(), transitionType);
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int httpStatusCode)
{
  if (!IsValidBrowser(browser))
    return;

  // workaround for:
  // https://github.com/chromiumembedded/cef/issues/3870
  // after navigation CEF resets the browser focus status
  // without any callback notification (AKA, released the
  // focus silently), so we need to update the CEF browser
  // focus status according to the one we have kept
  if (true                                     //
      && pCefViewPrivate_->isOSRModeEnabled_   //
      && pCefViewPrivate_->osr.hasCefGotFocus_ //
      && browser->GetHost()                    //
  ) {
    browser->GetHost()->SetFocus(true);
  }

  emit pCefViewPrivate_->q_ptr->loadEnd(
    browser->GetIdentifier(), ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), frame->IsMain(), httpStatusCode);
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              CefRefPtr<CefFrame>& frame,
                              int errorCode,
                              const CefString& errorMsg,
                              const CefString& failedUrl,
                              bool& handled)
{
  if (!IsValidBrowser(browser))
    return;

  handled = pCefViewPrivate_->handleLoadError(browser, frame, errorCode, errorMsg, failedUrl);
}
