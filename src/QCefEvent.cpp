#include <QCefEvent.h>

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>
#pragma endregion cef_headers

#include "details/QCefEventPrivate.h"

QCefEvent::QCefEvent()
  : d_ptr(new QCefEventPrivate)
{}

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
  d_ptr->name = name;
}

const QString
QCefEvent::eventName() const
{
  Q_D(const QCefEvent);
  return d->name;
}

void
QCefEvent::setIntProperty(const QString& key, int value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  Q_D(QCefEvent);
  d->args.insert(key, QVariant::fromValue(value));
}

void
QCefEvent::setDoubleProperty(const QString& key, double value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  Q_D(QCefEvent);
  d->args.insert(key, QVariant::fromValue(value));
}

void
QCefEvent::setStringProperty(const QString& key, const QString& value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  Q_D(QCefEvent);
  d->args.insert(key, QVariant::fromValue(value));
}

void
QCefEvent::setBoolProperty(const QString& key, bool value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  Q_D(QCefEvent);
  d->args.insert(key, QVariant::fromValue(value));
}
