#pragma once

#pragma region std_headers
#include <atomic>
#include <mutex>
#include <set>
#include <string>
#include <memory>
#pragma endregion std_headers

#pragma region qt_headers
#include <QObject>
#include <QTimer>
#include <QList>
#pragma endregion qt_headers

#include <CefViewBrowserClient.h>
#include <CefViewBrowserApp.h>

#include "CCefAppDelegate.h"
#include "CCefClientDelegate.h"
#include "QCefConfigPrivate.h"

/// <summary>
///
/// </summary>
class QCefContextPrivate : public QObject
{
  Q_OBJECT

private:
  typedef struct ArchiveMapping
  {
    QString path;
    QString url;
    QString psw;
  } ArchiveMapping;

  typedef struct FolderMapping
  {
    QString path;
    QString url;
    int priority;
  } FolderMapping;

private:
  QTimer cefWorkerTimer_;
  QList<FolderMapping> folderMappingList_;
  QList<ArchiveMapping> archiveMappingList_;

public:
  CefRefPtr<CefViewBrowserApp> pApp_;
  std::shared_ptr<CCefAppDelegate> pAppDelegate_;

  CefRefPtr<CefViewBrowserClient> pClient_;
  std::shared_ptr<CCefClientDelegate> pClientDelegate_;

public:
  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate();

  /// <summary>
  ///
  /// </summary>
  ~QCefContextPrivate();

  /// <summary>
  ///
  /// </summary>
  /// <param name="config"></param>
  /// <returns></returns>
  bool initialize(const QCefConfigPrivate* config);

  /// <summary>
  ///
  /// </summary>
  void uninitialize();

  /// <summary>
  ///
  /// </summary>
  /// <param name="delayMs"></param>
  void scheduleCefLoopWork(int64_t delayMs);

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
                       const std::string& url)
  {
    CefCookie cookie;
    CefString(&cookie.name).FromString(name);
    CefString(&cookie.value).FromString(value);
    CefString(&cookie.domain).FromString(domain);
    return CefCookieManager::GetGlobalManager(nullptr)->SetCookie(CefString(url), cookie, nullptr);
  }

public slots:
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
  bool initializeCef(const QCefConfigPrivate* config);

  /// <summary>
  ///
  /// </summary>
  void uninitializeCef();
};
