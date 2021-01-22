#include "CCefSetting.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

CefString CCefSetting::bridge_object_name;

CefString CCefSetting::browser_sub_process_path;

CefString CCefSetting::resource_directory_path;

CefString CCefSetting::locales_directory_path;

CefString CCefSetting::user_agent;

CefString CCefSetting::cache_path;

CefString CCefSetting::user_data_path;

int CCefSetting::persist_session_cookies;

int CCefSetting::persist_user_preferences;

CefString CCefSetting::locale;

int CCefSetting::remote_debugging_port = 9999;

cef_color_t CCefSetting::background_color;

CefString CCefSetting::accept_language_list;

std::list<CCefSetting::CookieItem> CCefSetting::global_cookie_list;

void
CCefSetting::initializeInstance()
{
  static CCefSetting s_instance;
}
