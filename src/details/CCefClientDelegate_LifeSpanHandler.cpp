#include "CCefClientDelegate.h"

#include <QThread>

#include "QCefSettingPrivate.h"
#include "QCefViewPrivate.h"

#define DEFAULT_POPUP_WIDTH 800
#define DEFAULT_POPUP_HEIGHT 600

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
  bool cancel = true;
  if (!pCefViewPrivate_) {
    return cancel;
  }

  auto url = QString::fromStdString(targetUrl);
  auto name = QString::fromStdString(targetFrameName);
  auto d = (QCefView::CefWindowOpenDisposition)targetDisposition;
  auto rc = QRect(windowInfo.bounds.x, windowInfo.bounds.y, windowInfo.bounds.width, windowInfo.bounds.height);

  if (rc.width() <= 0) {
    rc.setWidth(DEFAULT_POPUP_WIDTH);
  }

  if (rc.height() <= 0) {
    rc.setHeight(DEFAULT_POPUP_HEIGHT);
  }

  QCefSetting s;
  QCefSettingPrivate::CopyFromCefBrowserSettings(&s, &settings);

  if (targetDisposition == CefLifeSpanHandler::WindowOpenDisposition::WOD_NEW_POPUP) {
    Qt::ConnectionType c = pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection
                                                                                         : Qt::BlockingQueuedConnection;

    QMetaObject::invokeMethod(
      pCefViewPrivate_,
      [&]() {
        cancel = pCefViewPrivate_->onBeforeNewPopupCreate(frameId, //
                                                          url,     //
                                                          name,    //
                                                          d,       //
                                                          rc,      //
                                                          s,       //
                                                          disableJavascriptAccess);
        if (!cancel) {
          QCefSettingPrivate::CopyToCefBrowserSettings(&s, &settings);
          CefString(&windowInfo.window_name) = name.toStdString();
          windowInfo.bounds = { rc.x(), rc.y(), rc.width(), rc.height() };
        }
      },
      c);
  } else {
    cancel = true;
    QMetaObject::invokeMethod(
      pCefViewPrivate_,
      [=]() {
        pCefViewPrivate_->onBeforeNewBrowserCreate(frameId, //
                                                   url,     //
                                                   name,    //
                                                   d,       //
                                                   rc,      //
                                                   s);
      },
      Qt::QueuedConnection);
  }

  return cancel;
}

void
CCefClientDelegate::onAfterCreate(CefRefPtr<CefBrowser>& browser)
{
  if (!pCefViewPrivate_)
    return;

  QWindow* w = nullptr;

  if (!pCefViewPrivate_->isOSRModeEnabled() /*|| browser->IsPopup()*/) {
    // create QWindow from native browser window handle
    w = QWindow::fromWinId((WId)(browser->GetHost()->GetWindowHandle()));
  }

  Qt::ConnectionType c = Qt::DirectConnection;
  if (pCefViewPrivate_->q_ptr->thread() != QThread::currentThread()) {
    // change connection type
    if (!pCefViewPrivate_->isOSRModeEnabled() /*|| browser->IsPopup()*/) {
      c = Qt::QueuedConnection;
    } else {
      // OSR mode
      c = Qt::BlockingQueuedConnection;
    }

    // move window to main thread
    if (w != nullptr) {
      w->moveToThread(pCefViewPrivate_->q_ptr->thread());
    }
  }

  if (browser->IsPopup()) {
    QMetaObject::invokeMethod(
      pCefViewPrivate_, [=]() { pCefViewPrivate_->onAfterCefPopupCreated(browser); }, c);
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
}
