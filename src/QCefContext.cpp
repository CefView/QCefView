#include <QCefContext.h>

#include <QTimer>

#include "details/CCefManager.h"

/// <summary>
///
/// </summary>
class QCefContextPrivate
{
  Q_DECLARE_PUBLIC(QCefContext)
  QCefContext* q_ptr;

public:
  QCefContextPrivate(QCefContext* q)
    : q_ptr(q)
  {}

private:
  CCefManagerPtr pCefManager = nullptr;
  QTimer* cefWorkerTimer = nullptr;
};

QCefContext::QCefContext(QCoreApplication* app, QCefSetting& settings, int argc, char* argv[])
  : QObject(app)
  , d_ptr(new QCefContextPrivate(this))
{
  initialize(app, settings, argc, argv);
}

QCefContext::~QCefContext()
{
  uninitialize();
}

void
QCefContext::doCefWork()
{
  Q_D(QCefContext);

  if (d) {
    d->pCefManager->doCefWork();
  }
}

void
QCefContext::initialize(QObject* parent, const QCefSetting& settings, int argc, char* argv[])
{
  Q_D(QCefContext);

  // create manager instance
  if (!d->pCefManager) {
    d->pCefManager = std::make_shared<CCefManager>(argc, argv, settings);
  }

  if (d->pCefManager) {
    d->cefWorkerTimer = new QTimer(parent);
    connect(d->cefWorkerTimer, SIGNAL(timeout()), this, SLOT(doCefWork()));
    d->cefWorkerTimer->start(0);
    d->cefWorkerTimer->start();
  }
}

void
QCefContext::uninitialize()
{
  Q_D(QCefContext);

  if (d->pCefManager) {
    d->pCefManager.reset();
  }
}
