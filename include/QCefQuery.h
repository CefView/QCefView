/*
 * File: QCefQuery.h
 * Project: QCefView
 * Created: 29th March 2016
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFQUERY_H
#define QCEFQUERY_H
#pragma once
#include <QCefView_global.h>

#pragma region std_headers
#include <cstdint>
#pragma endregion std_headers

#pragma region qt_headers
#include <QScopedPointer>
#include <QString>
#pragma endregion qt_headers

class QCefQueryPrivate;

/// <summary>
/// Represents the query request sent from the web content(Javascript)
/// </summary>
class QCEFVIEW_EXPORT QCefQuery
{
  Q_DECLARE_PRIVATE(QCefQuery)
  QScopedPointer<QCefQueryPrivate> d_ptr;

public:
  /// <summary>
  /// Constructs a query instance
  /// </summary>
  QCefQuery();

  /// <summary>
  /// Constructs a query instance with request context and query id
  /// </summary>
  /// <param name="req">The request context</param>
  /// <param name="query">The query id</param>
  QCefQuery(const QString& req, const int64_t query);

  /// <summary>
  /// Constructs a query instance from existing one
  /// </summary>
  /// <param name="other">The other query instance</param>
  QCefQuery(const QCefQuery& other);

  /// <summary>
  /// Assigns an existing query instance to current
  /// </summary>
  /// <param name="other">The other query instance</param>
  QCefQuery& operator=(const QCefQuery& other);

  /// <summary>
  /// Destructs a query instance
  /// </summary>
  ~QCefQuery();

  /// <summary>
  /// Gets the query content
  /// </summary>
  /// <returns>The content string</returns>
  const QString request() const;

  /// <summary>
  /// Gets the query id
  /// </summary>
  /// <returns>The query id</returns>
  const qint64 id() const;

  /// <summary>
  /// Gets the response content string
  /// </summary>
  /// <returns>The response content string</returns>
  const QString response() const;

  /// <summary>
  /// Gets the response result
  /// </summary>
  /// <returns>The respone result</returns>
  const bool result() const;

  /// <summary>
  /// Gets the response error
  /// </summary>
  /// <returns>The response error</returns>
  const int error() const;

  /// <summary>
  /// Sets the response
  /// </summary>
  /// <param name="success">True if the query is successful; otherwise false</param>
  /// <param name="response">The response content string</param>
  /// <param name="error">The response error</param>
  void setResponseResult(bool success, const QString& response, int error = 0) const;
};

Q_DECLARE_METATYPE(QCefQuery);

#endif // QCEFQUERY_H
