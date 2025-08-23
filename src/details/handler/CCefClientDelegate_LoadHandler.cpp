#include "details/CCefClientDelegate.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread([=]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->loadingStateChanged(browser->GetIdentifier(), isLoading, canGoBack, canGoForward);
    }
  });
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int transitionType)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread([=]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->loadStart(
        browser->GetIdentifier(), ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), frame->IsMain(), transitionType);
    }
  });
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame, int httpStatusCode)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThread([=]() {
    if (pCefViewPrivate->q_ptr) {
      emit pCefViewPrivate->q_ptr->loadEnd(
        browser->GetIdentifier(), ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), frame->IsMain(), httpStatusCode);
    }
  });
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              CefRefPtr<CefFrame>& frame,
                              int errorCode,
                              const CefString& errorMsg,
                              const CefString& failedUrl,
                              bool& handled)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  runInMainThreadAndWait([&]() {
    if (pCefViewPrivate->q_ptr) {
      handled = pCefViewPrivate->handleLoadError(browser, frame, errorCode, errorMsg, failedUrl);
    }
  });
}
