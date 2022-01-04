#ifndef QCEF_H
#define QCEF_H
#pragma once
#include <QCefView_global.h>
#include <QCefSetting.h>

#pragma region qt_headers
#include <QCoreApplication>
#pragma endregion qt_headers

class QCefContextPrivate;

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefContext : public QObject
{
  Q_OBJECT

  Q_DECLARE_PRIVATE(QCefContext)

  QCefContextPrivate* d_ptr;

public:
  /// <summary>
  ///
  /// </summary>
  QCefContext(QCoreApplication* app, QCefSetting& settings, int argc, char* argv[]);

  /// <summary>
  ///
  /// </summary>
  ~QCefContext();

protected slots:
  void doCefWork();

private:
  /// <summary>
  ///
  /// </summary>
  void initialize(QObject* parent, const QCefSetting& settings, int argc, char* argv[]);

  /// <summary>
  ///
  /// </summary>
  void uninitialize();
};

#endif // QCEF_H
