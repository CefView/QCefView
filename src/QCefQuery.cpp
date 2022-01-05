#include <QCefQuery.h>

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>
#pragma endregion cef_headers

#include <QCefView.h>

#include "details/QCefQueryPrivate.h"

QCefQuery::QCefQuery()
  : d_ptr(new QCefQueryPrivate)
{}

QCefQuery::QCefQuery(const QString& req, const int64_t query)
  : QCefQuery()
{
  d_ptr->id_ = query;
  d_ptr->reqeust_ = req;
}

QCefQuery::QCefQuery(const QCefQuery& other)
  : QCefQuery()
{
  d_ptr->id_ = other.d_ptr->id_;
  d_ptr->reqeust_ = other.d_ptr->reqeust_;
  d_ptr->restult_ = other.d_ptr->restult_;
  d_ptr->response_ = other.d_ptr->response_;
}

QCefQuery&
QCefQuery::operator=(const QCefQuery& other)
{
  d_ptr->id_ = other.d_ptr->id_;
  d_ptr->reqeust_ = other.d_ptr->reqeust_;
  d_ptr->restult_ = other.d_ptr->restult_;
  d_ptr->response_ = other.d_ptr->response_;
  return *this;
}

QCefQuery::~QCefQuery() {}

const QString
QCefQuery::reqeust() const
{
  Q_D(const QCefQuery);
  return d->reqeust_;
}

const int64_t
QCefQuery::id() const
{
  Q_D(const QCefQuery);
  return d->id_;
}

const QString
QCefQuery::response() const
{
  Q_D(const QCefQuery);
  return d->response_;
}

const bool
QCefQuery::result() const
{
  Q_D(const QCefQuery);
  return d->restult_;
}

const int
QCefQuery::error() const
{
  Q_D(const QCefQuery);
  return d->error_;
}

void
QCefQuery::setResponseResult(bool success, const QString& response, int error /*= 0*/) const
{
  Q_ASSERT(d_ptr);
  d_ptr->restult_ = success;
  d_ptr->response_ = response;
  d_ptr->error_ = error;
}
