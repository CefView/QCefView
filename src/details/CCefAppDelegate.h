#pragma once
#include <CefViewBrowserAppDelegate.h>

class QCefContextPrivate;

class CCefAppDelegate : public CefViewBrowserAppDelegateInterface
{
public:
  CCefAppDelegate(QCefContextPrivate* context);

  void OnScheduleMessageLoopWork(int64_t delay_ms) override;

private:
  QCefContextPrivate* pContext_;
};
