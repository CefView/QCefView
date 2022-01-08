#include "CCefAppDelegate.h"

CCefAppDelegate::CCefAppDelegate(QCefContextPrivate* context)
  : pContext_(context)
{}

void
CCefAppDelegate::OnScheduleMessageLoopWork(int64_t delay_ms)
{
  if (pContext_) {
    pContext_->scheduleCefLoopWork(delay_ms);
  }
}
