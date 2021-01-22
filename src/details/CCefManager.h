#pragma once

#pragma region std_headers
#include <mutex>
#pragma endregion std_headers

#include <CefViewBrowserApp.h>

/// <summary>
///
/// </summary>
class CCefManager
{

protected:
  /// <summary>
  ///
  /// </summary>
  CCefManager();

  /// <summary>
  ///
  /// </summary>
  ~CCefManager(){};

public:
  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  static CCefManager& getInstance();

  /// <summary>
  ///
  /// </summary>
  void initializeCef();

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
  void uninitializeCef();

protected:
  /// <summary>
  ///
  /// </summary>
  void releaseCef();

private:
  /// <summary>
  ///
  /// </summary>
  CefRefPtr<CefViewBrowserApp> app_;

  /// <summary>
  ///
  /// </summary>
  CefSettings cef_settings_;

  /// <summary>
  ///
  /// </summary>
  int64_t nBrowserRefCount_;
};
