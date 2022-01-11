#ifndef QCEFVIEW_H
#define QCEFVIEW_H
#pragma once
#include <QCefView_global.h>
#include <QCefQuery.h>
#include <QCefEvent.h>
#include <QCefSetting.h>

#pragma region qt_headers
#include <QWidget>
#include <QVariantList>
#include <QScopedPointer>
#pragma endregion qt_headers

/** Outline of QCefView:
 **
 **		+--------------------------------------------------------------+
 **		| QCefView(QWidget)                                            |
 **		|                                                              |
 **		|    +----------------------------------------------------+    |
 **		|    | WindowWrapper(QWidget)                             |    |
 **		|    |                                                    |    |
 **		|    |    +-------------------------------------------+   |    |
 **		|    |    | CefBrowserWindowHost(Native)              |   |    |
 **		|    |    |                                           |   |    |
 **		|    |    |                                           |   |    |
 **		|    |    |                                           |   |    |
 **		|    |    +-------------------------------------------+   |    |
 **		|    |                                                    |    |
 **		|    +----------------------------------------------------+    |
 **		|                                                              |
 **		+--------------------------------------------------------------+
 **
 **/

class CefContextMenuHandler;
class CefDialogHandler;
class CefDisplayHandler;
class CefDownloadHandler;
class CefJSDialogHandler;
class CefKeyboardHandler;

class QCefViewPrivate;

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefView : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefView)
  Q_DISABLE_COPY(QCefView)
  QScopedPointer<QCefViewPrivate> d_ptr;

public:
  /// <summary>
  ///
  /// </summary>
  QCefView(const QString url, const QCefSetting* setting, QWidget* parent = 0);

  /// <summary>
  ///
  /// </summary>
  QCefView(QWidget* parent = 0);

  /// <summary>
  ///
  /// </summary>
  ~QCefView();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  int browserId();

  /// <summary>
  /// Navigates to the content.
  /// </summary>
  /// <param name="content"></param>
  void navigateToString(const QString& content);

  /// <summary>
  ///
  /// </summary>
  /// <param name="url"></param>
  void navigateToUrl(const QString& url);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  bool browserCanGoBack();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  bool browserCanGoForward();

  /// <summary>
  ///
  /// </summary>
  void browserGoBack();

  /// <summary>
  ///
  /// </summary>
  void browserGoForward();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  bool browserIsLoading();

  /// <summary>
  ///
  /// </summary>
  void browserReload();

  /// <summary>
  ///
  /// </summary>
  void browserStopLoad();

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="event"></param>
  /// <param name="frameId"></param>
  /// <returns></returns>
  bool triggerEvent(const QCefEvent& event);

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="event"></param>
  /// <param name="frameId"></param>
  /// <returns></returns>
  bool triggerEvent(const QCefEvent& event, int frameId);

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="event"></param>
  /// <returns></returns>
  bool broadcastEvent(const QCefEvent& event);

  /// <summary>
  ///
  /// </summary>
  /// <param name="query"></param>
  /// <returns></returns>
  bool responseQCefQuery(const QCefQuery& query);

  /// <summary>
  ///
  /// </summary>
  /// <param name="isLoading"></param>
  /// <param name="canGoBack"></param>
  /// <param name="canGoForward"></param>
  virtual void onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward);

  /// <summary>
  ///
  /// </summary>
  virtual void onLoadStart();

  /// <summary>
  ///
  /// </summary>
  /// <param name="httpStatusCode"></param>
  virtual void onLoadEnd(int httpStatusCode);

  /// <summary>
  ///
  /// </summary>
  /// <param name="errorCode"></param>
  /// <param name="errorMsg"></param>
  /// <param name="failedUrl"></param>
  /// <param name="handled"></param>
  virtual void onLoadError(int errorCode, const QString& errorMsg, const QString& failedUrl, bool& handled);

  /// <summary>
  ///
  /// </summary>
  /// <param name="draggableRegion"></param>
  /// <param name="nonDraggableRegion"></param>
  virtual void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

  /// <summary>
  ///
  /// </summary>
  /// <param name="message"></param>
  /// <param name="level"></param>
  virtual void onConsoleMessage(const QString& message, int level);

  /// <summary>
  ///
  /// </summary>
  /// <param name="next"></param>
  virtual void onTakeFocus(bool next);

  /// <summary>
  ///
  /// </summary>
  /// <param name="url"></param>
  virtual void onQCefUrlRequest(const QString& url);

  /// <summary>
  ///
  /// </summary>
  /// <param name="query"></param>
  virtual void onQCefQueryRequest(const QCefQuery& query);

  /// <summary>
  ///
  /// </summary>
  /// <param name="browserId"></param>
  /// <param name="frameId"></param>
  /// <param name="method"></param>
  /// <param name="arguments"></param>
  virtual void onInvokeMethodNotify(int browserId, int frameId, const QString& method, const QVariantList& arguments);

protected:
  /// <summary>
  ///
  /// </summary>
  /// <param name="event"></param>
  virtual void changeEvent(QEvent* event) override;

  /// <summary>
  ///
  /// </summary>
  /// <param name="watched"></param>
  /// <param name="event"></param>
  /// <returns></returns>
  virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // QCEFVIEW_H
