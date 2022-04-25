#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"

#include "QCefConfigPrivate.h"
#include "QCefViewPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate(QCoreApplication* app, int argc, char** argv)
  : argc_(argc)
  , argv_(argv)
{
#if defined(Q_OS_MACOS)
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.start(kCefWorkerIntervalMs);
  connect(&cefWorkerTimer_, SIGNAL(timeout()), this, SLOT(performCefLoopWork()));
#endif

  connect(app, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

QCefContextPrivate::~QCefContextPrivate() {}

CefRefPtr<CefViewBrowserApp>
QCefContextPrivate::getCefApp()
{
  return pApp_;
}

bool
QCefContextPrivate::initialize(const QCefConfig* config)
{
  if (!initializeCef(config)) {
    return false;
  }

  return true;
}

void
QCefContextPrivate::addLocalFolderResource(const QString& path, const QString& url, int priority /*= 0*/)
{
  folderResourceMappingList_.append({ path, url, priority });
}

const QList<FolderResourceMapping>&
QCefContextPrivate::folderResourceMappingList()
{
  return folderResourceMappingList_;
}

void
QCefContextPrivate::addArchiveResource(const QString& path,
                                       const QString& url,
                                       const QString& password /*= ""*/,
                                       int priority /*= 0*/)
{
  archiveResourceMappingList_.append({ path, url, password, priority });
}

const QList<ArchiveResourceMapping>&
QCefContextPrivate::archiveResourceMappingList()
{
  return archiveResourceMappingList_;
}

void
QCefContextPrivate::uninitialize()
{
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
  QCefViewPrivate::destroyAllInstance();

  // check whether can exit now
  if (!pApp_->IsSafeToExit()) {
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
      if (pApp_->IsSafeToExit())
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
