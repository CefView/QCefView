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
  if (!this->browserSubProcessPath_.empty())
    CefString(&settings.browser_subprocess_path) = this->browserSubProcessPath_;

  if (!this->resourceDirectoryPath_.empty())
    CefString(&settings.resources_dir_path) = this->resourceDirectoryPath_;

  if (!this->localesDirectoryPath_.empty())
    CefString(&settings.locales_dir_path) = this->localesDirectoryPath_;
#endif

  if (!this->userAgent_.empty())
    CefString(&settings.user_agent) = this->userAgent_;

  if (!this->cachePath_.empty())
    CefString(&settings.cache_path) = this->cachePath_;

  if (!this->userDataPath_.empty())
    CefString(&settings.user_data_path) = this->userDataPath_;

  if (!this->locale_.empty())
    CefString(&settings.locale) = this->locale_;

  if (!this->acceptLanguageList_.empty())
    CefString(&settings.accept_language_list) = this->acceptLanguageList_;

  if (this->persistSessionCookies_.canConvert<int>())
    settings.persist_session_cookies = this->persistSessionCookies_.toInt();

  if (this->persistUserPreferences_.canConvert<int>())
    settings.persist_user_preferences = this->persistUserPreferences_.toInt();

  if (this->backgroundColor_.canConvert<QColor>())
    settings.background_color = this->backgroundColor_.value<QColor>().value();

  if (this->remoteDebuggingport_.canConvert<int>())
    settings.remote_debugging_port = this->remoteDebuggingport_.toInt();

  if (this->logLevel_.canConvert<int>())
    settings.log_severity = (cef_log_severity_t)this->logLevel_.toInt();
}
