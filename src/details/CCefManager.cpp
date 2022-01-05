﻿#include "CCefManager.h"

#include <QCefContext.h>

#include "CCefSetting.h"

CCefManager::WeakPtr CCefManager::s_This_;

CCefManager::CCefManager() {}

CCefManager::~CCefManager() {}

bool
CCefManager::initialize(int argc, char* argv[], const QCefSetting& settings)
{
  s_This_ = WeakPtr(shared_from_this());

  if (!initializeCef(argc, argv, settings))
    return false;

  for (auto cookieItem : settings.d->cookieList_) {
    addCookie(cookieItem.name, cookieItem.value, cookieItem.domain, cookieItem.url);
  }

  return true;
}

void
CCefManager::uninitialize()
{
  uninitializeCef();

  s_This_.reset();
}

void
CCefManager::doCefWork()
{
  CefDoMessageLoopWork();
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

void
CCefManager::OnScheduleMessageLoopWork(int64_t delay_ms)
{
  auto p = QCefContext::instance();
  if (p) {
    p->scheduleMessageLoopWork(delay_ms);
  }
}