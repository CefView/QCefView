#include "QCefSetting.h"

#pragma region qt_headers
#include <QColor>
#pragma endregion qt_headers

#include "details/CCefSetting.h"

QCefSetting::QCefSetting()
  : d(new CCefSetting())
{}

QCefSetting::~QCefSetting() {}

QCefSetting&
QCefSetting::setBrowserSubProcessPath(const QString& path)
{
  d->browserSubProcessPath_ = path.toStdString();
  return *this;
}

const QString
QCefSetting::browserSubProcessPath()
{
  return QString::fromStdString(d->browserSubProcessPath_);
}

QCefSetting&
QCefSetting::setResourceDirectoryPath(const QString& path)
{
  d->resourceDirectoryPath_ = path.toStdString();
  return *this;
}

const QString
QCefSetting::resourceDirectoryPath()
{
  return QString::fromStdString(d->resourceDirectoryPath_);
}

QCefSetting&
QCefSetting::setLocalesDirectoryPath(const QString& path)
{
  d->localesDirectoryPath_ = path.toStdString();
  return *this;
}

const QString
QCefSetting::localesDirectoryPath()
{
  return QString::fromStdString(d->localesDirectoryPath_);
}

QCefSetting&
QCefSetting::setLocale(const QString& locale)
{
  d->locale_ = locale.toStdString();
  return *this;
}

const QString
QCefSetting::locale()
{
  return QString::fromStdString(d->locale_);
}

QCefSetting&
QCefSetting::setUserAgent(const QString& agent)
{
  d->userAgent_ = agent.toStdString();
  return *this;
}

const QString
QCefSetting::userAgent()
{
  return QString::fromStdString(d->userAgent_);
}

QCefSetting&
QCefSetting::setCachePath(const QString& path)
{
  d->cachePath_ = path.toStdString();
  return *this;
}

const QString
QCefSetting::cachePath()
{
  return QString::fromStdString(d->cachePath_);
}

QCefSetting&
QCefSetting::setUserDataPath(const QString& path)
{
  d->userDataPath_ = path.toStdString();
  return *this;
}

const QString
QCefSetting::userDataPath()
{
  return QString::fromStdString(d->userDataPath_);
}

QCefSetting&
QCefSetting::setBridgeObjectName(const QString& name)
{
  d->bridgeObjectName_ = name.toStdString();
  return *this;
}

const QString
QCefSetting::bridgeObjectName()
{
  return QString::fromStdString(d->bridgeObjectName_);
}

QCefSetting&
QCefSetting::setBackgroundColor(const QColor& color)
{
  d->backgroundColor_ = color.value();
  return *this;
}

const QColor
QCefSetting::backgroundColor()
{
  return d->backgroundColor_;
}

QCefSetting&
QCefSetting::setAcceptLanguageList(const QString& languages)
{
  d->acceptLanguageList_ = languages.toStdString();
  return *this;
}

const QString
QCefSetting::acceptLanguageList()
{
  return QString::fromStdString(d->acceptLanguageList_);
}

QCefSetting&
QCefSetting::setPersistSessionCookies(bool enabled)
{
  d->persistSessionCookies_ = enabled;
  return *this;
}

const bool
QCefSetting::persistSessionCookies()
{
  return d->persistSessionCookies_;
}

QCefSetting&
QCefSetting::setPersistUserPreferences(bool enabled)
{
  d->persistUserPreferences_ = enabled;
  return *this;
}

const bool
QCefSetting::persistUserPreferences()
{
  return d->persistUserPreferences_;
}

QCefSetting&
QCefSetting::setRemoteDebuggingPort(short port)
{
  d->remoteDebuggingport_ = port;
  return *this;
}

const short
QCefSetting::remoteDebuggingPort()
{
  return d->remoteDebuggingport_;
}

QCefSetting&
QCefSetting::addGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  d->cookieList_.push_back({ name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString() });
  return *this;
}
