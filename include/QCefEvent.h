#ifndef QCEFEVENT_H
#define QCEFEVENT_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QScopedPointer>
#include <QString>
#pragma endregion qt_headers

class QCefEventPrivate;

/// <summary>
/// Represents the event sent from native context(C/C++ code) to the web context(javascript)
/// </summary>
class QCEFVIEW_EXPORT QCefEvent
{
  Q_DECLARE_PRIVATE(QCefEvent)
  QScopedPointer<QCefEventPrivate> d_ptr;

  friend class QCefView;

public:
  /// <summary>
  /// Constructs an event instance
  /// </summary>
  QCefEvent();

  /// <summary>
  /// Constructs an event instance with name
  /// </summary>
  /// <param name="name">The event name</param>
  QCefEvent(const QString& name);

  /// <summary>
  /// Constructs an event instance from existing one
  /// </summary>
  /// <param name="other">The other event instance</param>
  QCefEvent(const QCefEvent& other);

  /// <summary>
  /// Assigns an existing event instance to current
  /// </summary>
  /// <param name="other">The other event instance</param>
  QCefEvent& operator=(const QCefEvent& other);

  /// <summary>
  /// Destructs the event instance
  /// </summary>
  ~QCefEvent();

  /// <summary>
  /// Sets the event name
  /// </summary>
  /// <param name="name">The name to be set</param>
  void setEventName(const QString& name);

  /// <summary>
  /// Gets the event name
  /// </summary>
  /// <returns>The event name</returns>
  const QString eventName() const;

  /// <summary>
  /// Sets a integer property with name and value
  /// </summary>
  /// <param name="key">The name</param>
  /// <param name="value">The value</param>
  void setIntProperty(const QString& key, int value);

  /// <summary>
  /// Sets a double property with name and value
  /// </summary>
  /// <param name="key">The name</param>
  /// <param name="value">The value</param>
  void setDoubleProperty(const QString& key, double value);

  /// <summary>
  /// Sets a string property with name and value
  /// </summary>
  /// <param name="key">The name</param>
  /// <param name="value">The value</param>
  void setStringProperty(const QString& key, const QString& value);

  /// <summary>
  /// Sets a boolean property with name and value
  /// </summary>
  /// <param name="key">The name</param>
  /// <param name="value">The value</param>
  void setBoolProperty(const QString& key, bool value);
};

#endif // QCEFEVENT_H
