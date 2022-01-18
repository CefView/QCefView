#ifndef QCEFSETTING_H
#define QCEFSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QScopedPointer>
#include <QString>
#pragma endregion qt_headers

class QCefSettingPrivate;

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefSetting
{
  Q_DECLARE_PRIVATE(QCefSetting)
  QScopedPointer<QCefSettingPrivate> d_ptr;

  friend class QCefView;

public:
  /// <summary>
  ///
  /// </summary>
  QCefSetting();

  /// <summary>
  /// 
  /// </summary>
  /// <param name="other"></param>
  QCefSetting(const QCefSetting& other);

  /// <summary>
  /// 
  /// </summary>
  /// <param name="other"></param>
  QCefSetting& operator=(const QCefSetting& other);

  /// <summary>
  ///
  /// </summary>
  ~QCefSetting();

private:
};

#endif
