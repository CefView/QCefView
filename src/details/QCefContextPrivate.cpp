#include "QCefContextPrivate.h"

#pragma region qt_headers
#include <QThread>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "CCefAppDelegate.h"
#include "CCefClientDelegate.h"

#include "QCefConfigPrivate.h"

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

QCefContextPrivate::QCefContextPrivate()
{
  QObject::connect(&cefWorkerTimer_, &QTimer::timeout, this, &QCefContextPrivate::performCefLoopWork);
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  cefWorkerTimer_.start(kCefWorkerIntervalMs);
}

QCefContextPrivate::~QCefContextPrivate() {}

bool
QCefContextPrivate::initialize(const QCefConfigPrivate* config)
{
  if (!initializeCef(config)) {
    return false;
  }

  // create cef client
  pClientDelegate_ = std::make_shared<CCefClientDelegate>();
  pClient_ = new CefViewBrowserClient(pClientDelegate_);

  // add archive mapping
  for (auto archiveMapping : archiveMappingList_) {
    pClient_->AddArchiveResourceProvider(
      archiveMapping.path.toStdString(), archiveMapping.url.toStdString(), archiveMapping.psw.toStdString());
  }

  // add local folder mapping
  for (auto folderMapping : folderMappingList_) {
    pClient_->AddLocalDirectoryResourceProvider(
      folderMapping.path.toStdString(), folderMapping.url.toStdString(), folderMapping.priority);
  }

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
QCefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}
