#pragma once

#pragma region stl_headers
#include <list>
#pragma endregion stl_headers

#pragma region qt_headers
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/internal/cef_types_wrappers.h>
#pragma endregion cef_headers

#include <QCefSetting.h>

typedef struct CookieItem
{
  std::string name;
  std::string value;
  std::string domain;
  std::string url;
} CookieItem;

class CCefSetting
{
public:
  explicit CCefSetting();

  CefString locale_;
  CefString userAgent_;
  CefString cachePath_;
  CefString userDataPath_;
  CefString bridgeObjectName_;
  CefString acceptLanguageList_;
  CefString localesDirectoryPath_;
  CefString browserSubProcessPath_;
  CefString resourceDirectoryPath_;
  uint32_t backgroundColor_;
  short remoteDebuggingport_;

  bool persistSessionCookies_;
  bool persistUserPreferences_;

  std::list<CookieItem> cookieList_;
};
