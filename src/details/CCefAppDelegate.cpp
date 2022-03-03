#include "CCefAppDelegate.h"

#include "QCefContextPrivate.h"

CCefAppDelegate::CCefAppDelegate(QCefContextPrivate* context)
  : pContext_(context)
{}

void
CCefAppDelegate::onBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
  // set switches
  if (process_type.empty()) {
    command_line->AppendSwitch("use-mock-keychain");
    command_line->AppendSwitch("disable-spell-checking");
    command_line->AppendSwitch("disable-remote-core-animation");
    command_line->AppendSwitch("disable-site-isolation-trials");
    command_line->AppendSwitch("enable-aggressive-domstorage-flushing");

    // set switches with value
    command_line->AppendSwitchWithValue("renderer-process-limit", "1");
    command_line->AppendSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");
  }
}

void
CCefAppDelegate::onScheduleMessageLoopWork(int64_t delay_ms)
{
  if (pContext_) {
    pContext_->scheduleCefLoopWork(delay_ms);
  }
}
