#pragma once

#pragma region qt_headers
#include <QWindow>
#include <QPointer>
#include <QVariant>
#include <QHash>
#include <QMutex>
#include <QRegion>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#include <CefViewBrowserHandlerDelegate.h>

#include <QCefQuery.h>
#include <QCefView.h>

/// <summary>
///
/// </summary>
class QCefWindow : public QWindow
{
  Q_OBJECT

public:
  /// <summary>
  ///
  /// </summary>

public:
  /// <summary>
  ///
  /// </summary>
  /// <param name="parent"></param>
  explicit QCefWindow(QWindow* parent, QCefView* hostView = 0);

  /// <summary>
  ///
  /// </summary>
  ~QCefWindow();

  /// <summary>
  ///
  /// </summary>
  /// <param name="win"></param>
  void setBrowserWindowId(CefWindowHandle win);

protected:
  /// <summary>
  ///
  /// </summary>
  void syncBrowserWindow();

  /// <summary>
  ///
  /// </summary>
  /// <param name="e"></param>
  virtual void exposeEvent(QExposeEvent* e);

  /// <summary>
  ///
  /// </summary>
  /// <param name="e"></param>
  virtual void resizeEvent(QResizeEvent* e);

private:
  /// <summary>
  ///
  /// </summary>
  QWindow* browserWindow_;
};
