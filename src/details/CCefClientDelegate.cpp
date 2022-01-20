#include "CCefClientDelegate.h"

#include "ValueConvertor.h"

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
  QVariantList args;
  for (int i = 0; i < arguments->GetSize(); i++) {
    QVariant qV;
    auto cV = arguments->GetValue(i);
    ValueConvertor::CefValueToQVariant(&qV, cV);
    args.push_back(qV);
  }

  auto browserId = browser->GetIdentifier();
  view->invokeMethod(browserId, frameId, m, args);
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
