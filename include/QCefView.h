#ifndef QCEFVIEW_H
#define QCEFVIEW_H
#pragma once
#include <QCefView_global.h>

#include <QCefEvent.h>
#include <QCefQuery.h>
#include <QCefSetting.h>

#pragma region qt_headers
#include <QScopedPointer>
#include <QVariantList>
#include <QWidget>
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

class QCefViewPrivate;

/// <summary>
/// Represents the CEF browser view
/// </summary>
class QCEFVIEW_EXPORT QCefView : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefView)
  Q_DISABLE_COPY(QCefView)
  QScopedPointer<QCefViewPrivate> d_ptr;

public:
  /// <summary>
  /// Constructs a QCefView instance
  /// </summary>
  /// <param name="url">The target url</param>
  /// <param name="setting">The <see cref="QCefSetting"/> instance</param>
  /// <param name="parent">The parent</param>
  QCefView(const QString url, const QCefSetting* setting, QWidget* parent = 0);

  /// <summary>
  /// Constructs a QCefView instance
  /// </summary>
  /// <param name="parent">The parent</param>
  QCefView(QWidget* parent = 0);

  /// <summary>
  /// Destructs the QCefView instance
  /// </summary>
  ~QCefView();

  /// <summary>
  /// Gets the browser id
  /// </summary>
  /// <returns>The browser id</returns>
  int browserId();

  /// <summary>
  /// Navigates to the content.
  /// </summary>
  /// <param name="content">The content</param>
  void navigateToString(const QString& content);

  /// <summary>
  /// Navigates to the URL
  /// </summary>
  /// <param name="url">The url</param>
  void navigateToUrl(const QString& url);

  /// <summary>
  /// Checks whether the browser can go back
  /// </summary>
  /// <returns>True if can; otherwise false</returns>
  bool browserCanGoBack();

  /// <summary>
  /// Checks whether the browser can go forward
  /// </summary>
  /// <returns>True if can; otherwise false</returns>
  bool browserCanGoForward();

  /// <summary>
  /// Requires the browser to go back
  /// </summary>
  void browserGoBack();

  /// <summary>
  /// Requires the browser to go forward
  /// </summary>
  void browserGoForward();

  /// <summary>
  /// Checks whether the browser is loading
  /// </summary>
  /// <returns>True if it is loading; otherwise false</returns>
  bool browserIsLoading();

  /// <summary>
  /// Requires the browser to reload
  /// </summary>
  void browserReload();

  /// <summary>
  /// Requires the browser to stop load
  /// </summary>
  void browserStopLoad();

  /// <summary>
  /// Triggers the event for main frame
  /// </summary>
  /// <param name="event">The <see cref="QCefEvent"/> instance</param>
  /// <returns>True on successful; otherwise false</returns>
  bool triggerEvent(const QCefEvent& event);

  /// <summary>
  /// Triggers the event for specified frame
  /// </summary>
  /// <param name="event">The <see cref="QCefEvent"/> instance</param>
  /// <param name="frameId">The frame id</param>
  /// <returns>True on successful; otherwise false</returns>
  bool triggerEvent(const QCefEvent& event, int frameId);

  /// <summary>
  /// Broad cast the event for all frames
  /// </summary>
  /// <param name="event">The <see cref="QCefEvent"/> instance</param>
  /// <returns>True on successful; otherwise false</returns>
  bool broadcastEvent(const QCefEvent& event);

  /// <summary>
  /// Response the <see cref="QCefQuery"/> request
  /// </summary>
  /// <param name="query">The query instance</param>
  /// <returns>True on successful; otherwise false</returns>
  bool responseQCefQuery(const QCefQuery& query);

  /// <summary>
  /// Gets called on loading state changed
  /// </summary>
  /// <param name="isLoading">Indicates the browser is loading</param>
  /// <param name="canGoBack">Indicates the browser can go back</param>
  /// <param name="canGoForward">Indicates the browser can go forward</param>
  virtual void onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward);

  /// <summary>
  /// Gets called on loading starts
  /// </summary>
  virtual void onLoadStart();

  /// <summary>
  /// Gets called on loading ends
  /// </summary>
  /// <param name="httpStatusCode"></param>
  virtual void onLoadEnd(int httpStatusCode);

  /// <summary>
  /// Gets called on loading failed due to error
  /// </summary>
  /// <param name="errorCode">The error code</param>
  /// <param name="errorMsg">The error message</param>
  /// <param name="failedUrl">The url caused the failure</param>
  /// <param name="handled">Sets this parameter to indicates whether this error was handled or not</param>
  virtual void onLoadError(int errorCode, const QString& errorMsg, const QString& failedUrl, bool& handled);

  /// <summary>
  /// Gets called on draggable region changed
  /// </summary>
  /// <param name="draggableRegion">The new draggable region</param>
  /// <param name="nonDraggableRegion">The new non-draggable region</param>
  virtual void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

  /// <summary>
  /// Gets called on console message from the web content
  /// </summary>
  /// <param name="message">The message</param>
  /// <param name="level">The level</param>
  virtual void onConsoleMessage(const QString& message, int level);

  /// <summary>
  /// Gets called on focus event
  /// </summary>
  /// <param name="next">The next</param>
  virtual void onTakeFocus(bool next);

  /// <summary>
  /// Gets called on cef url request
  /// </summary>
  /// <param name="url">The requested url</param>
  virtual void onQCefUrlRequest(const QString& url);

  /// <summary>
  /// Gets called on new <see cref="QCefQuery"/> request
  /// </summary>
  /// <param name="query">The query request</param>
  virtual void onQCefQueryRequest(const QCefQuery& query);

  /// <summary>
  /// Gets called on invoke method request from web content(Javascript)
  /// </summary>
  /// <param name="browserId">The browser id</param>
  /// <param name="frameId">The frame id</param>
  /// <param name="method">The method name</param>
  /// <param name="arguments">The arguments list</param>
  virtual void onInvokeMethodNotify(int browserId, int frameId, const QString& method, const QVariantList& arguments);
};

#endif // QCEFVIEW_H
