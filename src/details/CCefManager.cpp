#include "CCefManager.h"

#include "CCefSetting.h"

std::weak_ptr<CCefManager> CCefManager::s_This;

CCefManager::CCefManager(int argc, char* argv[], const QCefSetting& settings)
  : is_exiting_(false)
{
  initializeCef(argc, argv, settings);
}

CCefManager::~CCefManager()
{
  uninitializeCef();
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
