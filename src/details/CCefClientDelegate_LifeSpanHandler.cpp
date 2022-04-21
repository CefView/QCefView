#include "CCefClientDelegate.h"

#include <QThread>

#include "QCefSettingPrivate.h"
#include "QCefViewPrivate.h"

bool
CCefClientDelegate::onBeforPopup(CefRefPtr<CefBrowser>& browser,
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
    QCefView::WindowOpenDisposition d = (QCefView::WindowOpenDisposition)targetDisposition;
    QCefSettingPrivate::CopyFromCefBrowserSettings(&s, &settings);

    Qt::ConnectionType connType = pCefViewPrivate_->q_ptr->thread() == QThread::currentThread()
                                    ? Qt::DirectConnection
                                    : Qt::BlockingQueuedConnection;
    QMetaObject::invokeMethod(pCefViewPrivate_->q_ptr,
                              "onBeforPopup",                            //
                              connType,                                  //
                              Q_RETURN_ARG(bool, result),                //
                              Q_ARG(int64_t, frameId),                   //
                              Q_ARG(const QString&, url),                //
                              Q_ARG(const QString&, name),               //
                              Q_ARG(QCefView::WindowOpenDisposition, d), //
                              Q_ARG(QCefSetting&, s),                    //
                              Q_ARG(bool&, DisableJavascriptAccess)      //
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

  // create QWindow from native browser window handle
  QWindow* w = QWindow::fromWinId((WId)(browser->GetHost()->GetWindowHandle()));
  Qt::ConnectionType c = Qt::DirectConnection;
  if (pCefViewPrivate_->q_ptr->thread() != QThread::currentThread()) {
    // move window to QCefView thread
    w->moveToThread(pCefViewPrivate_->q_ptr->thread());

    // change connection type
#if defined(CEF_USE_OSR)
    c = Qt::BlockingQueuedConnection;
#else
    c = Qt::QueuedConnection;
#endif
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
