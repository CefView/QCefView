#pragma once
#pragma region qt_headers
#include <QMenu>
#include <QMutex>
#include <QPointer>
#include <QString>

#if defined(QT_DEBUG)
#include <QElapsedTimer>
#endif
#pragma endregion qt_headers

#include <CefViewBrowserClient.h>
#include <CefViewBrowserClientDelegate.h>

#include "CCefClientDelegate.h"
#include "QCefContextPrivate.h"
#include "QCefWindow.h"
#include "utils/MenuBuilder.h"

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
  bool disablePopupContextMenu_ = false;

  /// <summary>
  ///
  /// </summary>
  bool enableDragAndDrop_ = false;

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
  /// Offscreen rendering private data
  /// </summary>
  struct OsrPrivateData
  {
    /// <summary>
    ///
    /// </summary>
    bool transparentPaintingEnabled = false;

    /// <summary>
    ///
    /// </summary>
    bool showPopup_ = false;

    /// <summary>
    ///
    /// </summary>
    QRect qPopupRect_;

    /// <summary>
    ///
    /// </summary>
    QRect qImeCursorRect_;

    /// <summary>
    ///
    /// </summary>
    QMutex qViewPaintLock_;

    /// <summary>
    ///
    /// </summary>
    QImage qCefViewFrame_;

    /// <summary>
    ///
    /// </summary>
    QMutex qPopupPaintLock_;

    /// <summary>
    ///
    /// </summary>
    QImage qCefPopupFrame_;

    /// <summary>
    ///
    /// </summary>
    bool isShowingContextMenu_ = false;

    /// <summary>
    ///
    /// </summary>
    QMenu* contextMenu_ = nullptr;

    /// <summary>
    ///
    /// </summary>
    CefRefPtr<CefRunContextMenuCallback> contextMenuCallback_;
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
  explicit QCefViewPrivate(QCefContextPrivate* ctx,
                           QCefView* view,
                           const QString& url,
                           const QCefSetting* setting = nullptr);

  ~QCefViewPrivate();

  void createCefBrowser(QCefView* view, const QString& url, const QCefSetting* setting);

  void destroyCefBrowser();

  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);

  void addArchiveResource(const QString& path, const QString& url, const QString& password = "", int priority = 0);

  void setCefWindowFocus(bool focus);

  bool isOSRModeEnabled() const;

  QCefQuery createQuery(const QString& req, const int64_t id);

protected:
  void onCefBrowserCreated(CefRefPtr<CefBrowser> browser, QWindow* window);

  bool onBeforeNewBrowserCreate(qint64 sourceFrameId,
                                const QString& targetUrl,
                                const QString& targetFrameName,
                                QCefView::CefWindowOpenDisposition targetDisposition,
                                QRect rect,
                                QCefSetting settings);

  bool onBeforeNewPopupCreate(qint64 sourceFrameId,
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

public slots:
  void onAppFocusChanged(QWidget* old, QWidget* now);

  void onViewScreenChanged(QScreen* screen);

  void onCefWindowLostTabFocus(bool next);

  void onCefWindowGotFocus();

  void onCefUpdateCursor(const QCursor& cursor);

  void onCefInputStateChanged(bool editable);

  void onOsrImeCursorRectChanged(const QRect& rc);

  void onOsrShowPopup(bool show);

  void onOsrResizePopup(const QRect& rc);

  void onContextMenuTriggered(QAction* action);

  void onContextMenuDestroyed(QObject* obj);

signals:
  void updateOsrFrame();

protected:
  void onOsrUpdateViewFrame(const QImage& frame, const QRegion& region);

  void onOsrUpdatePopupFrame(const QImage& frame, const QRegion& region);

  void onBeforeCefContextMenu(const MenuBuilder::MenuData& data);

  void onRunCefContextMenu(QPoint pos, CefRefPtr<CefRunContextMenuCallback> callback);

  void onCefContextMenuDismissed();

  void onFileDialog(CefBrowserHost::FileDialogMode mode,
                    const CefString& title,
                    const CefString& default_file_path,
                    const std::vector<CefString>& accept_filters,
#if CEF_VERSION_MAJOR < 102
                    int selected_accept_filter,
#endif
                    CefRefPtr<CefFileDialogCallback> callback);

  bool hasDevTools();

  void showDevTools();

  void closeDevTools();

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;

  QVariant onViewInputMethodQuery(Qt::InputMethodQuery query) const;

  void onViewInputMethodEvent(QInputMethodEvent* event);

  void onViewVisibilityChanged(bool visible);

  void onViewFocusChanged(bool focused);

  void onViewSizeChanged(const QSize& size, const QSize& oldSize);

  void onViewKeyEvent(QKeyEvent* event);

  void onViewMouseEvent(QMouseEvent* event);

  void onViewWheelEvent(QWheelEvent* event);

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

  bool triggerEvent(const QString& name, const QVariantList& args, int64_t frameId = CefViewBrowserClient::MAIN_FRAME);

  bool responseQCefQuery(const QCefQuery& query);

  bool responseQCefQuery(const int64_t query, bool success, const QString& response, int error);

  bool executeJavascript(int64_t frameId, const QString& code, const QString& url);

  bool executeJavascriptWithResult(int64_t frameId, const QString& code, const QString& url, const QString& context);

  void notifyMoveOrResizeStarted();

  bool sendEventNotifyMessage(int64_t frameId, const QString& name, const QVariantList& args);

  bool setPreference(const QString& name, const QVariant& value, const QString& error);
};
