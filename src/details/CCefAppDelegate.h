#pragma once
#include <string>
#include <unordered_map>

#include <CefViewBrowserAppDelegate.h>

class QCefContextPrivate;

class CCefAppDelegate : public CefViewBrowserAppDelegateInterface
{
  typedef std::unordered_map<std::string, std::string> CommandLineArgs;

public:
  CCefAppDelegate(QCefContextPrivate* context, CommandLineArgs args);

  virtual void onBeforeCommandLineProcessing(const CefString& process_type,
                                             CefRefPtr<CefCommandLine> command_line) override;

  virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;

  void onScheduleMessageLoopWork(int64_t delay_ms) override;

private:
  QCefContextPrivate* pContext_;

  CommandLineArgs commandLineArgs_;
};
