#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "CCefClientDelegate.h"

#include "QCefConfigPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate(QCoreApplication* app)
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
  pClient_->CloseAllBrowsers();

  // keep processing events till all browser closed
  QCoreApplication::processEvents();
  while (!pClient_->HasOneRef() || pClient_->GetBrowserCount()) {
    QCoreApplication::processEvents();
  }
}

void
QCefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}
