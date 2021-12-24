#pragma once

#pragma region stl_headers
#include <list>
#pragma endregion stl_headers

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
  static std::string bridge_object_name;

  /// <summary>
  ///
  /// </summary>
  static std::string browser_sub_process_path;

  /// <summary>
  ///
  /// </summary>
  static std::string resource_directory_path;

  /// <summary>
  ///
  /// </summary>
  static std::string locales_directory_path;

  /// <summary>
  ///
  /// </summary>
  static std::string user_agent;

  /// <summary>
  ///
  /// </summary>
  static std::string cache_path;

  /// <summary>
  ///
  /// </summary>
  static std::string user_data_path;

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
  static std::string locale;

  /// <summary>
  ///
  /// </summary>
  static int remote_debugging_port;

  /// <summary>
  ///
  /// </summary>
  static uint32_t background_color;

  /// <summary>
  ///
  /// </summary>
  static std::string accept_language_list;

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
