#pragma once

#pragma region std_headers
#include <atomic>
#include <mutex>
#include <set>
#include <string>
#include <memory>
#pragma endregion std_headers

#include <CefViewBrowserAppDelegate.h>
#include <CefViewBrowserHandler.h>
#include <CefViewBrowserApp.h>

#include "QCefSettingPrivate.h"

/// <summary>
///
/// </summary>
class CCefManager
  : public CefViewBrowserAppDelegateInterface
  , public std::enable_shared_from_this<CCefManager>
{
public:
  /// <summary>
  ///
  /// </summary>
  typedef std::shared_ptr<CCefManager> RefPtr;

  /// <summary>
  ///
  /// </summary>
  typedef std::weak_ptr<CCefManager> WeakPtr;

  /// <summary>
  ///
  /// </summary>
  CCefManager();

  /// <summary>
  ///
  /// </summary>
  ~CCefManager();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static RefPtr getInstance() { return s_This_.lock(); }

  /// <summary>
  ///
  /// </summary>
  /// <param name="settings"></param>
  /// <returns></returns>
  bool initialize(const QCefSettingPrivate* settings);

  /// <summary>
  ///
  /// </summary>
  void uninitialize();

  /// <summary>
  ///
  /// </summary>
  void doCefWork();

  /// <summary>
  ///
  /// </summary>
  /// <param name="name"></param>
  /// <param name="value"></param>
  /// <param name="domain"></param>
  /// <param name="url"></param>
  /// <returns></returns>
  bool addCookie(const std::string& name, const std::string& value, const std::string& domain, const std::string& url);

  /// <summary>
  ///
  /// </summary>
  /// <param name="handler"></param>
  void registerBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler);

  /// <summary>
  ///
  /// </summary>
  /// <param name="handler"></param>
  void removeBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler);

  /// <summary>
  ///
  /// </summary>
  void closeAllBrowserHandler();

  /// <summary>
  ///
  /// </summary>
  /// <param name="delay_ms"></param>
  void OnScheduleMessageLoopWork(int64_t delay_ms) override;

protected:
  /// <summary>
  ///
  /// </summary>
  /// <param name="argc"></param>
  /// <param name="argv"></param>
  /// <param name="settings"></param>
  /// <returns></returns>
  bool initializeCef(const QCefSettingPrivate* settings);

  /// <summary>
  ///
  /// </summary>
  void uninitializeCef();

private:
  /// <summary>
  ///
  /// </summary>
  static WeakPtr s_This_;

  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserApp> app_;

  /// <summary>
  ///
  /// </summary>
  std::mutex handler_set_locker_;

  /// <summary>
  ///
  /// </summary>
  std::set<CefRefPtr<CefViewBrowserHandler>> handler_set_;
};
