#include "details/CCefClientDelegate.h"

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#endif

#include <QDebug>
#include <QScreen>
#include <QThread>

#include "details/QCefSettingPrivate.h"
#include "details/QCefViewPrivate.h"

#define DEFAULT_POPUP_WIDTH 800
#define DEFAULT_POPUP_HEIGHT 600

bool
CCefClientDelegate::onBeforePopup(CefRefPtr<CefBrowser>& browser,
                                  CefRefPtr<CefFrame>& frame,
                                  const CefString& targetUrl,
                                  const CefString& targetFrameName,
                                  CefLifeSpanHandler::WindowOpenDisposition targetDisposition,
                                  CefWindowInfo& windowInfo,
                                  CefBrowserSettings& settings,
                                  bool& disableJavascriptAccess)
{
  auto pCefViewPrivate = pCefViewPrivate_.lock();
  if (!pCefViewPrivate) {
    return true;
  }

  bool cancel = true;

  auto url = QString::fromStdString(targetUrl);
  auto name = QString::fromStdString(targetFrameName);
  auto d = (QCefView::CefWindowOpenDisposition)targetDisposition;
#if CEF_VERSION_MAJOR > 95
  auto rc = QRect(windowInfo.bounds.x, windowInfo.bounds.y, windowInfo.bounds.width, windowInfo.bounds.height);
#else
  auto rc = QRect(windowInfo.x, windowInfo.y, windowInfo.width, windowInfo.height);
#endif

  if (rc.width() <= 0) {
    rc.setWidth(DEFAULT_POPUP_WIDTH);
  }

  if (rc.height() <= 0) {
    rc.setHeight(DEFAULT_POPUP_HEIGHT);
  }

  QCefSetting s;
  QCefSettingPrivate::CopyFromCefBrowserSettings(&s, &settings);

#if CEF_VERSION_MAJOR < 119
  auto CefNewPopupValue = CefLifeSpanHandler::WindowOpenDisposition::WOD_NEW_POPUP;
#else
  auto CefNewPopupValue = CefLifeSpanHandler::WindowOpenDisposition::CEF_WOD_NEW_POPUP;
#endif

  if (targetDisposition == CefNewPopupValue) {
    // the new browser was created from javascript, we need to conform
    // the CEF pop-up browser lifecycle because CEF need to return the
    // new browser identity to javascript context

    // we need to get the user customized settings, thus we should wait for the result
    runInMainThreadAndWait([&]() {
      cancel = pCefViewPrivate->onBeforeNewPopupCreate(ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), //
                                                       url,                                                //
                                                       name,                                               //
                                                       d,                                                  //
                                                       rc,                                                 //
                                                       s,                                                  //
                                                       disableJavascriptAccess);
      if (!cancel) {
        QCefSettingPrivate::CopyToCefBrowserSettings(&s, &settings);
        CefString(&windowInfo.window_name) = name.toStdString();

#if CEF_VERSION_MAJOR > 95
        windowInfo.bounds = { rc.x(), rc.y(), rc.width(), rc.height() };
#else
        windowInfo.x = rc.x();
        windowInfo.y = rc.y();
        windowInfo.width = rc.width();
        windowInfo.height = rc.height();
#endif
      }
    });
  } else {
    // the new browser was created from non-javascript,
    // CEF doesn't need to get it's identity so we just
    // create a new browser instead

    cancel = true;
    runInMainThread( //
      [=]() {
        pCefViewPrivate->onBeforeNewBrowserCreate(ValueConvertor::FrameIdC2Q(frame->GetIdentifier()), //
                                                  url,                                                //
                                                  name,                                               //
                                                  d,                                                  //
                                                  rc,                                                 //
                                                  s);
      });
  }

  return cancel;
}

void
CCefClientDelegate::onAfterCreate(CefRefPtr<CefBrowser>& browser)
{
  auto pCefViewPrivate = pCefViewPrivate_.lock();
  if (!pCefViewPrivate) {
    return;
  }

  QWindow* w = nullptr;

  if (!pCefViewPrivate->isOSRModeEnabled_ && !browser->IsPopup()) {
    // NCW mode and not pop-up browser
    auto winHandle = browser->GetHost()->GetWindowHandle();

#if defined(Q_OS_WINDOWS)
    // get native window size
    RECT rect = { 0 };
    ::GetWindowRect(winHandle, &rect);
    auto width = rect.right - rect.left;
    auto height = rect.bottom - rect.top;

#if defined(DEBUG) || defined(_DEBUG)
    qDebug() << "Native CEF window size:" << width << ", " << height;
#endif

    // strange behavior: when we create QWindow from native window handle
    // the size will be modified by the QWindow, for example:
    // before the invocation of QWindow::fromWinId,
    // the window size is (1920 * 1080),but after the invocation,
    // the QWindow size becomes to (1280 * 720) = (1920 * 1080) / X.
    // which x = primary screen scale factor
    w = QWindow::fromWinId((WId)(winHandle));

#if defined(DEBUG) || defined(_DEBUG)
    qDebug() << "Before fix, QWindow size:" << w->size();
#endif

    // fix the window size
    if (width > 0 && height > 0) {
      w->resize(width, height);
    }

#if defined(DEBUG) || defined(_DEBUG)
    qDebug() << "Before fix, QWindow size:" << w->size();
#endif
#else
    w = QWindow::fromWinId((WId)(winHandle));
#endif
  }

  if (qApp->thread() != QThread::currentThread()) {
    // move window to main thread
    if (w != nullptr) {
      w->moveToThread(qApp->thread());
    }
  }

  // crucial!
  // must call the onCefBrowserCreated/onAfterCefPopupCreated in main thread
  // and wait for the return, because there will be a validation of
  // pCefView->pCefBrowser_ in all other callbacks
  if (browser->IsPopup()) {
    // pop-up window
    runInMainThreadAndWait([=]() { pCefViewPrivate->onAfterCefPopupCreated(browser); });
  } else {
    // new normal browser
    runInMainThreadAndWait([=]() { pCefViewPrivate->onCefBrowserCreated(browser, w); });
  }
}

bool
CCefClientDelegate::doClose(CefRefPtr<CefBrowser>& browser)
{
  qDebug() << "destroy browser from native";

  return false;
}

bool
CCefClientDelegate::requestClose(CefRefPtr<CefBrowser>& browser)
{
  qDebug() << "destroy browser request from web";

  auto pCefViewPrivate = pCefViewPrivate_.lock();
  if (!pCefViewPrivate) {
    return false;
  }

  bool ignoreClose = false;
  runInMainThreadAndWait([&]() { ignoreClose = !(pCefViewPrivate->requestCloseFromWeb(browser)); });
  return ignoreClose;
}

void
CCefClientDelegate::onBeforeClose(CefRefPtr<CefBrowser>& browser)
{
}
