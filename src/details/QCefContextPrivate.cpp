#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion

#pragma region cef_headers
#include <include/cef_origin_whitelist.h>
#pragma endregion

#include <QCefContext.h>

#include "CCefAppDelegate.h"

#include "QCefConfigPrivate.h"
#include "QCefViewPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate(QCoreApplication* app, int argc, char** argv)
  : argc_(argc)
  , argv_(argv)
  , config_(nullptr)
{
#if defined(Q_OS_MACOS) || defined(CEF_USE_QT_EVENT_LOOP)
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
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
  config_ = config;

  // initialize CEF
  if (!initializeCef(config)) {
    return false;
  }
  
#if defined(Q_OS_MACOS) || defined(CEF_USE_QT_EVENT_LOOP)
  cefWorkerTimer_.start(kCefWorkerIntervalMs);
#endif

  return true;
}

const QCefConfig*
QCefContextPrivate::cefConfig() const
{
  return config_;
}

void
QCefContextPrivate::addLocalFolderResource(const QString& path, const QString& url, int priority /*= 0*/)
{
  pApp_->AddLocalFolderResource(path.toStdString(), url.toStdString(), priority);
}

void
QCefContextPrivate::addArchiveResource(const QString& path,
                                       const QString& url,
                                       const QString& password /*= ""*/,
                                       int priority /*= 0*/)
{
  pApp_->AddArchiveResource(path.toStdString(), url.toStdString(), password.toStdString(), priority);
}

bool
QCefContextPrivate::addGlobalCookie(const std::string& name,
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

bool
QCefContextPrivate::deleteAllCookies()
{
  return CefCookieManager::GetGlobalManager(nullptr)->DeleteCookies(CefString(), CefString(), nullptr);
}

bool
QCefContextPrivate::addCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                                 const QString& targetSchema,
                                                 const QString& targetDomain,
                                                 bool allowTargetSubdomains)
{
  CefString source(sourceOrigin.toStdString());
  CefString schema(targetSchema.toStdString());
  CefString domain(targetDomain.toStdString());
  return CefAddCrossOriginWhitelistEntry(source, schema, domain, allowTargetSubdomains);
}

bool
QCefContextPrivate::removeCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                                    const QString& targetSchema,
                                                    const QString& targetDomain,
                                                    bool allowTargetSubdomains)
{
  CefString source(sourceOrigin.toStdString());
  CefString schema(targetSchema.toStdString());
  CefString domain(targetDomain.toStdString());
  return CefRemoveCrossOriginWhitelistEntry(source, schema, domain, allowTargetSubdomains);
}

bool
QCefContextPrivate::clearCrossOriginWhitelistEntry()
{
  return CefClearCrossOriginWhitelist();
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
  QTimer::singleShot(static_cast<int>(delay), this, SLOT(performCefLoopWork()));
}

void
QCefContextPrivate::onAboutToQuit()
{
  if (!pApp_) {
    return;
  }
  
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
    connect(&exitCheckTimer, &QTimer::timeout, [&]() {
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
