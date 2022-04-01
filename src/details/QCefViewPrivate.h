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

/// <summary>
///
/// </summary>
enum PaintType
{
  PaintView = 0,
  PaintPopup,
};

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
  QCefContextPrivate* pContextPrivate_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserClient> pClient_;

  /// <summary>
  ///
  /// </summary>
  CCefClientDelegate::RefPtr pClientDelegate_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefBrowser> pCefBrowser_;

  bool showPopup_;
  QRect qPopupRect_;
  QRect qImeCursorRect_;
  QPixmap qCefViewFrame_;
  QPixmap qCefPopupFrame_;

public:
  explicit QCefViewPrivate(QCefView* view, const QString& url, const QCefSetting* setting = nullptr);

  ~QCefViewPrivate();

  void createCefBrowser(QCefView* view, const QString url, const QCefSetting* setting);

  void destroyCefBrowser();

  void invalidateRender();

protected:
  void onCefBrowserCreated(CefRefPtr<CefBrowser>& browser);

public slots:
  void onInputStateChanged(bool editable);

  void onImeCursorRectChanged(const QRect& rc);

  void onUpdateCursor(const QCursor& cursor);

  void onCefWindowLostTabFocus(bool next);

  void onCefWindowGotFocus();

  void onShowPopup(bool show);

  void onResizePopup(const QRect& rc);

  void onCefViewFrame(const QImage& frame, const QRegion& region);

  void onCefPopupFrame(const QImage& frame, const QRegion& region);

protected:
  virtual bool eventFilter(QObject* watched, QEvent* event) override;

  void onVisibilityChanged(bool visible);

  void onFocusChanged(bool focused);

  void onSizeChanged(const QSize& size, const QSize& oldSize);

  void onMouseEvent(QMouseEvent* event);

  void onWheelEvent(QWheelEvent* event);

  void onInputMethodEvent(QInputMethodEvent* event);

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
