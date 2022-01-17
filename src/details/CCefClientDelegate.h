#pragma once

#pragma region std_headers
#include <string>
#include <unordered_map>
#include <vector>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#include <CefViewBrowserClientDelegate.h>

#include <QCefQuery.h>

class QCefView;

class CCefClientDelegate : public CefViewBrowserClientDelegateInterface
{
public:
  CCefClientDelegate();

  void insertBrowserViewMapping(CefRefPtr<CefBrowser>& browser, QCefView* view);

  void removeBrowserViewMapping(CefRefPtr<CefBrowser>& browser);

  virtual void loadingStateChanged(CefRefPtr<CefBrowser>& browser,
                                   bool isLoading,
                                   bool canGoBack,
                                   bool canGoForward) override;

  virtual void loadStart(CefRefPtr<CefBrowser>& browser) override;

  virtual void loadEnd(CefRefPtr<CefBrowser>& browser, int httpStatusCode) override;

  virtual void loadError(CefRefPtr<CefBrowser>& browser,
                         int errorCode,
                         const std::string& errorMsg,
                         const std::string& failedUrl,
                         bool& handled) override;

  virtual void draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                      const std::vector<CefDraggableRegion>& regions) override;

  virtual void consoleMessage(CefRefPtr<CefBrowser>& browser, const std::string& message, int level) override;

  virtual void takeFocus(CefRefPtr<CefBrowser>& browser, bool next) override;

  virtual void processUrlRequest(const std::string& url) override;

  virtual void processQueryRequest(CefRefPtr<CefBrowser>& browser,
                                   const std::string& query,
                                   const int64_t query_id) override;

  virtual void invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                  int frameId,
                                  const std::string& method,
                                  const CefRefPtr<CefListValue>& arguments) override;

protected:
  QCefView* take(CefRefPtr<CefBrowser>& browser);

private:
  std::unordered_map<int, QCefView*> view_map_;
};
