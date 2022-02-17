#ifndef QCEF_H
#define QCEF_H
#pragma once
#include <QCefView_global.h>

#include <QCefConfig.h>

#pragma region qt_headers
#include <QCoreApplication>
#include <QScopedPointer>
#pragma endregion qt_headers

class QCefContextPrivate;

/// <summary>
/// Represents the CEF context
/// </summary>
class QCEFVIEW_EXPORT QCefContext : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefContext)
  QScopedPointer<QCefContextPrivate> d_ptr;

  friend class QCefViewPrivate;

public:
  /// <summary>
  /// Constructs the CEF context
  /// </summary>
  /// <param name="app">The application</param>
  /// <param name="config">The <see cref="QCefConfig"/> instance</param>
  QCefContext(QCoreApplication* app, const QCefConfig* config);

  /// <summary>
  /// Gets the unique default instance
  /// </summary>
  /// <returns>The default instance</returns>
  static QCefContext* instance();

  /// <summary>
  /// Destructs the CEF context
  /// </summary>
  ~QCefContext();

  /// <summary>
  /// Adds a url mapping item with local web resource directory
  /// </summary>
  /// <param name="path">The path to the local resource directory</param>
  /// <param name="url">The url to be mapped to</param>
  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);

  /// <summary>
  /// Adds a url mapping item with local archive (.zip) file which contains the web resource
  /// </summary>
  /// <param name="path">The path to the local archive file</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="password">The password of the archive</param>
  void addArchiveResource(const QString& path, const QString& url, const QString& password = "");

  /// <summary>
  /// Adds a cookie to the CEF context, this cookie is accessible from all browsers created with this context
  /// </summary>
  /// <param name="name">The cookie item name</param>
  /// <param name="value">The cookie item value</param>
  /// <param name="domain">The applicable domain name</param>
  /// <param name="url">The applicable url</param>
  /// <returns>True on success; otherwise false</returns>
  bool addCookie(const QString& name, const QString& value, const QString& domain, const QString& url);

protected:
  /// <summary>
  /// Initialize the CEF context
  /// </summary>
  /// <param name="config">The <see cref="QCefConfig"/> instance</param>
  /// <returns>True on success; otherwise false</returns>
  bool init(const QCefConfig* config);

  /// <summary>
  /// Uninitialize the CEF context
  /// </summary>
  void uninit();

private:
  /// <summary>
  /// The default instance
  /// </summary>
  static QCefContext* s_self;
};

#endif // QCEF_H
