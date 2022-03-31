#include "CCefClientDelegate.h"

#include "QCefSettingPrivate.h"
#include "QCefViewPrivate.h"
#include "ValueConvertor.h"

CCefClientDelegate::CCefClientDelegate(QCefViewPrivate* p)
  : pQCefViewPrivate_(p)
{}

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
  if (pQCefViewPrivate_) {
    auto url = QString::fromStdString(targetUrl);
    auto name = QString::fromStdString(targetFrameName);

    QCefSetting s;
    QCefSettingPrivate::CopyFromCefBrowserSettings(&s, &settings);
    result = pQCefViewPrivate_->q_ptr->onBeforPopup(
      frameId, url, name, static_cast<QCefView::WindowOpenDisposition>(targetDisposition), s, DisableJavascriptAccess);
    QCefSettingPrivate::CopyToCefBrowserSettings(&s, &settings);
  }
  return result;
}

void
CCefClientDelegate::onAfterCreate(CefRefPtr<CefBrowser>& browser)
{
  if (browser->IsPopup()) {
  } else {
    if (pQCefViewPrivate_) {
      QMetaObject::invokeMethod(pQCefViewPrivate_, [=]() {
        CefRefPtr<CefBrowser> b(browser);
        pQCefViewPrivate_->cefBrowserCreated(b);
      });
    }
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

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  if (pQCefViewPrivate_)
    pQCefViewPrivate_->q_ptr->loadingStateChanged(isLoading, canGoBack, canGoForward);
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser)
{
  if (pQCefViewPrivate_)
    pQCefViewPrivate_->q_ptr->loadStart();
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, int httpStatusCode)
{
  if (pQCefViewPrivate_)
    pQCefViewPrivate_->q_ptr->loadEnd(httpStatusCode);
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              int errorCode,
                              const std::string& errorMsg,
                              const std::string& failedUrl,
                              bool& handled)
{
  if (pQCefViewPrivate_) {
    auto msg = QString::fromStdString(errorMsg);
    auto url = QString::fromStdString(failedUrl);
    pQCefViewPrivate_->q_ptr->loadError(errorCode, msg, url, handled);
  }
}

void
CCefClientDelegate::draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                           const std::vector<CefDraggableRegion>& regions)
{
  if (!pQCefViewPrivate_)
    return;

  // Determine new draggable region.
  QRegion draggableRegion;
  QRegion nonDraggableRegion;
  std::vector<CefDraggableRegion>::const_iterator it = regions.begin();
  for (; it != regions.end(); ++it) {
    if (it->draggable) {
      draggableRegion += QRegion(it->bounds.x, it->bounds.y, it->bounds.width, it->bounds.height);
    } else {
      nonDraggableRegion += QRegion(it->bounds.x, it->bounds.y, it->bounds.width, it->bounds.height);
    }
  }

  pQCefViewPrivate_->q_ptr->draggableRegionChanged(draggableRegion, nonDraggableRegion);
}

void
CCefClientDelegate::addressChanged(CefRefPtr<CefBrowser>& browser, int64_t frameId, const std::string& url)
{
  if (pQCefViewPrivate_) {
    auto u = QString::fromStdString(url);
    pQCefViewPrivate_->q_ptr->addressChanged(frameId, u);
  }
}

void
CCefClientDelegate::titleChanged(CefRefPtr<CefBrowser>& browser, const std::string& title)
{
  if (pQCefViewPrivate_) {
    auto t = QString::fromStdString(title);
    pQCefViewPrivate_->q_ptr->titleChanged(t);
  }
}

void
CCefClientDelegate::fullscreenModeChanged(CefRefPtr<CefBrowser>& browser, bool fullscreen)
{
  if (pQCefViewPrivate_) {
    pQCefViewPrivate_->q_ptr->fullscreenModeChanged(fullscreen);
  }
}

bool
CCefClientDelegate::tooltipMessage(CefRefPtr<CefBrowser>& browser, const std::string& text)
{
  // allow the tooltip action
  return false;
}

