#pragma once

#pragma region std_headers
#include <atomic>
#include <mutex>
#include <set>
#include <string>
#include <memory>
#pragma endregion std_headers

#include <CefViewBrowserHandler.h>
#include <CefViewBrowserApp.h>

#include <QCefSetting.h>

/// <summary>
///
/// </summary>
class CCefManager
{
public:
  /// <summary>
  ///
  /// </summary>
  CCefManager(int argc, char* argv[], const QCefSetting& settings);

  /// <summary>
  ///
  /// </summary>
  ~CCefManager();

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static CCefManager* getInstance() { return s_This; }

  /// <summary>
  ///
  /// </summary>
  bool initializeCef(int argc, char* argv[], const QCefSetting& settings);

  /// <summary>
  ///
  /// </summary>
  void uninitializeCef();

  /// <summary>
  ///
  /// </summary>
  void doCefWork();

  /// <summary>
  ///
  /// </summary>
  bool addCookie(const std::string& name, const std::string& value, const std::string& domain, const std::string& url);

  /// <summary>
  ///
  /// </summary>
  void registerBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler);

  /// <summary>
  ///
  /// </summary>
  void removeBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler);

  /// <summary>
  ///
  /// </summary>
  void closeAllBrowserHandler();

private:
  static CCefManager* s_This;

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

  /// <summary>
  ///
  /// </summary>
  bool is_exiting_;
};

typedef std::shared_ptr<CCefManager> CCefManagerPtr;
