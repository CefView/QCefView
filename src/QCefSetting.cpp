#include <QCefSetting.h>

#include "details/QCefSettingPrivate.h"

QCefSetting::QCefSetting(int argc, char* argv[])
  : d_ptr(new QCefSettingPrivate)
{
  d_ptr->argc = argc;
  d_ptr->argv = argv;
}

QCefSetting::QCefSetting(const QCefSetting& other)
  : d_ptr(new QCefSettingPrivate)
{
  *d_ptr = *(other.d_ptr);
}

QCefSetting&
QCefSetting::operator=(const QCefSetting& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

QCefSetting::~QCefSetting() {}

#if !defined(OS_MACOS)
void
QCefSetting::setBrowserSubProcessPath(const QString& path)
{
  Q_D(QCefSetting);
  d->browserSubProcessPath_ = path.toStdString();
}

const QString
QCefSetting::browserSubProcessPath() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->browserSubProcessPath_);
}

void
QCefSetting::setResourceDirectoryPath(const QString& path)
{
  Q_D(QCefSetting);
  d->resourceDirectoryPath_ = path.toStdString();
}

const QString
QCefSetting::resourceDirectoryPath() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->resourceDirectoryPath_);
}

void
QCefSetting::setLocalesDirectoryPath(const QString& path)
{
  Q_D(QCefSetting);
  d->localesDirectoryPath_ = path.toStdString();
}

const QString
QCefSetting::localesDirectoryPath() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->localesDirectoryPath_);
}
#endif

void
QCefSetting::setLocale(const QString& locale)
{
  Q_D(QCefSetting);
  d->locale_ = locale.toStdString();
}

const QString
QCefSetting::locale() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->locale_);
}

void
QCefSetting::setUserAgent(const QString& agent)
{
  Q_D(QCefSetting);
  d->userAgent_ = agent.toStdString();
}

const QString
QCefSetting::userAgent() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->userAgent_);
}

void
QCefSetting::setCachePath(const QString& path)
{
  Q_D(QCefSetting);
  d->cachePath_ = path.toStdString();
}

const QString
QCefSetting::cachePath() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->cachePath_);
}

void
QCefSetting::setUserDataPath(const QString& path)
{
  Q_D(QCefSetting);
  d->userDataPath_ = path.toStdString();
}

const QString
QCefSetting::userDataPath() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->userDataPath_);
}

void
QCefSetting::setBridgeObjectName(const QString& name)
{
  Q_D(QCefSetting);
  d->bridgeObjectName_ = name.toStdString();
}

const QString
QCefSetting::bridgeObjectName() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->bridgeObjectName_);
}

void
QCefSetting::setBackgroundColor(const QColor& color)
{
  Q_D(QCefSetting);
  d->backgroundColor_ = color.value();
}

const QColor
QCefSetting::backgroundColor() const
{
  Q_D(const QCefSetting);
  return d->backgroundColor_;
}

void
QCefSetting::setAcceptLanguageList(const QString& languages)
{
  Q_D(QCefSetting);
  d->acceptLanguageList_ = languages.toStdString();
}

const QString
QCefSetting::acceptLanguageList() const
{
  Q_D(const QCefSetting);
  return QString::fromStdString(d->acceptLanguageList_);
}

void
QCefSetting::setPersistSessionCookies(bool enabled)
{
  Q_D(QCefSetting);
  d->persistSessionCookies_ = enabled;
}

const bool
QCefSetting::persistSessionCookies() const
{
  Q_D(const QCefSetting);
  return d->persistSessionCookies_;
}

void
QCefSetting::setPersistUserPreferences(bool enabled)
{
  Q_D(QCefSetting);
  d->persistUserPreferences_ = enabled;
}

const bool
QCefSetting::persistUserPreferences() const
{
  Q_D(const QCefSetting);
  return d->persistUserPreferences_;
}

void
QCefSetting::setRemoteDebuggingPort(short port)
{
  Q_D(QCefSetting);
  d->remoteDebuggingport_ = port;
}

const short
QCefSetting::remoteDebuggingPort() const
{
  Q_D(const QCefSetting);
  return d->remoteDebuggingport_;
}

void
QCefSetting::addGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  Q_D(QCefSetting);
  d->cookieList_.push_back({ name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString() });
}
