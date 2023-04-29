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
  bool result = false;
  if (pCefViewPrivate_) {
    auto url = QString::fromStdString(targetUrl);
    auto name = QString::fromStdString(targetFrameName);

    QCefSetting s;
    QCefView::CefWindowOpenDisposition d = (QCefView::CefWindowOpenDisposition)targetDisposition;
    QCefSettingPrivate::CopyFromCefBrowserSettings(&s, &settings);

    Qt::ConnectionType c = pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection
                                                                                         : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(pCefViewPrivate_->q_ptr,
                              "onBeforePopup",                              //
                              c,                                            //
                              Q_RETURN_ARG(bool, result),                   //
                              Q_ARG(qint64, frameId),                       //
                              Q_ARG(const QString&, url),                   //
                              Q_ARG(const QString&, name),                  //
                              Q_ARG(QCefView::CefWindowOpenDisposition, d), //
                              Q_ARG(QCefSetting&, s),                       //
                              Q_ARG(bool&, DisableJavascriptAccess)         //
    );
    QCefSettingPrivate::CopyToCefBrowserSettings(&s, &settings);
  }
  return result;
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
      CefRefPtr<CefBrowser> b(browser);
      if (b->IsPopup())
        pCefViewPrivate_->onCefPopupBrowserCreated(b, w);
      else
        pCefViewPrivate_->onCefMainBrowserCreated(b, w);
    },
    c);
}

bool
CCefClientDelegate::doClose(CefRefPtr<CefBrowser> browser)
{
  return false;
}

void
CCefClientDelegate::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  return;
}
