#include "QCefConfigPrivate.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#include <QString>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

QCefConfigPrivate::QCefConfigPrivate()
{
  userAgent_ = CEFVIEW_USER_AGENT;

#if !defined(OS_MACOS)
  QDir ExeDir = QCoreApplication::applicationDirPath();

  QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
  browserSubProcessPath_ = QDir::toNativeSeparators(strExePath).toStdString();

  QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);
  resourceDirectoryPath_ = QDir::toNativeSeparators(strResPath).toStdString();

  QDir ResPath(strResPath);
  localesDirectoryPath_ = QDir::toNativeSeparators(ResPath.filePath(LOCALES_DIRECTORY_NAME)).toStdString();
#endif
}

void
QCefConfigPrivate::CopyToCefSettings(CefSettings& settings) const
{
#if !defined(OS_MACOS)
  if (!browserSubProcessPath_.empty())
    CefString(&settings.browser_subprocess_path) = browserSubProcessPath_;

  if (!resourceDirectoryPath_.empty())
    CefString(&settings.resources_dir_path) = resourceDirectoryPath_;

  if (!localesDirectoryPath_.empty())
    CefString(&settings.locales_dir_path) = localesDirectoryPath_;
#endif

  if (!userAgent_.empty())
    CefString(&settings.user_agent) = userAgent_;

  if (!cachePath_.empty())
    CefString(&settings.cache_path) = cachePath_;

  if (!userDataPath_.empty())
    CefString(&settings.user_data_path) = userDataPath_;

  if (!locale_.empty())
    CefString(&settings.locale) = locale_;

  if (!acceptLanguageList_.empty())
    CefString(&settings.accept_language_list) = acceptLanguageList_;

  if (persistSessionCookies_.canConvert<int>())
    settings.persist_session_cookies = persistSessionCookies_.toInt();

  if (persistUserPreferences_.canConvert<int>())
    settings.persist_user_preferences = persistUserPreferences_.toInt();

  if (backgroundColor_.canConvert<QColor>())
    settings.background_color = backgroundColor_.value<QColor>().rgba();

  if (remoteDebuggingport_.canConvert<int>())
    settings.remote_debugging_port = remoteDebuggingport_.toInt();

  settings.log_severity = (cef_log_severity_t)logLevel_;
}
