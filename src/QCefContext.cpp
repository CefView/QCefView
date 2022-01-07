#include <QCefContext.h>

#include <QTimer>
#include <QThread>
#include <QDebug>

#include "details/CCefManager.h"

#define kCefWorkerIntervalMs (0 * 1000 / 60)

/// <summary>
///
/// </summary>
class QCefContextPrivate
{
public:
  QCefContextPrivate()
  {
    cefWorkerTimer.setTimerType(Qt::PreciseTimer);
    cefWorkerTimer.setSingleShot(true);
  }

  ~QCefContextPrivate() {}

  QTimer cefWorkerTimer;
  CCefManager::RefPtr pCefManager;
};

QCefContext* QCefContext::s_self;

QCefContext::QCefContext(QCoreApplication* app, const QCefConfig* config)
  : QObject(app)
  , d_ptr(new QCefContextPrivate)
{
  init(app, config);
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
QCefContext::scheduleMessageLoopWork(int64_t delayMs)
{
  Q_D(QCefContext);

  if (QThread::currentThread() == this->thread()) {
    delayMs <= 0 ? doCefWork() : d->cefWorkerTimer.start(delayMs);
  } else {
    QMetaObject::invokeMethod(
      &d->cefWorkerTimer, "start", Qt::QueuedConnection, Q_ARG(int, delayMs <= 0 ? 0 : delayMs));
  }
}

void
QCefContext::doCefWork()
{
  Q_D(QCefContext);

  // process cef work
  d->pCefManager->doCefWork();

  // check timer status
  int delayMs = d->cefWorkerTimer.remainingTime();

  // overdue
  if (delayMs == 0) {
    // overdue, process immediately
    d->pCefManager->doCefWork();
  } else {
    // schedule at regular interval
    d->cefWorkerTimer.start(delayMs > 0 ? delayMs : kCefWorkerIntervalMs);
  }
}

bool
QCefContext::init(QObject* parent, const QCefConfig* config)
{
  Q_ASSERT_X(!s_self, "QCefContext::init()", "There can be only one QCefContext instance");
  s_self = this;

  Q_D(QCefContext);

  // create and initialize the worker timer
  connect(&d->cefWorkerTimer, &QTimer::timeout, this, &QCefContext::doCefWork);

  // create and initialize the cef manager
  d->pCefManager = std::make_shared<CCefManager>();
  d->pCefManager->initialize(config->d_func());

  return true;
}

void
QCefContext::uninit()
{
  Q_D(QCefContext);

  // reset to release the cef manager
  d->pCefManager->uninitialize();
  d->pCefManager.reset();
  d->cefWorkerTimer.stop();

  s_self = nullptr;
}
