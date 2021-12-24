#include "CCefManager.h"

#include "CCefSetting.h"

CCefManager::CCefManager()
  : bridgeObjectName()
  , debugPort(0)
  , backgroundColor(-1)
  , is_initialized_(false)
  , is_exiting_(false)
{}

CCefManager&
CCefManager::getInstance()
{
  static CCefManager s_instance;
  return s_instance;
}

bool
CCefManager::isInitialized()
{
  std::lock_guard<std::mutex> lock(init_locker_);
  return is_initialized_;
}

bool
CCefManager::addCookie(const std::string& name,
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

void
CCefManager::registerBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler)
{
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  handler_set_.insert(handler);
}
