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

  static void setInstnace(const std::shared_ptr<CCefManager>& s) { s_This = s; }

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static std::shared_ptr<CCefManager> getInstance()
  {
    if (!s_This.expired()) {
      auto p = s_This.lock();
      return p;
    }
    return nullptr;
  }

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
  static inline std::weak_ptr<CCefManager> s_This;

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
