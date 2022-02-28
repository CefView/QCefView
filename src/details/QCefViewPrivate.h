#pragma once
#pragma region qt_headers
#include <QString>
#include <QWindow>
#pragma endregion qt_headers

#include <CefViewBrowserClient.h>

#include "QCefContextPrivate.h"
#include "QCefSettingPrivate.h"

#include <QCefView.h>

class QCefViewPrivate : public QObject
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(QCefView)
  QCefView* q_ptr;

  friend class CCefClientDelegate;

private:
  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate* pContext_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefBrowser> pCefBrowser_;

  /// <summary>
  ///
  /// </summary>
  QWindow* qBrowserWindow_;

  /// <summary>
  ///
  /// </summary>
  QWidget* qBrowserWidget_;

public:
  explicit QCefViewPrivate(QCefView* view, const QString& url, const QCefSettingPrivate* setting = nullptr);

  ~QCefViewPrivate();

protected:
  void createBrowser(QCefView* view, const QString url, const QCefSettingPrivate* setting);

  void closeBrowser();

  void destroyBrowser();

protected slots:
  void focusChanged(QWidget* old, QWidget* now);

protected:
  /// <summary>
  ///
  /// </summary>
  /// <param name="watched"></param>
  /// <param name="event"></param>
  /// <returns></returns>
  virtual bool eventFilter(QObject* watched, QEvent* event) override;

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

  void onTakeFocus(bool next);

  void setFocus(bool focus);

  void onGotFocus();

  bool setPreference(const QString& name, const QVariant& value, const QString& error);
};
