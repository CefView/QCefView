#pragma once

#pragma region stl_headers
#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#pragma endregion

#pragma region qt_headers
#include <QCoreApplication>
#include <QList>
#include <QObject>
#include <QTimer>
#pragma endregion

#include <CefViewBrowserApp.h>
#include <CefViewBrowserClient.h>

#include "CCefAppDelegate.h"
#include "QCefConfigPrivate.h"

/// <summary>
///
/// </summary>
class QCefContextPrivate : public QObject
{
  Q_OBJECT

private:
  /// <summary>
  ///
  /// </summary>
  int argc_;

  /// <summary>
  ///
  /// </summary>
  char** argv_;

  /// <summary>
  ///
  /// </summary>
  const QCefConfig* config_;

#if defined(Q_OS_MACOS) || defined(CEF_USE_QT_EVENT_LOOP)
  /// <summary>
  ///
  /// </summary>
  QTimer cefWorkerTimer_;
#endif

#if defined(Q_OS_WINDOWS)
  /// <summary>
  ///
  /// </summary>
  HANDLE windowsJobHandle_ = nullptr;

  /// <summary>
  ///
  /// </summary>
  QString windowsJobName_;
#endif

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserApp> pApp_;

  /// <summary>
  ///
  /// </summary>
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

  /// <summary>
  /// Adds a url mapping item with local archive (.zip) file which contains the web resource
  /// </summary>
  /// <param name="path">The path to the local archive file</param>
  /// <param name="url">The url to be mapped to</param>
  /// <param name="password">The password of the archive</param>
  /// <param name="priority">The priority</param>
  void addArchiveResource(const QString& path, const QString& url, const QString& password = "", int priority = 0);

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
