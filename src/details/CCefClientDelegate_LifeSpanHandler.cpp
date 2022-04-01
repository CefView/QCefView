#include "CCefClientDelegate.h"

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

    QMetaObject::invokeMethod(pCefViewPrivate_->q_ptr,
                              "onBeforPopup",                            //
                              Qt::BlockingQueuedConnection,              //
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

  if (browser->IsPopup()) {
    QWindow* wnd = QWindow::fromWinId((WId)(browser->GetHost()->GetWindowHandle()));
    QMetaObject::invokeMethod(pCefViewPrivate_->q_ptr,      //
                              "onPopupCreated",             //
                              Qt::BlockingQueuedConnection, //
                              Q_ARG(QWindow*, wnd));
  } else {
    pCefViewPrivate_->onCefBrowserCreated(browser);

    // QMetaObject::invokeMethod(
    //  pCefViewPrivate_,
    //  [=]() {
    //    CefRefPtr<CefBrowser> b(browser);
    //    pCefViewPrivate_->onCefBrowserCreated(b);
    //  },
    //  Qt::BlockingQueuedConnection);
  }
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
