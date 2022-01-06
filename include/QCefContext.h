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
  ;

  /// <summary>
  ///
  /// </summary>
  ~QCefContext();

  /// <summary>
  ///
  /// </summary>
  /// <param name="delay_ms"></param>
  void scheduleMessageLoopWork(int64_t delay_ms);

protected slots:
  void doCefWork();

private:
  /// <summary>
  ///
  /// </summary>
  static QCefContext* s_self;

  /// <summary>
  ///
  /// </summary>
  bool init(QObject* parent, const QCefConfig* config);

  /// <summary>
  ///
  /// </summary>
  void uninit();
};

#endif // QCEF_H
