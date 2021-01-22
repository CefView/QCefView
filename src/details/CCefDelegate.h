#pragma once

#pragma region qt_headers
#include <QWindow>
#include <QPointer>
#include <QVariant>
#include <QHash>
#include <QMutex>
#include <QRegion>
#pragma endregion qt_headers

#pragma region std_headers
#include <memory>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#include <CefViewBrowserDelegate.h>

#include <QCefQuery.h>
#include <QCefView.h>

#include "QCefWindow.h"

class CCefDelegate
  : public CefViewBrowserDelegateInterface
  , public std::enable_shared_from_this<CCefDelegate>
{
public:
  explicit CCefDelegate(QCefView* view, QCefWindow* window);

  virtual void setCefBrowserWindowHandle(CefWindowHandle win) override;

  virtual void loadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward) override;

  virtual void loadStart() override;

  virtual void loadEnd(int httpStatusCode) override;

  virtual void loadError(int errorCode,
                         const std::string& errorMsg,
                         const std::string& failedUrl,
                         bool& handled) override;

  virtual void draggableRegionChanged(const std::vector<CefDraggableRegion>& regions) override;

  virtual void consoleMessage(const std::string& message, int level) override;

  virtual void takeFocus(bool next) override;

  virtual void processUrlRequest(const std::string& url) override;

  virtual void processQueryRequest(const std::string& query, const int64_t query_id) override;

  virtual void invokeMethodNotify(int browserId,
                                  int frameId,
                                  const std::string& method,
                                  const CefRefPtr<CefListValue>& arguments) override;

  virtual void browserIsDestroying() override;

private:
  QCefView* view_;
  QCefWindow* window_;
};
