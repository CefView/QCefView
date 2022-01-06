#include "CCefHandlerDelegate.h"

CCefHandlerDelegate::CCefHandlerDelegate(QCefView* view, QCefWindow* window)
  : view_(view)
  , window_(window)
{}

void
CCefHandlerDelegate::setBrowserWindowId(CefWindowHandle win)
{
  if (window_)
    window_->setBrowserWindowId(win);
}

void
CCefHandlerDelegate::loadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward)
{
  if (view_)
    view_->onLoadingStateChanged(isLoading, canGoBack, canGoForward);
}

void
CCefHandlerDelegate::loadStart()
{
  if (view_)
    view_->onLoadStart();
}

void
CCefHandlerDelegate::loadEnd(int httpStatusCode)
{
  if (view_)
    view_->onLoadEnd(httpStatusCode);
}

void
CCefHandlerDelegate::loadError(int errorCode, const std::string& errorMsg, const std::string& failedUrl, bool& handled)
{
  if (view_) {
    auto msg = QString::fromStdString(errorMsg);
    auto url = QString::fromStdString(failedUrl);
    view_->onLoadError(errorCode, msg, url, handled);
  }
}

void
CCefHandlerDelegate::draggableRegionChanged(const std::vector<CefDraggableRegion>& regions)
{
  if (!view_)
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

  view_->onDraggableRegionChanged(draggableRegion, nonDraggableRegion);
}

void
CCefHandlerDelegate::consoleMessage(const std::string& message, int level)
{
  if (view_) {
    auto msg = QString::fromStdString(message);
    view_->onConsoleMessage(msg, level);
  }
}

void
CCefHandlerDelegate::takeFocus(bool next)
{
  if (view_)
    view_->onTakeFocus(next);
}

void
CCefHandlerDelegate::processUrlRequest(const std::string& url)
{
  if (view_) {
    auto u = QString::fromStdString(url);
    view_->onQCefUrlRequest(u);
  }
}

void
CCefHandlerDelegate::processQueryRequest(const std::string& request, const int64_t query_id)
{
  if (view_) {
    auto req = QString::fromStdString(request);
    view_->onQCefQueryRequest(QCefQuery(req, query_id));
  }
}

void
CCefHandlerDelegate::invokeMethodNotify(int browserId,
                                        int frameId,
                                        const std::string& method,
                                        const CefRefPtr<CefListValue>& arguments)
{
  if (!view_)
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

  view_->onInvokeMethodNotify(browserId, frameId, m, argumentList);
}

void
CCefHandlerDelegate::browserIsDestroying()
{
  return;
}
