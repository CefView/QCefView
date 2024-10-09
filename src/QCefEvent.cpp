#include <QCefEvent.h>

#pragma region cef_headers
#include <include/wrapper/cef_message_router.h>
#pragma endregion 

#include "details/QCefEventPrivate.h"
#include "details/utils/CommonUtils.h"

REGISTER_METATYPE(QCefEvent);

QCefEvent::QCefEvent()
  : d_ptr(new QCefEventPrivate)
{
}

QCefEvent::QCefEvent(const QString& name)
  : QCefEvent()
{
  setEventName(name);
}

QCefEvent::QCefEvent(const QCefEvent& other)
  : QCefEvent()
{
  *d_ptr = *(other.d_ptr);
}

QCefEvent&
QCefEvent::operator=(const QCefEvent& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

QCefEvent::~QCefEvent() {}

void
QCefEvent::setEventName(const QString& name)
{
  Q_D(QCefEvent);
  d->name = name;
}

const QString
QCefEvent::eventName() const
{
  Q_D(const QCefEvent);
  return d->name;
}

void
QCefEvent::setArguments(const QVariantList& args)
{
  Q_D(QCefEvent);
  d->args = args;
}

QVariantList&
QCefEvent::arguments()
{
  Q_D(QCefEvent);
  return d->args;
}
