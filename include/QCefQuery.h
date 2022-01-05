#ifndef QCEFQUERY_H
#define QCEFQUERY_H
#pragma once
#include <QCefView_global.h>

#pragma region std_headers
#include <cstdint>
#pragma endregion std_headers

#pragma region qt_headers
#include <QString>
#include <QScopedPointer>
#pragma endregion qt_headers

class QCefQueryPrivate;

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefQuery
{
  Q_DECLARE_PRIVATE(QCefQuery)
  QScopedPointer<QCefQueryPrivate> d_ptr;

public:
  /// <summary>
  ///
  /// </summary>
  QCefQuery();

  /// <summary>
  ///
  /// </summary>
  /// <param name="req"></param>
  /// <param name="query"></param>
  QCefQuery(const QString& req, const int64_t query);

  /// <summary>
  ///
  /// </summary>
  /// <param name="other"></param>
  QCefQuery(const QCefQuery& other);

  /// <summary>
  ///
  /// </summary>
  /// <param name="other"></param>
  /// <returns></returns>
  QCefQuery& operator=(const QCefQuery& other);

  /// <summary>
  ///
  /// </summary>
  ~QCefQuery();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString reqeust() const;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const int64_t id() const;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString response() const;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const bool result() const;

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const int error() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="success"></param>
  /// <param name="response"></param>
  /// <param name="error"></param>
  void setResponseResult(bool success, const QString& response, int error = 0) const;
};

#endif // QCEFQUERY_H
