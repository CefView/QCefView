#include "CCefClientDelegate.h"

#include "QCefView.h"

CCefClientDelegate::CCefClientDelegate() {}

void
CCefClientDelegate::insertBrowserViewMapping(CefRefPtr<CefBrowser>& browser, QCefView* view)
{
  auto id = browser->GetIdentifier();
  view_map_[id] = view;
}

void
CCefClientDelegate::removeBrowserViewMapping(CefRefPtr<CefBrowser>& browser)
{
  auto id = browser->GetIdentifier();
  view_map_.erase(id);
}

void
CCefClientDelegate::loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                        bool isLoading,
                                        bool canGoBack,
                                        bool canGoForward)
{
  auto view = take(browser);
  if (view)
    view->loadingStateChanged(isLoading, canGoBack, canGoForward);
}

void
CCefClientDelegate::loadStart(CefRefPtr<CefBrowser>& browser)
{
  auto view = take(browser);
  if (view)
    view->loadStart();
}

void
CCefClientDelegate::loadEnd(CefRefPtr<CefBrowser>& browser, int httpStatusCode)
{
  auto view = take(browser);
  if (view)
    view->loadEnd(httpStatusCode);
}

void
CCefClientDelegate::loadError(CefRefPtr<CefBrowser>& browser,
                              int errorCode,
                              const std::string& errorMsg,
                              const std::string& failedUrl,
                              bool& handled)
{
  auto view = take(browser);
  if (view) {
    auto msg = QString::fromStdString(errorMsg);
    auto url = QString::fromStdString(failedUrl);
    view->loadError(errorCode, msg, url, handled);
  }
}

void
CCefClientDelegate::draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                           const std::vector<CefDraggableRegion>& regions)
{
  auto view = take(browser);
  if (!view)
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

  view->draggableRegionChanged(draggableRegion, nonDraggableRegion);
}

void
CCefClientDelegate::consoleMessage(CefRefPtr<CefBrowser>& browser, const std::string& message, int level)
{
  auto view = take(browser);
  if (view) {
    auto msg = QString::fromStdString(message);
    view->consoleMessage(msg, level);
  }
}

void
CCefClientDelegate::takeFocus(CefRefPtr<CefBrowser>& browser, bool next)
{
  auto view = take(browser);
  if (view)
    view->takeFocus(next);
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
                                        const std::string& request,
                                        const int64_t query_id)
{
  auto view = take(browser);
  if (view) {
    auto req = QString::fromStdString(request);
    view->cefQueryRequest(QCefQuery(req, query_id));
  }
}

void
CCefClientDelegate::invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                       int frameId,
                                       const std::string& method,
                                       const CefRefPtr<CefListValue>& arguments)
{
  auto view = take(browser);
  if (!view)
    return;

  auto m = QString::fromStdString(method);

  QVariantList argumentList;
  QString qStr;
  for (int idx = 0; idx < arguments->GetSize(); idx++) {
    if (CefValueType::VTYPE_NULL == arguments->GetType(idx))
      argumentList.push_back(0);
    else if (CefValueType::VTYPE_BOOL == arguments->GetType(idx))
      argumentList.push_back(QVariant::fromValue(arguments->GetBool(idx)));
    else if (CefValueType::VTYPE_INT == arguments->GetType(idx))
      argumentList.push_back(QVariant::fromValue(arguments->GetInt(idx)));
    else if (CefValueType::VTYPE_DOUBLE == arguments->GetType(idx))
      argumentList.push_back(QVariant::fromValue(arguments->GetDouble(idx)));
    else if (CefValueType::VTYPE_STRING == arguments->GetType(idx)) {
#if defined(CEF_STRING_TYPE_UTF16)
      qStr = QString::fromUtf16((char16_t*)arguments->GetString(idx).c_str());
#elif defined(CEF_STRING_TYPE_UTF8)
      qStr = QString::fromUtf8(arguments->GetString(idx).c_str());
#elif defined(CEF_STRING_TYPE_WIDE)
      qStr = QString::fromWCharArray(arguments->GetString(idx).c_str());
#endif
      argumentList.push_back(qStr);
    } else {
      // invalid type
    }
  }

  auto browserId = browser->GetIdentifier();
  view->invokeMethod(browserId, frameId, m, argumentList);
}

QCefView*
CCefClientDelegate::take(CefRefPtr<CefBrowser>& browser)
{
  if (!browser)
    return nullptr;
  auto it = view_map_.find(browser->GetIdentifier());
  if (it == view_map_.end())
    return nullptr;

  return it->second;
}
