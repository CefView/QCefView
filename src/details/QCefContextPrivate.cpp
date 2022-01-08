#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "QCefConfigPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate()
{
  QObject::connect(&cefWorkerTimer_, &QTimer::timeout, this, &QCefContextPrivate::performCefLoopWork);
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.setInterval(0); // at startup we want the high priority
  cefWorkerTimer_.start();
}

QCefContextPrivate::~QCefContextPrivate() {}

bool
QCefContextPrivate::initialize(const QCefConfigPrivate* config)
{
  // create app delegate before initialization
  pAppDelegate_.reset(new CCefAppDelegate(this));

  // initialize CEF
  return initializeCef(config);
}

void
QCefContextPrivate::uninitialize()
{
  // reset delegate before cleanup
  pAppDelegate_.reset();

  // cleanup CEF
  uninitializeCef();
}

void
QCefContextPrivate::scheduleCefLoopWork(int64_t delayMs)
{
  // calculate the effective delay number
  auto delay = qMax((int64_t)0, qMin(delayMs, kCefWorkerIntervalMs));

  // update timer interval by different thread context
  if (QThread::currentThread() == this->thread()) {
    // current it is in main GUI thread, update timer interval directly
    cefWorkerTimer_.start(delay);
  } else {
    // not in main GUI thread, postpone the updating of timer interval
    QMetaObject::invokeMethod(&cefWorkerTimer_, "start", Qt::QueuedConnection, Q_ARG(int, delay));
  }
}

void
QCefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}
