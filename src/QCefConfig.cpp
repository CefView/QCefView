#include <QCefConfig.h>

#include "details/QCefConfigPrivate.h"

QCefConfig::QCefConfig(int argc, char* argv[])
  : d_ptr(new QCefConfigPrivate)
{
  d_ptr->argc = argc;
  d_ptr->argv = argv;
}

QCefConfig::QCefConfig(const QCefConfig& other)
  : d_ptr(new QCefConfigPrivate)
{
  *d_ptr = *(other.d_ptr);
}

QCefConfig&
QCefConfig::operator=(const QCefConfig& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

QCefConfig::~QCefConfig() {}

#if !defined(OS_MACOS)
void
QCefConfig::setBrowserSubProcessPath(const QString& path)
{
  Q_D(QCefConfig);
  d->browserSubProcessPath_ = path.toStdString();
}

const QString
QCefConfig::browserSubProcessPath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->browserSubProcessPath_);
}

void
QCefConfig::setResourceDirectoryPath(const QString& path)
{
  Q_D(QCefConfig);
  d->resourceDirectoryPath_ = path.toStdString();
}

const QString
QCefConfig::resourceDirectoryPath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->resourceDirectoryPath_);
}

void
QCefConfig::setLocalesDirectoryPath(const QString& path)
{
  Q_D(QCefConfig);
  d->localesDirectoryPath_ = path.toStdString();
}

const QString
QCefConfig::localesDirectoryPath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->localesDirectoryPath_);
}
#endif

void
QCefConfig::setLocale(const QString& locale)
{
  Q_D(QCefConfig);
  d->locale_ = locale.toStdString();
}

const QString
QCefConfig::locale() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->locale_);
}

void
QCefConfig::setUserAgent(const QString& agent)
{
  Q_D(QCefConfig);
  d->userAgent_ = agent.toStdString();
}

const QString
QCefConfig::userAgent() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->userAgent_);
}

void
QCefConfig::setCachePath(const QString& path)
{
  Q_D(QCefConfig);
  d->cachePath_ = path.toStdString();
}

const QString
QCefConfig::cachePath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->cachePath_);
}

void
QCefConfig::setUserDataPath(const QString& path)
{
  Q_D(QCefConfig);
  d->userDataPath_ = path.toStdString();
}

const QString
QCefConfig::userDataPath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->userDataPath_);
}

void
QCefConfig::setBridgeObjectName(const QString& name)
{
  Q_D(QCefConfig);
  d->bridgeObjectName_ = name.toStdString();
}

const QString
QCefConfig::bridgeObjectName() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->bridgeObjectName_);
}

void
QCefConfig::setBackgroundColor(const QColor& color)
{
  Q_D(QCefConfig);
  d->backgroundColor_ = color.value();
}

const QColor
QCefConfig::backgroundColor() const
{
  Q_D(const QCefConfig);
  return d->backgroundColor_;
}

void
QCefConfig::setAcceptLanguageList(const QString& languages)
{
  Q_D(QCefConfig);
  d->acceptLanguageList_ = languages.toStdString();
}

const QString
QCefConfig::acceptLanguageList() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->acceptLanguageList_);
}

void
QCefConfig::setPersistSessionCookies(bool enabled)
{
  Q_D(QCefConfig);
  d->persistSessionCookies_ = enabled;
}

const bool
QCefConfig::persistSessionCookies() const
{
  Q_D(const QCefConfig);
  return d->persistSessionCookies_;
}

void
QCefConfig::setPersistUserPreferences(bool enabled)
{
  Q_D(QCefConfig);
  d->persistUserPreferences_ = enabled;
}

const bool
QCefConfig::persistUserPreferences() const
{
  Q_D(const QCefConfig);
  return d->persistUserPreferences_;
}

void
QCefConfig::setRemoteDebuggingPort(short port)
{
  Q_D(QCefConfig);
  d->remoteDebuggingport_ = port;
}

const short
QCefConfig::remoteDebuggingPort() const
{
  Q_D(const QCefConfig);
  return d->remoteDebuggingport_;
}

void
QCefConfig::addGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  Q_D(QCefConfig);
  d->cookieList_.push_back({ name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString() });
}
