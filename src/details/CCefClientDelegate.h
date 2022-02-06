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

class QCefViewPrivate;

class CCefClientDelegate : public CefViewBrowserClientDelegateInterface
{
private:
  std::unordered_map<int, QCefViewPrivate*> view_map_;

public:
  CCefClientDelegate();

protected:
  QCefViewPrivate* take(CefRefPtr<CefBrowser>& browser);

public:
  void insertBrowserViewMapping(CefRefPtr<CefBrowser>& browser, QCefViewPrivate* view);

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

  virtual void addressChanged(CefRefPtr<CefBrowser>& browser, int frameId, const CefString& url) override;

  virtual void titleChanged(CefRefPtr<CefBrowser>& browser, const CefString& title) override;

  virtual void fullscreenModeChanged(CefRefPtr<CefBrowser>& browser, bool fullscreen) override;

  virtual bool tooltipMessage(CefRefPtr<CefBrowser>& browser, const CefString& text) override;

  virtual void statusMessage(CefRefPtr<CefBrowser>& browser, const CefString& value) override;

  virtual void consoleMessage(CefRefPtr<CefBrowser>& browser, const std::string& message, int level) override;

  virtual void loadingProgressChanged(CefRefPtr<CefBrowser>& browser, double progress) override;

  virtual bool cursorChanged(CefRefPtr<CefBrowser> browser,
                             CefCursorHandle cursor,
                             cef_cursor_type_t type,
                             const CefCursorInfo& custom_cursor_info) override;

  virtual void takeFocus(CefRefPtr<CefBrowser>& browser, bool next) override;

  virtual bool setFocus(CefRefPtr<CefBrowser>& browser) override;

  virtual void gotFocus(CefRefPtr<CefBrowser>& browser) override;

  virtual void processUrlRequest(const std::string& url) override;

  virtual void processQueryRequest(CefRefPtr<CefBrowser>& browser,
                                   int frameId,
                                   const std::string& query,
                                   const int64_t query_id) override;

  virtual void invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                  int frameId,
                                  const std::string& method,
                                  const CefRefPtr<CefListValue>& arguments) override;

  virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

  virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;

  virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY) override;

  virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info) override;

  virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;

  virtual void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) override;

  virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                       CefRenderHandler::PaintElementType type,
                       const CefRenderHandler::RectList& dirtyRects,
                       const void* buffer,
                       int width,
                       int height) override;
};
