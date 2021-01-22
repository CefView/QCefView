#include <QCefEvent.h>

#pragma region qt_headers
#include <QVariant>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/wrapper/cef_message_router.h>
#pragma endregion cef_headers

QCefEvent::QCefEvent()
  : QObject(nullptr)
{}

QCefEvent::QCefEvent(const QString& name)
  : QObject()
{
  setObjectName(name);
}

void
QCefEvent::setEventName(const QString& name)
{
  setObjectName(name);
}

void
QCefEvent::setIntProperty(const QString& key, int value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  setProperty(key.toUtf8().constData(), QVariant::fromValue(value));
}

void
QCefEvent::setDoubleProperty(const QString& key, double value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  setProperty(key.toUtf8().constData(), QVariant::fromValue(value));
}

void
QCefEvent::setStringProperty(const QString& key, const QString& value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  setProperty(key.toUtf8().constData(), QVariant::fromValue(value));
}

void
QCefEvent::setBoolProperty(const QString& key, bool value)
{
  Q_ASSERT(0 != QString::compare(key, "name", Qt::CaseInsensitive));
  setProperty(key.toUtf8().constData(), QVariant::fromValue(value));
}
