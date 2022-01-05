#pragma once
#pragma region qt_headers
#include <QPoint>
#pragma endregion qt_headers

#include <CefViewBrowserHandler.h>

#include "QCefWindow.h"
#include "CCefHandlerDelegate.h"

#include <QCefView.h>

class QCefViewPrivate
{
  Q_DECLARE_PUBLIC(QCefView)

  QCefView* q_ptr;

private:
  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserHandler> pQCefViewHandler_;

  /// <summary>
  ///
  /// </summary>
  CefViewBrowserHandlerDelegateInterface::RefPtr pCefHandlerDelegate_;

public:
  /// <summary>
  ///
  /// </summary>
  typedef struct FolderMapping
  {
    QString path;
    QString url;
    int priority;
  } FolderMapping;
  static QList<FolderMapping> folderMappingList_;

  /// <summary>
  ///
  /// </summary>
  typedef struct ArchiveMapping
  {
    QString path;
    QString url;
    QString psw;
  } ArchiveMapping;
  static QList<ArchiveMapping> archiveMappingList_;

public:
  explicit QCefViewPrivate(const QString& url, QCefView* view, QCefWindow* win);

  ~QCefViewPrivate();

  void closeAllBrowsers();

  void addLocalFolderResource(const QString& path, const QString& url);

  void addArchiveResource(const QString& path, const QString& url, const QString& password);

  void addCookie(const QString& name, const QString& value, const QString& domain, const QString& url);

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
