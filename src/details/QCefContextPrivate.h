#pragma once

#pragma region std_headers
#include <atomic>
#include <memory>
#include <mutex>
#include <set>
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
#include "CCefClientDelegate.h"
#include "QCefConfigPrivate.h"

/// <summary>
///
/// </summary>
class QCefContextPrivate : public QObject
{
  Q_OBJECT

private:
  QTimer cefWorkerTimer_;

public:
  CefRefPtr<CefViewBrowserApp> pApp_;
  std::shared_ptr<CCefAppDelegate> pAppDelegate_;

  CefRefPtr<CefViewBrowserClient> pClient_;
  std::shared_ptr<CCefClientDelegate> pClientDelegate_;

public:
  /// <summary>
  ///
  /// </summary>
  QCefContextPrivate(QCoreApplication* app);

  /// <summary>
  ///
  /// </summary>
  ~QCefContextPrivate();

  /// <summary>
  ///
  /// </summary>
  /// <param name="config"></param>
  /// <returns></returns>
  bool initialize(const QCefConfig* config);

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
