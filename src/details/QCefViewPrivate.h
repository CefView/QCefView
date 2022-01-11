#pragma once
#pragma region qt_headers
#include <QString>
#include <QWindow>
#pragma endregion qt_headers

#include <CefViewBrowserClient.h>

#include "QCefContextPrivate.h"

#include <QCefView.h>

class QCefViewPrivate
{
  Q_DECLARE_PUBLIC(QCefView)
  QCefView* q_ptr;

private:
  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate* pContext_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefBrowser> pCefBrowser_;

public:
  explicit QCefViewPrivate(QCefView* view, const QString& url);

  ~QCefViewPrivate();

  QWindow* createBrowserWindow(const QString url, const QCefSetting* setting);

  int browserId();

  void closeAllBrowsers();

  void navigateToString(const QString& content);

  void navigateToUrl(const QString& url);

  bool browserCanGoBack();

  bool browserCanGoForward();

  void browserGoBack();

  void browserGoForward();

  bool browserIsLoading();

  void browserReload();

  void browserStopLoad();

  bool triggerEvent(const QString& name, const QVariantMap& args, int frameId = CefViewBrowserClient::MAIN_FRAME);

  bool responseQCefQuery(const QCefQuery& query);

  void notifyMoveOrResizeStarted();

  bool sendEventNotifyMessage(int frameId, const QString& name, const QVariantMap& args);

  void onToplevelWidgetMoveOrResize();
};
