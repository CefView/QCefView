#pragma once
#pragma region qt_headers
#include <QString>
#pragma endregion qt_headers

#include <CefViewBrowserHandler.h>

#include "QCefWindow.h"
#include "QCefContextPrivate.h"
#include "CCefHandlerDelegate.h"

#include <QCefView.h>

class QCefViewPrivate
{
  Q_DECLARE_PUBLIC(QCefView)

  QCefView* q_ptr;

private:
  QCefContextPrivate* pContext_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserHandler> pQCefViewHandler_;

  /// <summary>
  ///
  /// </summary>
  CefViewBrowserHandlerDelegateInterface::RefPtr pCefHandlerDelegate_;

public:
  explicit QCefViewPrivate(QCefContextPrivate* context, const QString& url, QCefView* view, QCefWindow* win);

  ~QCefViewPrivate();

  void closeAllBrowsers();

  void addLocalFolderResource(const QString& path, const QString& url);

  void addArchiveResource(const QString& path, const QString& url, const QString& password);

  void navigateToString(const QString& content);

  void navigateToUrl(const QString& url);

  bool browserCanGoBack();

  bool browserCanGoForward();

  void browserGoBack();

  void browserGoForward();

  bool browserIsLoading();

  void browserReload();

  void browserStopLoad();

  bool triggerEvent(const QString& name, const QVariantMap& args, int frameId = CefViewBrowserHandler::MAIN_FRAME);

  bool responseQCefQuery(const QCefQuery& query);

  void notifyMoveOrResizeStarted();

  bool sendEventNotifyMessage(int frameId, const QString& name, const QVariantMap& args);

  void onToplevelWidgetMoveOrResize();

  void setContextMenuHandler(CefContextMenuHandler* handler);

  void setDialogHandler(CefDialogHandler* handler);

  void setDisplayHandler(CefDisplayHandler* handler);

  void setDownloadHandler(CefDownloadHandler* handler);

  void setJSDialogHandler(CefJSDialogHandler* handler);

  void setKeyboardHandler(CefKeyboardHandler* handler);
};
