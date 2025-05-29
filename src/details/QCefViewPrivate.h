#pragma once
#pragma region qt_headers
#include <QMenu>
#include <QMutex>
#include <QPointer>
#include <QScopedPointer>
#include <QString>
#include <QStringList>

#if defined(QT_DEBUG)
#include <QElapsedTimer>
#endif
#pragma endregion

#include <CefViewBrowserClient.h>
#include <CefViewBrowserClientDelegate.h>

#include "CCefClientDelegate.h"
#include "QCefContextPrivate.h"
#include "QCefWindow.h"
#include "render/CefViewRendererFactory.h"
#include "utils/MenuBuilder.h"
#include "utils/ValueConvertor.h"

#include <QCefQuery.h>
#include <QCefView.h>

class QCefViewPrivate : public QObject
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(QCefView)
  QCefView* q_ptr;

  friend class CCefClientDelegate;

private:
  static QSet<QCefViewPrivate*> sLiveInstances;

public:
  /// <summary>
  ///
  /// </summary>
  static void destroyAllInstance();

  /// <summary>
  ///
  /// </summary>
  bool allowDrag_ = false;

  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate* pContextPrivate_ = nullptr;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserClient> pClient_ = nullptr;

  /// <summary>
  ///
  /// </summary>
  CCefClientDelegate::RefPtr pClientDelegate_ = nullptr;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefBrowser> pCefBrowser_ = nullptr;

  /// <summary>
  ///
  /// </summary>
  bool isOSRModeEnabled_ = false;

  /// <summary>
  /// Off-screen rendering private data
  /// </summary>
  struct OsrPrivateData
  {
    /// <summary>
    /// workaround for:
    /// https://github.com/chromiumembedded/cef/issues/3870
    /// after navigation CEF resets the browser focus status
    /// without any callback notification (AKA, released the
    /// focus silently), so we need to keep the CEF browser
    /// focus status by ourself, and at every loadEnd callback
    /// we need to sync the status to CEF browser.
    /// </summary>
    bool hasCefGotFocus_ = false;

    // allowed drag operations
    int32_t allowedDragOperations_;

    // IME parameters
    QRect imeCursorRect_;

    // context menu status and parameters
    bool isShowingContextMenu_ = false;
    QMenu* contextMenu_ = nullptr;
    CefRefPtr<CefRunContextMenuCallback> contextMenuCallback_;

    // OSR renderer
    CefViewRendererPtr pRenderer_;
  } osr;

  /// <summary>
  /// Native child window private data
  /// </summary>
  struct NcwPrivateData
  {
    /// <summary>
    ///
    /// </summary>
    QCefWindow* qBrowserWindow_ = nullptr;

    /// <summary>
    ///
    /// </summary>
    QWidget* qBrowserWidget_ = nullptr;
  } ncw;

  /// <summary>
  /// The last visited URL
  /// </summary>
  CefString lastUrl_;

#if defined(QT_DEBUG)
  QElapsedTimer paintTimer_;
#endif

public:
  explicit QCefViewPrivate(QCefContextPrivate* ctx, QCefView* view);

  ~QCefViewPrivate();

  void createCefBrowser(const QString& url, const QCefSettingPrivate* setting);

  void destroyCefBrowser();

  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);

  void addArchiveResource(const QString& path, const QString& url, const QString& password = "", int priority = 0);

  void setCefWindowFocus(bool focus);

  QCefQuery createQuery(const QString& req, const int64_t id);

protected:
  void onCefBrowserCreated(CefRefPtr<CefBrowser> browser, QWindow* window);

  bool onBeforeNewBrowserCreate(const QCefFrameId& sourceFrameId,
                                const QString& targetUrl,
                                const QString& targetFrameName,
                                QCefView::CefWindowOpenDisposition targetDisposition,
                                QRect rect,
                                QCefSetting settings);

  bool onBeforeNewPopupCreate(const QCefFrameId& sourceFrameId,
                              const QString& targetUrl,
                              QString& targetFrameName,
                              QCefView::CefWindowOpenDisposition targetDisposition,
                              QRect& rect,
                              QCefSetting& settings,
                              bool& disableJavascriptAccess);

  void onAfterCefPopupCreated(CefRefPtr<CefBrowser> browser);

  void onNewDownloadItem(QSharedPointer<QCefDownloadItem> item, const QString& suggestedName);

  void onUpdateDownloadItem(QSharedPointer<QCefDownloadItem> item);

  bool handleLoadError(CefRefPtr<CefBrowser>& browser,
                       CefRefPtr<CefFrame>& frame,
                       int errorCode,
                       const std::string& errorMsg,
                       const std::string& failedUrl);

  bool requestCloseFromWeb(CefRefPtr<CefBrowser>& browser);

  qreal scaleFactor();

