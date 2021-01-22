#ifndef QCEFEVENT_H
#define QCEFEVENT_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QObject>
#include <QString>
#pragma endregion qt_headers

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefEvent : public QObject
{
  Q_OBJECT

public:
  /// <summary>
  ///
  /// </summary>
  QCefEvent();

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  QCefEvent(const QString& name);

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  void setEventName(const QString& name);

  /// <summary>
  ///
  /// </summary>
  /// <param name="key"></param>
  /// <param name="value"></param>
  void setIntProperty(const QString& key, int value);

  /// <summary>
  ///
  /// </summary>
  /// <param name="key"></param>
  /// <param name="value"></param>
  void setDoubleProperty(const QString& key, double value);

  /// <summary>
  ///
  /// </summary>
  /// <param name="key"></param>
  /// <param name="value"></param>
  void setStringProperty(const QString& key, const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <param name="key"></param>
  /// <param name="value"></param>
  void setBoolProperty(const QString& key, bool value);
};

#endif // QCEFEVENT_H