void
CCefClientDelegate::statusMessage(CefRefPtr<CefBrowser>& browser, const std::string& value)
{
  if (pQCefViewPrivate_) {
    auto msg = QString::fromStdString(value);
    pQCefViewPrivate_->q_ptr->statusMessage(msg);
  }
}

void
CCefClientDelegate::consoleMessage(CefRefPtr<CefBrowser>& browser, const std::string& message, int level)
{
  if (pQCefViewPrivate_) {
    auto msg = QString::fromStdString(message);
    pQCefViewPrivate_->q_ptr->consoleMessage(msg, level);
  }
}

void
CCefClientDelegate::loadingProgressChanged(CefRefPtr<CefBrowser>& browser, double progress)
{
  if (pQCefViewPrivate_) {
    pQCefViewPrivate_->q_ptr->loadingProgressChanged(progress);
  }
}

bool
CCefClientDelegate::cursorChanged(CefRefPtr<CefBrowser> browser,
                                  CefCursorHandle cursor,
                                  cef_cursor_type_t type,
                                  const CefCursorInfo& custom_cursor_info)
{
  // allow the cursor changing action
  return false;
}

void
CCefClientDelegate::takeFocus(CefRefPtr<CefBrowser>& browser, bool next)
{
  if (pQCefViewPrivate_) {
    QMetaObject::invokeMethod(pQCefViewPrivate_, [=]() { pQCefViewPrivate_->onCefWindowLostTabFocus(next); });
  }
}

bool
CCefClientDelegate::setFocus(CefRefPtr<CefBrowser>& browser)
{
  // allow the focus setting action
  return false;
}

void
CCefClientDelegate::gotFocus(CefRefPtr<CefBrowser>& browser)
{
  if (pQCefViewPrivate_) {
    QMetaObject::invokeMethod(pQCefViewPrivate_, [=]() { pQCefViewPrivate_->onCefWindowGotFocus(); });
  }
}

void
CCefClientDelegate::processUrlRequest(const std::string& url)
{
  // deprecated feature
  // auto view = take(browser);
  // if (view) {
  //  auto u = QString::fromStdString(url);
  //  view->onQCefUrlRequest(u);
  //}
}

void
CCefClientDelegate::processQueryRequest(CefRefPtr<CefBrowser>& browser,
                                        int64_t frameId,
                                        const std::string& request,
                                        const int64_t query_id)
{
  if (pQCefViewPrivate_) {
    auto browserId = browser->GetIdentifier();
    auto req = QString::fromStdString(request);
    pQCefViewPrivate_->q_ptr->cefQueryRequest(browserId, frameId, QCefQuery(req, query_id));
  }
}

void
CCefClientDelegate::invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                       int64_t frameId,
                                       const std::string& method,
                                       const CefRefPtr<CefListValue>& arguments)
{
  if (!pQCefViewPrivate_)
    return;

  auto m = QString::fromStdString(method);
  QVariantList args;
  for (int i = 0; i < arguments->GetSize(); i++) {
    QVariant qV;
    auto cV = arguments->GetValue(i);
    ValueConvertor::CefValueToQVariant(&qV, cV.get());
    args.push_back(qV);
  }

  auto browserId = browser->GetIdentifier();
  pQCefViewPrivate_->q_ptr->invokeMethod(browserId, frameId, m, args);
}

void
CCefClientDelegate::reportJSResult(CefRefPtr<CefBrowser>& browser,
                                   int64_t frameId,
                                   int64_t contextId,
                                   const CefRefPtr<CefValue>& result)
{
  if (!pQCefViewPrivate_)
    return;

  auto browserId = browser->GetIdentifier();
  QVariant qV;
  ValueConvertor::CefValueToQVariant(&qV, result.get());
  pQCefViewPrivate_->q_ptr->reportJavascriptResult(browserId, frameId, contextId, qV);
}

bool
CCefClientDelegate::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  return false;
}

void
CCefClientDelegate::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{}

bool
CCefClientDelegate::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  return false;
}

bool
CCefClientDelegate::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
  return false;
}

void
CCefClientDelegate::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{}

void
CCefClientDelegate::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{}

void
CCefClientDelegate::OnPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{}
