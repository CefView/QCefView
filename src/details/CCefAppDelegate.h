#pragma once
#include <CefViewBrowserAppDelegate.h>

class QCefContextPrivate;

class CCefAppDelegate : public CefViewBrowserAppDelegateInterface
{
public:
  CCefAppDelegate(QCefContextPrivate* context);

  virtual void onBeforeCommandLineProcessing(const CefString& process_type,
                                             CefRefPtr<CefCommandLine> command_line) override;

  void onScheduleMessageLoopWork(int64_t delay_ms) override;

private:
  QCefContextPrivate* pContext_;
};
