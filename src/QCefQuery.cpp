#include <QCefQuery.h>

#pragma region cef_headers
#include <include/wrapper/cef_message_router.h>
#pragma endregion 

#include <QCefView.h>

#include "details/QCefQueryPrivate.h"
#include "details/utils/CommonUtils.h"

REGISTER_METATYPE(QCefQuery);

QCefQuery::QCefQuery(QCefViewPrivate* source, const QString& req, const int64_t query)
  : d_ptr(new QCefQueryPrivate)
{
  d_ptr->source_ = source;
  d_ptr->id_ = query;
  d_ptr->request_ = req;
}

void
QCefQuery::markAsReplied() const
{
  d_ptr->replyed_ = true;
}

QCefQuery::QCefQuery()
  : d_ptr(new QCefQueryPrivate)
{
}

QCefQuery::~QCefQuery()
{
  d_ptr.reset();
}

const QString
QCefQuery::request() const
{
  Q_D(const QCefQuery);
  return d->request_;
}

const qint64
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
  d_ptr->restult_ = success;
  d_ptr->response_ = response;
  d_ptr->error_ = error;
}

void
QCefQuery::reply(bool success, const QString& response, int error /*= 0*/) const
{
  if (d_ptr->source_) {
    d_ptr->restult_ = success;
    d_ptr->response_ = response;
    d_ptr->error_ = error;
    d_ptr->source_->responseQCefQuery(*this);
  }
}
