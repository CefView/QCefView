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
                                  bool& DisableJavascriptAccess)
{
  if (pCefViewPrivate_) {

    Qt::ConnectionType c =
      pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection : Qt::QueuedConnection;

    QMetaObject::invokeMethod(
      pCefViewPrivate_,
      [=]() {
        pCefViewPrivate_->onBeforeCefPopupCreate(
          browser, frameId, targetUrl, targetFrameName, targetDisposition, windowInfo, settings);
      },
      c);
  }

  // QCefView doesn't use CEF built-in popup browser
  return true;
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

  QMetaObject::invokeMethod(
    pCefViewPrivate_,
    [=]() {
      pCefViewPrivate_->onCefBrowserCreated(browser, w);
    },
    c);
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
