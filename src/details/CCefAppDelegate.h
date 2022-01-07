#pragma once
#pragma region qt_headers
#include <QWindow>
#pragma endregion qt_headers

#pragma region std_headers
#include <memory>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#include <CefViewBrowserAppDelegate.h>

#include "QCefContextPrivate.h"

class CCefAppDelegate : public CefViewBrowserAppDelegateInterface
{
public:
  explicit CCefAppDelegate(QCefContextPrivate* context);

  void OnScheduleMessageLoopWork(int64_t delay_ms) override;

private:
  QCefContextPrivate* pContext_;
};
