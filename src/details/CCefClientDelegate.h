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
  QCefViewPrivate* pCefViewPrivate_;

  CefSize viewSize_;

public:
  CCefClientDelegate(QCefViewPrivate* p);

  virtual void processUrlRequest(const std::string& url) override;

  virtual void processQueryRequest(CefRefPtr<CefBrowser>& browser,
                                   int64_t frameId,
                                   const std::string& query,
                                   const int64_t query_id) override;

  virtual void focusedEditableNodeChanged(CefRefPtr<CefBrowser>& browser,
                                          int64_t frameId,
                                          bool focusOnEditableNode) override;

  virtual void invokeMethodNotify(CefRefPtr<CefBrowser>& browser,
                                  int64_t frameId,
                                  const std::string& method,
                                  const CefRefPtr<CefListValue>& arguments) override;
  virtual void reportJSResult(CefRefPtr<CefBrowser>& browser,
                              int64_t frameId,
                              int64_t contextId,
                              const CefRefPtr<CefValue>& result) override;

  // LifSpan handler
  virtual bool onBeforPopup(CefRefPtr<CefBrowser>& browser,
                            int64_t frameId,
                            const std::string& targetUrl,
                            const std::string& targetFrameName,
                            CefLifeSpanHandler::WindowOpenDisposition targetDisposition,
                            CefWindowInfo& windowInfo,
                            CefBrowserSettings& settings,
                            bool& DisableJavascriptAccess) override;
  virtual void onAfterCreate(CefRefPtr<CefBrowser>& browser) override;
  virtual bool doClose(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // Load handler
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

  // Display handler
  virtual void draggableRegionChanged(CefRefPtr<CefBrowser>& browser,
                                      const std::vector<CefDraggableRegion>& regions) override;
  virtual void addressChanged(CefRefPtr<CefBrowser>& browser, int64_t frameId, const std::string& url) override;
  virtual void titleChanged(CefRefPtr<CefBrowser>& browser, const std::string& title) override;
  virtual void fullscreenModeChanged(CefRefPtr<CefBrowser>& browser, bool fullscreen) override;
  virtual bool tooltipMessage(CefRefPtr<CefBrowser>& browser, const std::string& text) override;
  virtual void statusMessage(CefRefPtr<CefBrowser>& browser, const std::string& value) override;
  virtual void consoleMessage(CefRefPtr<CefBrowser>& browser, const std::string& message, int level) override;
  virtual void loadingProgressChanged(CefRefPtr<CefBrowser>& browser, double progress) override;
  virtual bool cursorChanged(CefRefPtr<CefBrowser> browser,
                             CefCursorHandle cursor,
                             cef_cursor_type_t type,
                             const CefCursorInfo& custom_cursor_info) override;

  virtual bool onPreKeyEvent(CefRefPtr<CefBrowser> browser,
                             const CefKeyEvent& event,
                             CefEventHandle os_event,
                             bool* is_keyboard_shortcut) override;
  virtual bool onKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event) override;

  // Focus handler
  virtual void takeFocus(CefRefPtr<CefBrowser>& browser, bool next) override;
  virtual bool setFocus(CefRefPtr<CefBrowser>& browser) override;
  virtual void gotFocus(CefRefPtr<CefBrowser>& browser) override;

  // Render handler
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
  virtual void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                  CefRenderHandler::PaintElementType type,
                                  const CefRenderHandler::RectList& dirtyRects,
                                  void* shared_handle) override;
  virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefDragData> drag_data,
                             CefRenderHandler::DragOperationsMask allowed_ops,
                             int x,
                             int y) override;
  virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation) override;
  virtual void OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y) override;
  virtual void OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
                                            const CefRange& selected_range,
                                            const CefRenderHandler::RectList& character_bounds) override;
  virtual void OnTextSelectionChanged(CefRefPtr<CefBrowser> browser,
                                      const CefString& selected_text,
                                      const CefRange& selected_range) override;
  virtual void OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
                                          CefRenderHandler::TextInputMode input_mode) override;
};
