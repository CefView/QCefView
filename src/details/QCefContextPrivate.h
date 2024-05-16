#pragma once

#pragma region std_headers
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#pragma endregion std_headers

#pragma region qt_headers
#include <QCoreApplication>
#include <QList>
#include <QObject>
#include <QTimer>
#pragma endregion qt_headers

#include <CefViewBrowserApp.h>
#include <CefViewBrowserClient.h>

#include "CCefAppDelegate.h"
#include "QCefConfigPrivate.h"

typedef struct FolderResourceMapping
{
  QString path;
  QString url;
  int priority;
} FolderResourceMapping;

typedef struct ArchiveResourceMapping
{
  QString path;
  QString url;
  QString password;
  int priority;
} ArchiveResourceMapping;

/// <summary>
///
/// </summary>
class QCefContextPrivate : public QObject
{
  Q_OBJECT

private:
  int argc_;
  char** argv_;

private:
  const QCefConfig* config_;
  QList<FolderResourceMapping> folderResourceMappingList_;
  QList<ArchiveResourceMapping> archiveResourceMappingList_;

#if defined(Q_OS_MACOS) || defined(CEF_USE_QT_EVENT_LOOP)
  QTimer cefWorkerTimer_;
#endif

#if defined(Q_OS_WINDOWS)
  HANDLE windowsJobHandle_ = nullptr;
  QString windowsJobName_;
#endif

private:
  CefRefPtr<CefViewBrowserApp> pApp_;
  CCefAppDelegate::RefPtr pAppDelegate_;

public:
  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate(QCoreApplication* app, int argc, char** argv);

  /// <summary>
  ///
  /// </summary>
  ~QCefContextPrivate();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  CefRefPtr<CefViewBrowserApp> getCefApp();

  /// <summary>
  ///
  /// </summary>
  /// <param name="config"></param>
  /// <returns></returns>
  bool initialize(const QCefConfig* config);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QCefConfig* cefConfig() const;

  /// <summary>
  /// Adds a url mapping item with local web resource directory
  /// </summary>
  /// <param name="path">The path to the local resource directory</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="priority">The priority</param>
  void addLocalFolderResource(const QString& path, const QString& url, int priority = 0);
  const QList<FolderResourceMapping>& folderResourceMappingList();

  /// <summary>
  /// Adds a url mapping item with local archive (.zip) file which contains the web resource
  /// </summary>
  /// <param name="path">The path to the local archive file</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="password">The password of the archive</param>
  /// <param name="priority">The priority</param>
  void addArchiveResource(const QString& path, const QString& url, const QString& password = "", int priority = 0);
  const QList<ArchiveResourceMapping>& archiveResourceMappingList();

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="value"></param>
  /// <param name="domain"></param>
  /// <param name="url"></param>
  /// <returns></returns>
  bool addGlobalCookie(const std::string& name,
                       const std::string& value,
                       const std::string& domain,
                       const std::string& url);

  /// <summary>
  ///
  /// </summary>
  bool deleteAllCookies();

  /// <summary>
  ///
  /// </summary>
  /// <param name="sourceOrigin"></param>
  /// <param name="targetSchema"></param>
  /// <param name="targetDomain"></param>
  /// <param name="allowTargetSubdomains"></param>
  bool addCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                    const QString& targetSchema,
                                    const QString& targetDomain,
                                    bool allowTargetSubdomains);

  /// <summary>
  ///
  /// </summary>
  /// <param name="sourceOrigin"></param>
  /// <param name="targetSchema"></param>
  /// <param name="targetDomain"></param>
  /// <param name="allowTargetSubdomains"></param>
  bool removeCrossOriginWhitelistEntry(const QString& sourceOrigin,
                                       const QString& targetSchema,
                                       const QString& targetDomain,
                                       bool allowTargetSubdomains);

  /// <summary>
  ///
  /// </summary>
  bool clearCrossOriginWhitelistEntry();

  /// <summary>
  ///
  /// </summary>
  void uninitialize();

  /// <summary>
  ///
  /// </summary>
  /// <param name="delayMs"></param>
  void scheduleCefLoopWork(int64_t delayMs);

public slots:
  /// <summary>
  ///
  /// </summary>
  void onAboutToQuit();

  /// <summary>
  ///
  /// </summary>
  void performCefLoopWork();

protected:
  /// <summary>
  ///
  /// </summary>
  /// <param name="argc"></param>
  /// <param name="argv"></param>
  /// <param name="config"></param>
  /// <returns></returns>
  bool initializeCef(const QCefConfig* config);

  /// <summary>
  ///
  /// </summary>
  void uninitializeCef();
};
