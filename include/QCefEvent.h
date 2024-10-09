/*
 * File: QCefEvent.h
 * Project: QCefView
 * Created: 29th March 2016
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFEVENT_H
#define QCEFEVENT_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QScopedPointer>
#include <QString>
#include <QVariantList>
#pragma endregion 

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
  /// Sets the argument list
  /// </summary>
  /// <param name="args">The argument list</param>
  void setArguments(const QVariantList& args);

  /// <summary>
  /// Gets the argument list
  /// </summary>
  /// <returns>The argument list</returns>
  QVariantList& arguments();
};

Q_DECLARE_METATYPE(QCefEvent);

#endif // QCEFEVENT_H
