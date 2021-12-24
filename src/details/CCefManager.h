#pragma once

#pragma region std_headers
#include <atomic>
#include <mutex>
#include <set>
#include <string>
#pragma endregion std_headers

#include <CefViewBrowserHandler.h>
#include <CefViewBrowserApp.h>

/// <summary>
///
/// </summary>
class CCefManager
{
public:
  /// <summary>
  ///
  /// </summary>
  std::string cacheRootPath;

  /// <summary>
  ///
  /// </summary>
  std::string cachePath;

  /// <summary>
  ///
  /// </summary>
  std::string bridgeObjectName;

  /// <summary>
  ///
  /// </summary>
  uint32_t backgroundColor;

  /// <summary>
  ///
  /// </summary>
  uint16_t debugPort;

public:
  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static CCefManager& getInstance();

  /// <summary>
  ///
  /// </summary>
  bool initializeCef(int argc, const char* argv[]);

  /// <summary>
  ///
  /// </summary>
  void uninitializeCef();

  /// <summary>
  ///
  /// </summary>
  bool isInitialized();

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

protected:
  /// <summary>
  ///
  /// </summary>
  CCefManager();

  /// <summary>
  ///
  /// </summary>
  ~CCefManager() {}

private:
  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserApp> app_;

  /// <summary>
  ///
  /// </summary>
  std::atomic_bool is_initialized_;

  /// <summary>
  ///
  /// </summary>
  std::mutex init_locker_;

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