public slots:
  void onAppFocusChanged(QWidget* old, QWidget* now);

  void onViewScreenChanged(QScreen* screen);

  void onCefWindowLostTabFocus(bool next);

  void onCefWindowGotFocus();

  void onCefUpdateCursor(const QCursor& cursor);

  void onCefInputStateChanged(bool editable);

  void onOsrImeCursorRectChanged(const QRect& rc);

  void onContextMenuTriggered(QAction* action);

  void onContextMenuDestroyed(QObject* obj);

  void onStartDragging(CefRefPtr<CefDragData>& dragData, CefRenderHandler::DragOperationsMask allowedOps);

protected:
  void onBeforeCefContextMenu(const MenuBuilder::MenuData& data);

  void onRunCefContextMenu(QPoint pos, CefRefPtr<CefRunContextMenuCallback> callback);

  void onCefContextMenuDismissed();

  void onFileDialog(CefBrowserHost::FileDialogMode mode,
                    const QString& title,
                    const QString& default_file_path,
                    const QStringList& accept_filters,
#if CEF_VERSION_MAJOR < 102
                    int selected_accept_filter,
#endif
                    CefRefPtr<CefFileDialogCallback> callback);

  bool hasDevTools();

  void showDevTools();

  void closeDevTools();

  bool shouldAllowDrop(CefRefPtr<CefDragData>& dragData, CefDragHandler::DragOperationsMask mask);

  bool shouldAllowDrag(CefRefPtr<CefDragData>& dragData, CefRenderHandler::DragOperationsMask allowedOps, int x, int y);

  void updateDragOperation(CefRenderHandler::DragOperationsMask allowedOps);

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;

  QVariant onViewInputMethodQuery(Qt::InputMethodQuery query) const;

  void onViewInputMethodEvent(QInputMethodEvent* event);

  void onViewVisibilityChanged(bool visible);

  void onViewFocusChanged(bool focused);

  void onPaintEvent(QPaintEvent* event);

  void onViewMoved();

  void onViewSizeChanged(const QSize& size, const QSize& oldSize);

  void onViewKeyEvent(QKeyEvent* event);

  void onViewMouseEvent(QMouseEvent* event);

  void onViewWheelEvent(QWheelEvent* event);

  void onContextMenuEvent(const QPoint& pos);

  void onDragEnter(QDragEnterEvent* event);

  void onDragMove(QDragMoveEvent* event);

  void onDragLeave(QDragLeaveEvent* event);

  void onDrop(QDropEvent* event);

public:
  int browserId();

  void navigateToString(const QString& content);

  void navigateToUrl(const QString& url);

  bool browserCanGoBack();

  bool browserCanGoForward();

  void browserGoBack();

  void browserGoForward();

  bool browserIsLoading();

  void browserReload();

  void browserStopLoad();

  bool triggerEvent(const QString& name, const QVariantList& args, const QCefFrameId& frameId = QCefView::MainFrameID);

  bool responseQCefQuery(const QCefQuery& query);

  bool responseQCefQuery(const int64_t query, bool success, const QString& response, int error);

  bool executeJavascript(const QCefFrameId& frameId, const QString& code, const QString& url);

  bool executeJavascriptWithResult(const QCefFrameId& frameId,
                                   const QString& code,
                                   const QString& url,
                                   const QString& context);

  void notifyMoveOrResizeStarted();

  bool sendEventNotifyMessage(const QCefFrameId& frameId, const QString& name, const QVariantList& args);

  bool setPreference(const QString& name, const QVariant& value, const QString& error);

  void setOSRFrameRate(int fps);
};
