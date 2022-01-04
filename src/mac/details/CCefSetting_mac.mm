#include "details/CCefSetting.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

CCefSetting::CCefSetting()
{
  user_agent.FromString(CEFVIEW_USER_AGENT);
}
