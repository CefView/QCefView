/*
 * File: QCefView.h
 * Project: QCefView
 * Created: 29th March 2016
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFVIEW_H
#define QCEFVIEW_H
#pragma once
#include <QCefView_global.h>

#include <QCefDownloadItem.h>
#include <QCefEvent.h>
#include <QCefQuery.h>
#include <QCefSetting.h>

#pragma region qt_headers
#include <QScopedPointer>
#include <QVariantList>
#include <QWidget>
#include <QWindow>
#pragma endregion qt_headers

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
  /// The main frame identity
  /// </summary>
  static const qint64 MainFrameID = 0;

  /// <summary>
  /// Represents the CEF popup windows open disposition
  /// </summary>
  enum CefWindowOpenDisposition
  {
    CefWindowOpenDispositionUnknown,
    CefWindowOpenDispositionCurrentTab,
    CefWindowOpenDispositionSingletonTab,
    CefWindowOpenDispositionNewForeGroundTab,
    CefWindowOpenDispositionNewBackgroundTab,
    CefWindowOpenDispositionNewPopup,
    CefWindowOpenDispositionNewWindow,
    CefWindowOpenDispositionSaveToDisk,
    CefWindowOpenDispositionOffTheRecord,
    CefWindowOpenDispositionIgnoreAction,
  };
  Q_ENUM(CefWindowOpenDisposition)

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
  /// Adds a url mapping item with local web resource directory
  /// </summary>
  /// <param name="path">The path to the local resource directory</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="priority">The priority</param>
  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);

  /// <summary>
  /// Adds a url mapping item with local archive (.zip) file which contains the web resource
  /// </summary>
  /// <param name="path">The path to the local archive file</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="password">The password of the archive</param>
  /// <param name="priority">The priority</param>
  void addArchiveResource(const QString& path, const QString& url, const QString& password = "", int priority = 0);

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
  bool triggerEvent(const QCefEvent& event, qint64 frameId);

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
  /// Executes javascript code in specified frame, this method does not report the result of the javascript.
  /// To get the result of the javascript execution use <see cref="executeJavascriptWithResult"/>
  /// </summary>
  /// <param name="frameId">The frame id</param>
  /// <param name="code">The javascript code</param>
  /// <param name="url">
  /// The URL where the script in question can be found, if any. The renderer may request this URL to show the developer
  /// the source of the error
  /// </param>
  /// <returns>True on successful; otherwise false</returns>
  bool executeJavascript(qint64 frameId, const QString& code, const QString& url);

  /// <summary>
  /// Executes javascript code in specified frame and the result will be reported through <see
  /// cref="reportJavascriptResult"/> signal
  /// </summary>
  /// <param name="frameId">The frame id</param>
  /// <param name="code">The javascript code</param>
  /// <param name="url">
  /// The URL where the script in question can be found, if any. The renderer may request this URL to show the developer
  /// the source of the error
  /// </param>
  /// <param name="context">The context used to identify the one execution</param>
  /// <returns>True on successful; otherwise false</returns>
  bool executeJavascriptWithResult(qint64 frameId, const QString& code, const QString& url, qint64 context);

  /// <summary>
  /// Sets the preference for this browser
  /// </summary>
  /// <param name="name">The preference name</param>
  /// <param name="value">
  /// The preference value, if this value is QVariant::UnknownType or QVariant::Invalid, the
  /// preference will be restored to default value
  /// </param>
  /// <param name="error">The error message populated on failure</param>
  // <returns>True on successful; otherwise false</returns>
  bool setPreference(const QString& name, const QVariant& value, const QString& error);

  /// <summary>
  /// Sets whether to disable the context menu for popup browser
  /// </summary>
  /// <param name="disable">True to disable; otherwise false</param>
  void setDisablePopupContextMenu(bool disable);

  /// <summary>
  /// Gets whether to disable the context menu for popup browser
  /// </summary>
  /// <returns>True to disable; otherwise false</returns>
  bool isPopupContextMenuDisabled();

signals:
  /// <summary>
  /// Gets called on loading state changed
  /// </summary>
  /// <param name="browserId">Indicates the browser id</param>
  /// <param name="isLoading">Indicates the browser is loading</param>
  /// <param name="canGoBack">Indicates the browser can go back</param>
  /// <param name="canGoForward">Indicates the browser can go forward</param>
  void loadingStateChanged(int browserId, bool isLoading, bool canGoBack, bool canGoForward);

  /// <summary>
  /// Gets called on loading starts
  /// </summary>
  /// <param name="browserId">Indicates the browser id</param>
  /// <param name="frameId">Indicates the frame id</param>
  /// <param name="isMainFrame">Indicates the whether this is the main frame</param>
  /// <param name="transition_type">transition type</param>
  void loadStart(int browserId, qint64 frameId, bool isMainFrame, int transition_type);

  /// <summary>
  /// Gets called on loading ends
  /// </summary>
  /// <param name="browserId">Indicates the browser id</param>
  /// <param name="frameId">Indicates the frame id</param>
  /// <param name="isMainFrame">Indicates the whether this is the main frame</param>
  /// <param name="httpStatusCode">The HTTP status code</param>
  void loadEnd(int browserId, qint64 frameId, bool isMainFrame, int httpStatusCode);

  /// <summary>
  /// Gets called on loading failed due to error
  /// </summary>
  /// <param name="browserId">Indicates the browser id</param>
  /// <param name="frameId">Indicates the frame id</param>
  /// <param name="isMainFrame">Indicates the whether this is the main frame</param>
  /// <param name="errorCode">The error code</param>
  /// <param name="errorMsg">The error message</param>
  /// <param name="failedUrl">The url caused the failure</param>
  void loadError(int browserId,
                 qint64 frameId,
                 bool isMainFrame,
                 int errorCode,
                 const QString& errorMsg,
                 const QString& failedUrl);

  /// <summary>
  /// Gets called on draggable region changed
  /// </summary>
  /// <param name="draggableRegion">The new draggable region</param>
  /// <param name="nonDraggableRegion">The new non-draggable region</param>
  void draggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

  /// <summary>
  /// Gets called on the address changed
  /// </summary>
  /// <param name="frameId">The frame id</param>
  /// <param name="url">The address</param>
  void addressChanged(qint64 frameId, const QString& url);

  /// <summary>
  /// Gets called on title changed
  /// </summary>
  /// <param name="title">The title</param>
  void titleChanged(const QString& title);

  /// <summary>
  /// Gets called on title changed
  /// </summary>
  /// <param name="urls">The urls</param>
  void faviconURLChanged(const QStringList& urls);

  /// <summary>
  /// Gets called on title changed
  /// </summary>
  /// <param name="icon">The icon</param>
  void faviconChanged(const QIcon& icon);

  /// <summary>
  /// Gets called on fullscreen mode changed
  /// </summary>
  /// <param name="fullscreen">The current fullscreen mode</param>
  void fullscreenModeChanged(bool fullscreen);

  /// <summary>
  /// Gets called on status message changed
  /// </summary>
  /// <param name="message">The status message</param>
  void statusMessage(const QString& message);

  /// <summary>
  /// Gets called on console message from the web content
  /// </summary>
  /// <param name="message">The message</param>
  /// <param name="level">The level</param>
  void consoleMessage(const QString& message, int level);

  /// <summary>
  /// Gets called on loading progress changed
  /// </summary>
  /// <param name="progress">Current progress</param>
  void loadingProgressChanged(double progress);

  /// <summary>
  /// Gets called on new <see cref="QCefQuery"/> request
  /// </summary>
  /// <param name="browserId">The browser id</param>
  /// <param name="frameId">The frame id</param>
  /// <param name="query">The query request</param>
  void cefQueryRequest(int browserId, qint64 frameId, const QCefQuery& query);

  /// <summary>
  /// Gets called on invoking method request from web content(Javascript)
  /// </summary>
  /// <param name="browserId">The browser id</param>
  /// <param name="frameId">The frame id</param>
  /// <param name="method">The method name</param>
  /// <param name="arguments">The arguments list</param>
  void invokeMethod(int browserId, qint64 frameId, const QString& method, const QVariantList& arguments);

  /// <summary>
  /// Gets called on the result of the javascript executed with <see cref="executeJavascriptWithResult"/> returned
  /// </summary>
  /// <param name="browserId">The browser id</param>
  /// <param name="frameId">The frame id</param>
  /// <param name="context">The context</param>
  /// <param name="result">The result</param>
  void reportJavascriptResult(int browserId, qint64 frameId, qint64 context, const QVariant& result);

public slots:
  /// <summary>
  /// Gets called after the main browser window created. This slot does not work for OSR mode.
  /// </summary>
  /// <param name="win">The CEF window</param>
  virtual void onBrowserWindowCreated(QWindow* win);

  /// <summary>
  /// Gets called before the popup browser created
  /// </summary>
  /// <param name="frameId">The source frame id</param>
  /// <param name="targetUrl">The target URL</param>
  /// <param name="targetFrameName">The target name</param>
  /// <param name="targetDisposition">Target window open method</param>
  /// <param name="settings">Settings to be used for the popup</param>
  /// <param name="DisableJavascriptAccess">Disable the access to Javascript</param>
  /// <returns>True to cancel the popup; false to allow</returns>
  virtual bool onBeforePopup(qint64 frameId,
                             const QString& targetUrl,
                             const QString& targetFrameName,
                             QCefView::CefWindowOpenDisposition targetDisposition,
                             QCefSetting& settings,
                             bool& DisableJavascriptAccess);

protected:
  /// <summary>
  /// Gets called right after the popup browser was created
  /// </summary>
  /// <param name="wnd">The host window of new created browser</param>
  virtual void onPopupCreated(QWindow* wnd);

  /// <summary>
  /// Gets called on new download item was required. Keep reference to the download item
  /// and call <see cref="QCefDownloadItem::start"/> method to allow and start the download,
  /// Ignore the download item to disallow the download
  /// </summary>
  /// <param name="item">The new download item</param>
  /// <param name="suggestedName">The nesuggested name</param>
  virtual void onNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName);

  /// <summary>
  /// Gets called on download item updated. To get this method called <see cref="QCefDownloadItem::start"/> method must
  /// be called in <see cref="newDownloadItem"/> method
  /// </summary>
  /// <param name="item">The download item</param>
  virtual void onUpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item);

#pragma region QWidget
public slots:
  /// <summary>
  ///
  /// </summary>
  inline void setFocus()
  {
    setFocus(Qt::OtherFocusReason);
  }

public:
  /// <summary>
  /// Please refer to QWidget::setFocus
  /// </summary>
  void setFocus(Qt::FocusReason reason);

  /// <summary>
  /// Please refer to QWidget::inputMethodQuery
  /// </summary>
  QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

protected:
  /// <summary>
  /// Please refer to QWidget::paintEvent
  /// </summary>
  void paintEvent(QPaintEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::inputMethodEvent
  /// </summary>
  void inputMethodEvent(QInputMethodEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::showEvent
  /// </summary>
  void showEvent(QShowEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::hideEvent
  /// </summary>
  void hideEvent(QHideEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::focusInEvent
  /// </summary>
  void focusInEvent(QFocusEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::focusOutEvent
  /// </summary>
  void focusOutEvent(QFocusEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::resizeEvent
  /// </summary>
  void resizeEvent(QResizeEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::keyPressEvent
  /// </summary>
  void keyPressEvent(QKeyEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::keyReleaseEvent
  /// </summary>
  void keyReleaseEvent(QKeyEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::mouseMoveEvent
  /// </summary>
  void mouseMoveEvent(QMouseEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::mousePressEvent
  /// </summary>
  void mousePressEvent(QMouseEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::mouseReleaseEvent
  /// </summary>
  void mouseReleaseEvent(QMouseEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::wheelEvent
  /// </summary>
  void wheelEvent(QWheelEvent* event) override;

  /// <summary>
  /// Please refer to QWidget::contextMenuEvent
  /// </summary>
  void contextMenuEvent(QContextMenuEvent* event) override;
#pragma endregion
};

#endif // QCEFVIEW_H
