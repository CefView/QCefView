#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "CCefClientDelegate.h"

#include "QCefConfigPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate(QCoreApplication* app, int argc, char** argv)
  : argc_(argc)
  , argv_(argv)
{
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.start(kCefWorkerIntervalMs);
  connect(&cefWorkerTimer_, SIGNAL(timeout()), this, SLOT(performCefLoopWork()));
  connect(app, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

QCefContextPrivate::~QCefContextPrivate() {}

bool
QCefContextPrivate::initialize(const QCefConfig* config)
{
  if (!initializeCef(config)) {
    return false;
  }

  // create cef client
  pClientDelegate_ = std::make_shared<CCefClientDelegate>();
  pClient_ = new CefViewBrowserClient(pClientDelegate_);
  return true;
}

void
QCefContextPrivate::uninitialize()
{
  pClientDelegate_ = nullptr;
  pClient_ = nullptr;

  // cleanup CEF
  uninitializeCef();
}

void
QCefContextPrivate::scheduleCefLoopWork(int64_t delayMs)
{
  // calculate the effective delay number
  auto delay = qMax((int64_t)0, qMin(delayMs, kCefWorkerIntervalMs));
  QTimer::singleShot(delay, this, SLOT(performCefLoopWork()));
}

void
QCefContextPrivate::onAboutToQuit()
{
  // close all live browsers
  pClient_->CloseAllBrowsers();

  // check whether can exit now
  if (!this->canExit()) {
    // launch an event loop to wait for the clean process
    // of CEF browsers resource
    QEventLoop exitCleanLoop;

    // create an idle timer to check whether the CEF resource has been
    // cleaned up. If yes then exit the event loop to continue the
    // application exit process
    QTimer exitCheckTimer;
    this->connect(&exitCheckTimer, &QTimer::timeout, [&]() {
      // if all browser were closed and there is only one reference to the
      // CefBrowserClient object (only referred by QCefContextPrivate instance),
      // we can quit safely
      if (this->canExit())
        exitCleanLoop.quit();
    });

    // start the timer
    exitCheckTimer.start(0);

    // enter the event loop
    exitCleanLoop.exec();
  }
}

void
QCefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}

bool
QCefContextPrivate::canExit()
{
  return !pClient_ || (!pClient_->GetBrowserCount() && pClient_->HasOneRef());
}
