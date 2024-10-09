#include <QCefContext.h>

#pragma region qt_headers
#include <QDebug>
#pragma endregion 

#include "details/QCefContextPrivate.h"

QCefContext* QCefContext::s_self;

QCefContext::QCefContext(QCoreApplication* app, int argc, char** argv, const QCefConfig* config)
  : QObject(app)
  , d_ptr(new QCefContextPrivate(app, argc, argv))
{
  init(config);
}

QCefContext*
QCefContext::instance()
{
  return s_self;
}

QCefContext::~QCefContext()
{
  uninit();
}

void
QCefContext::addLocalFolderResource(const QString& path, const QString& url, int priority /*= 0*/)
{
  Q_D(QCefContext);

  d->addLocalFolderResource(path, url, priority);
}

void
QCefContext::addArchiveResource(const QString& path,
                                const QString& url,
                                const QString& password /*= ""*/,
                                int priority /*= 0*/)
{
  Q_D(QCefContext);

  d->addArchiveResource(path, url, password, priority);
}

bool
QCefContext::addCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  Q_D(QCefContext);

  return d->addGlobalCookie(name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString());
}

bool
QCefContext::deleteAllCookies()
{
  Q_D(QCefContext);

  return d->deleteAllCookies();
}

bool
QCefContext::addCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                          const QString& targetSchema,
                                          const QString& targetDomain,
                                          bool allowTargetSubdomains)
{
  Q_D(QCefContext);

  return d->addCrossOriginWhitelistEntry(sourceOrigin, targetSchema, targetDomain, allowTargetSubdomains);
}

bool
QCefContext::removeCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                             const QString& targetSchema,
                                             const QString& targetDomain,
                                             bool allowTargetSubdomains)
{
  Q_D(QCefContext);

  return d->removeCrossOriginWhitelistEntry(sourceOrigin, targetSchema, targetDomain, allowTargetSubdomains);
}

bool
QCefContext::clearCrossOriginWhitelistEntry()
{
  Q_D(QCefContext);

  return d->clearCrossOriginWhitelistEntry();
}

const QCefConfig*
QCefContext::cefConfig() const
{
  Q_D(const QCefContext);
  return d->cefConfig();
}

bool
QCefContext::init(const QCefConfig* config)
{
  Q_ASSERT_X(!s_self, "QCefContext::init()", "There can be only one QCefContext instance");
  s_self = this;

  Q_D(QCefContext);
  d->initialize(config);

  return true;
}

void
QCefContext::uninit()
{
  Q_D(QCefContext);
  d->uninitialize();

  s_self = nullptr;
}
