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
///
/// </summary>
class QCEFVIEW_EXPORT QCefContext : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefContext)
  QScopedPointer<QCefContextPrivate> d_ptr;

  friend class QCefView;

public:
  /// <summary>
  ///
  /// </summary>
  QCefContext(QCoreApplication* app, const QCefConfig* config);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static QCefContext* instance();

  /// <summary>
  ///
  /// </summary>
  ~QCefContext();

  /// <summary>
  ///
  /// </summary>
  /// <param name="path"></param>
  /// <param name="url"></param>
  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);

  /// <summary>
  ///
  /// </summary>
  /// <param name="path"></param>
  /// <param name="url"></param>
  /// <param name="password"></param>
  void addArchiveResource(const QString& path, const QString& url, const QString& password = "");

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="value"></param>
  /// <param name="domain"></param>
  /// <param name="url"></param>
  void addCookie(const QString& name, const QString& value, const QString& domain, const QString& url);

protected:
  /// <summary>
  ///
  /// </summary>
  /// <param name="config"></param>
  /// <returns></returns>
  bool init(const QCefConfig* config);

  /// <summary>
  ///
  /// </summary>
  void uninit();

private:
  /// <summary>
  ///
  /// </summary>
  static QCefContext* s_self;
};

#endif // QCEF_H
