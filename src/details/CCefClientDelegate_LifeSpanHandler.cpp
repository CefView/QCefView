#include "CCefClientDelegate.h"

#include <QThread>

#include "QCefSettingPrivate.h"
#include "QCefViewPrivate.h"

bool
CCefClientDelegate::onBeforePopup(CefRefPtr<CefBrowser>& browser,
                                  int64_t frameId,
                                  const std::string& targetUrl,
                                  const std::string& targetFrameName,
                                  CefLifeSpanHandler::WindowOpenDisposition targetDisposition,
                                  CefWindowInfo& windowInfo,
                                  CefBrowserSettings& settings,
                                  bool& disableJavascriptAccess)
{
  bool cancelPopup = true;

  if (!pCefViewPrivate_)
    return cancelPopup;

  // determine the connection type
  Qt::ConnectionType c =
    pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection : Qt::BlockingQueuedConnection;

  // invoke the method
  QMetaObject::invokeMethod(
    pCefViewPrivate_,
    [&]() {
      cancelPopup = pCefViewPrivate_->onBeforeCefPopupBrowserCreate(browser,                //
                                                                    frameId,                //
                                                                    targetUrl,              //
                                                                    targetFrameName,        //
                                                                    targetDisposition,      //
                                                                    windowInfo,             //
                                                                    settings,               //
                                                                    disableJavascriptAccess //
      );
    },
    c);

  return cancelPopup;
}

void
CCefClientDelegate::onAfterCreate(CefRefPtr<CefBrowser>& browser)
{
  if (!pCefViewPrivate_)
    return;

  QWindow* w = nullptr;
  // #if !defined(CEF_USE_OSR)
  if (!pCefViewPrivate_->isOSRModeEnabled() || browser->IsPopup()) {
    // create QWindow from native browser window handle
    w = QWindow::fromWinId((WId)(browser->GetHost()->GetWindowHandle()));
  }
  // #endif

  Qt::ConnectionType c = Qt::DirectConnection;
  if (pCefViewPrivate_->q_ptr->thread() != QThread::currentThread()) {
    // change connection type
    // #if !defined(CEF_USE_OSR)
    if (!pCefViewPrivate_->isOSRModeEnabled() || browser->IsPopup()) {
      c = Qt::QueuedConnection;
    } else {
      // #else
      c = Qt::BlockingQueuedConnection;
    }
    // #endif

    // move window to main thread
    if (w != nullptr) {
      w->moveToThread(pCefViewPrivate_->q_ptr->thread());
    }
  }

  if (browser->IsPopup()) {
    QMetaObject::invokeMethod(
      pCefViewPrivate_, [=]() { pCefViewPrivate_->onCefPopupBrowserCreated(browser, w); }, c);
  } else {
    QMetaObject::invokeMethod(
      pCefViewPrivate_, [=]() { pCefViewPrivate_->onCefBrowserCreated(browser, w); }, c);
  }
}

bool
CCefClientDelegate::doClose(CefRefPtr<CefBrowser> browser)
{
  return false;
}

void
CCefClientDelegate::onBeforeClose(CefRefPtr<CefBrowser> browser)
{
  return;
}
