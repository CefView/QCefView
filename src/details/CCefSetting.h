#pragma once

#pragma region stl_headers
#include <list>
#pragma endregion stl_headers

#pragma region cef_headers
#include <include/cef_base.h>
#pragma endregion cef_headers

/// <summary>
///
/// </summary>
class CCefSetting
{
protected:
  /// <summary>
  ///
  /// </summary>
  CCefSetting();

  /// <summary>
  ///
  /// </summary>
  ~CCefSetting(){};

public:
  /// <summary>
  ///
  /// </summary>
  static void initializeInstance();

public:
  /// <summary>
  ///
  /// </summary>
  static CefString bridge_object_name;

  /// <summary>
  ///
  /// </summary>
  static CefString browser_sub_process_path;

  /// <summary>
  ///
  /// </summary>
  static CefString resource_directory_path;

  /// <summary>
  ///
  /// </summary>
  static CefString locales_directory_path;

  /// <summary>
  ///
  /// </summary>
  static CefString user_agent;

  /// <summary>
  ///
  /// </summary>
  static CefString cache_path;

  /// <summary>
  ///
  /// </summary>
  static CefString user_data_path;

  /// <summary>
  ///
  /// </summary>
  static int persist_session_cookies;

  /// <summary>
  ///
  /// </summary>
  static int persist_user_preferences;

  /// <summary>
  ///
  /// </summary>
  static CefString locale;

  /// <summary>
  ///
  /// </summary>
  static int remote_debugging_port;

  /// <summary>
  ///
  /// </summary>
  static cef_color_t background_color;

  /// <summary>
  ///
  /// </summary>
  static CefString accept_language_list;

  /// <summary>
  ///
  /// </summary>
  typedef struct CookieItem
  {
    std::string name;
    std::string value;
    std::string domain;
    std::string url;
  } CookieItem;
  static std::list<CookieItem> global_cookie_list;
};
