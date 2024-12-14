#include "CCefAppDelegate.h"

#include <QDebug>

#include "QCefContextPrivate.h"

CCefAppDelegate::CCefAppDelegate(QCefContextPrivate* context, CommandLineArgs args)
  : pContext_(context)
  , commandLineArgs_(args)
{
}

CCefAppDelegate::~CCefAppDelegate()
{
  qDebug() << "CCefAppDelegate is being destructed";
}

void
CCefAppDelegate::onBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
  for (auto& kv : commandLineArgs_) {
    if (!kv.first.empty()) {
      if (!kv.second.empty())
        command_line->AppendSwitchWithValue(kv.first, kv.second);
      else
        command_line->AppendSwitch(kv.first);
    }
  }
}

void
CCefAppDelegate::onBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  for (auto& kv : commandLineArgs_) {
    if (!kv.first.empty()) {
      if (!kv.second.empty())
        command_line->AppendSwitchWithValue(kv.first, kv.second);
      else
        command_line->AppendSwitch(kv.first);
    }
  }
}

void
CCefAppDelegate::onScheduleMessageLoopWork(int64_t delay_ms)
{
  if (pContext_) {
    pContext_->scheduleCefLoopWork(delay_ms);
  }
}
