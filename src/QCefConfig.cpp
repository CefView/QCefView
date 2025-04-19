#include <QCefConfig.h>

#include "details/QCefConfigPrivate.h"
#include "details/utils/CommonUtils.h"

REGISTER_METATYPE(QCefConfig);

QCefConfig::QCefConfig()
  : d_ptr(new QCefConfigPrivate)
{
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

void
QCefConfig::addCommandLineSwitch(const QString& smitch)
{
  if (smitch.isEmpty())
    return;

  Q_D(QCefConfig);
  d->commandLineArgs_[smitch.toStdString()] = std::string();
}

void
QCefConfig::addCommandLineSwitchWithValue(const QString& smitch, const QString& v)
{
  if (smitch.isEmpty() || v.isEmpty())
    return;

  Q_D(QCefConfig);
  d->commandLineArgs_[smitch.toStdString()] = v.toStdString();
}

void
QCefConfig::setWindowlessRenderingEnabled(const bool enabled)
{
  Q_D(QCefConfig);
  d->windowlessRenderingEnabled_ = enabled;
}

const QVariant
QCefConfig::WindowlessRenderingEnabled() const
{
  Q_D(const QCefConfig);
  return d->windowlessRenderingEnabled_;
}

void
QCefConfig::setCommandLinePassthroughDisabled(const bool disabled)
{
  Q_D(QCefConfig);
  d->commandLinePassthroughDisabled_ = disabled;
}

const QVariant
QCefConfig::commandLinePassthroughDisabled() const
{
  Q_D(const QCefConfig);
  return d->commandLinePassthroughDisabled_;
}

#if !defined(Q_OS_MACOS)
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
QCefConfig::setLogLevel(const LogLevel lvl)
{
  Q_D(QCefConfig);
  d->logLevel_ = lvl;
}

const QCefConfig::LogLevel
QCefConfig::logLevel() const
{
  Q_D(const QCefConfig);
  return static_cast<LogLevel>(d->logLevel_);
}

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

#if CEF_VERSION_MAJOR < 115
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

#else

const QString
QCefConfig::rootCachePath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->rootCachePath_);
}

void
QCefConfig::setRootCachePath(const QString& path)
{
  Q_D(QCefConfig);
  d->rootCachePath_ = path.toStdString();
}
#endif

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
QCefConfig::setBuiltinSchemeName(const QString& name)
{
  Q_D(QCefConfig);
  d->builtinSchemeName_ = name.toStdString();
}

const QString
QCefConfig::builtinSchemeName() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->builtinSchemeName_);
}

void
QCefConfig::setBackgroundColor(const QColor& color)
{
  Q_D(QCefConfig);
  d->backgroundColor_ = color;
}

const QVariant
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

const QVariant
QCefConfig::persistSessionCookies() const
{
  Q_D(const QCefConfig);
  return d->persistSessionCookies_;
}

#if CEF_VERSION_MAJOR < 128
void
QCefConfig::setPersistUserPreferences(bool enabled)
{
  Q_D(QCefConfig);
  d->persistUserPreferences_ = enabled;
}

const QVariant
QCefConfig::persistUserPreferences() const
{
  Q_D(const QCefConfig);
  return d->persistUserPreferences_;
}
#endif

void
QCefConfig::setRemoteDebuggingPort(short port)
{
  Q_D(QCefConfig);
  d->remoteDebuggingport_ = port;
}

const QVariant
QCefConfig::remoteDebuggingPort() const
{
  Q_D(const QCefConfig);
  return d->remoteDebuggingport_;
}
