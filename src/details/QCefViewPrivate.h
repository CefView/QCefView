#pragma once
#pragma region qt_headers
#include <QAbstractNativeEventFilter>
#include <QSet>
#include <QString>
#include <QWindow>
#pragma endregion qt_headers

#include <CefViewBrowserClient.h>
#include <CefViewBrowserClientDelegate.h>

#include "CCefClientDelegate.h"
#include "QCefContextPrivate.h"

#include <QCefView.h>

class QCefViewPrivate
  : public QObject
  , public QAbstractNativeEventFilter
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

#if defined(CEF_USE_OSR)
  /// <summary>
  /// Offscreen rendering private data
  /// </summary>
  struct OsrPrivateData
  {
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
    QPixmap qCefViewFrame_;

    /// <summary>
    ///
    /// </summary>
    QPixmap qCefPopupFrame_;
  } osr;
#else
  /// <summary>
  /// Native child window private data
  /// </summary>
  struct NcwPrivateData
  {
    /// <summary>
    ///
    /// </summary>
    QWindow* qBrowserWindow_ = nullptr;

    /// <summary>
    ///
    /// </summary>
    QWidget* qBrowserWidget_ = nullptr;
  } ncw;
#endif

public:
  explicit QCefViewPrivate(QCefContextPrivate* ctx,
                           QCefView* view,
                           const QString& url,
                           const QCefSetting* setting = nullptr);

  ~QCefViewPrivate();

  void createCefBrowser(QCefView* view, const QString url, const QCefSetting* setting);

  void destroyCefBrowser();

protected:
  void onCefMainBrowserCreated(CefRefPtr<CefBrowser>& browser, QWindow* window);

  void onCefPopupBrowserCreated(CefRefPtr<CefBrowser>& browser, QWindow* window);

  bool onCefDoCloseBrowser(CefRefPtr<CefBrowser>& browser);

  void onCefBeforeCloseBrowser(CefRefPtr<CefBrowser>& browser);

  void setCefWindowFocus(bool focus);

public slots:
  void onAppFocusChanged(QWidget* old, QWidget* now);

  void onCefWindowLostTabFocus(bool next);

  void onCefWindowGotFocus();

  void onCefUpdateCursor(const QCursor& cursor);

  void onCefInputStateChanged(bool editable);

#if defined(CEF_USE_OSR)
  void onOsrImeCursorRectChanged(const QRect& rc);

  void onOsrShowPopup(bool show);

  void onOsrResizePopup(const QRect& rc);

  void onOsrUpdateViewFrame(const QImage& frame, const QRegion& region);

  void onOsrUpdatePopupFrame(const QImage& frame, const QRegion& region);
#endif

protected:
  virtual bool eventFilter(QObject* watched, QEvent* event) override;

  QVariant onViewInputMethodQuery(Qt::InputMethodQuery query) const;

  void onViewInputMethodEvent(QInputMethodEvent* event);

  void onViewVisibilityChanged(bool visible);

  void onViewFocusChanged(bool focused);

  void onViewSizeChanged(const QSize& size, const QSize& oldSize);

  void onViewMouseEvent(QMouseEvent* event);

  void onViewWheelEvent(QWheelEvent* event);

  virtual bool nativeEventFilter(const QByteArray& eventType,
                                 void* message,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                 qintptr* result
#else
                                 long* result
#endif
                                 ) override;

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

  bool executeJavascript(int64_t frameId, const QString& code, const QString& url);

  bool executeJavascriptWithResult(int64_t frameId, const QString& code, const QString& url, int64_t context);

  void notifyMoveOrResizeStarted();

  bool sendEventNotifyMessage(int64_t frameId, const QString& name, const QVariantList& args);

  bool setPreference(const QString& name, const QVariant& value, const QString& error);
};
