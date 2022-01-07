#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "QCefConfigPrivate.h"

#define kCefWorkerIntervalMs (0 * 1000 / 60)

QCefContextPrivate::QCefContextPrivate()
{
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.setSingleShot(true);
  QObject::connect(&cefWorkerTimer_, &QTimer::timeout, this, &QCefContextPrivate::doCefWork);
}

QCefContextPrivate::~QCefContextPrivate() {}

bool
QCefContextPrivate::initialize(const QCefConfigPrivate* config)
{
  pAppDelegate_ = std::make_shared<CCefAppDelegate>(this);
  if (!initializeCef(config))
    return false;

  return true;
}

void
QCefContextPrivate::uninitialize()
{
  pAppDelegate_.reset();

  uninitializeCef();
}

bool
QCefContextPrivate::addCookie(const std::string& name,
                              const std::string& value,
                              const std::string& domain,
                              const std::string& url)
{
  CefCookie cookie;
  CefString(&cookie.name).FromString(name);
  CefString(&cookie.value).FromString(value);
  CefString(&cookie.domain).FromString(domain);
  return CefCookieManager::GetGlobalManager(nullptr)->SetCookie(CefString(url), cookie, nullptr);
}

void
QCefContextPrivate::scheduleMessageLoopWork(int64_t delayMs)
{
  if (QThread::currentThread() == this->thread()) {
    delayMs <= 0 ? doCefWork() : cefWorkerTimer_.start(delayMs);
  } else {
    QMetaObject::invokeMethod(&cefWorkerTimer_, "start", Qt::QueuedConnection, Q_ARG(int, delayMs <= 0 ? 0 : delayMs));
  }
}

void
QCefContextPrivate::doCefWork()
{
  // process cef work
  CefDoMessageLoopWork();

  // check timer status
  int delayMs = cefWorkerTimer_.remainingTime();

  // overdue
  if (delayMs == 0) {
    // overdue, process immediately
    CefDoMessageLoopWork();
  } else {
    // schedule at regular interval
    cefWorkerTimer_.start(delayMs > 0 ? delayMs : kCefWorkerIntervalMs);
  }
}
