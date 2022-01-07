#include <QCefContext.h>

#pragma region qt_headers
#include <QDebug>
#pragma endregion qt_headers

#include "details/QCefContextPrivate.h"

QCefContext* QCefContext::s_self;

QCefContext::QCefContext(QCoreApplication* app, const QCefConfig* config)
  : QObject(app)
  , d_ptr(new QCefContextPrivate)
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
QCefContext::addLocalFolderResource(const QString& path, const QString& url, int priority /* = 0*/)
{
  Q_D(QCefContext);

  d->folderMappingList_.push_back({ path, url, priority });
}

void
QCefContext::addArchiveResource(const QString& path, const QString& url, const QString& password /* = ""*/)
{
  Q_D(QCefContext);

  d->archiveMappingList_.push_back({ path, url, password });
}

void
QCefContext::addCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  Q_D(QCefContext);

  d->addCookie(name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString());
}

bool
QCefContext::init(const QCefConfig* config)
{
  Q_ASSERT_X(!s_self, "QCefContext::init()", "There can be only one QCefContext instance");
  s_self = this;

  Q_D(QCefContext);
  d->initialize(config->d_func());

  return true;
}

void
QCefContext::uninit()
{
  Q_D(QCefContext);
  d->uninitialize();

  s_self = nullptr;
}
