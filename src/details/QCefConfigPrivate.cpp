#include "QCefConfigPrivate.h"

#pragma region qt_headers
#include <QCoreApplication>
#include <QDir>
#include <QString>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>
#include <CefViewWingProcessName.h>

QCefConfigPrivate::QCefConfigPrivate()
{
  backgroundColor_ = QColor::fromRgba(qRgba(255, 255, 255, 255));
  userAgent_ = kCefViewDefaultUserAgent;

#if !defined(Q_OS_MACOS)
  QDir ExeDir = QCoreApplication::applicationDirPath();

  QString strExePath = ExeDir.filePath(kCefViewRenderProcessName);
  browserSubProcessPath_ = QDir::toNativeSeparators(strExePath).toStdString();

  QString strResPath = ExeDir.filePath(kCefViewResourceDirectoryName);
  resourceDirectoryPath_ = QDir::toNativeSeparators(strResPath).toStdString();

  QDir ResPath(strResPath);
  localesDirectoryPath_ = QDir::toNativeSeparators(ResPath.filePath(kCefViewLocalesDirectoryName)).toStdString();
#endif
}

void
QCefConfigPrivate::CopyToCefSettings(const QCefConfig* config, CefSettings* settings)
{
  // validate input target setting parameter
  if (!settings) {
    // nothing to do but return
    return;
  }

  // validate the input source config parameter
  if (!config) {
    // just copy the mandatory fields
    QCefConfigPrivate cfg;

    settings->background_color = cfg.backgroundColor_.value<QColor>().rgba();
    CefString(&settings->user_agent) = cfg.userAgent_;

#if !defined(Q_OS_MACOS)
    CefString(&settings->browser_subprocess_path) = cfg.browserSubProcessPath_;
    CefString(&settings->resources_dir_path) = cfg.resourceDirectoryPath_;
    CefString(&settings->locales_dir_path) = cfg.localesDirectoryPath_;
#endif
    return;
  }

  if (config->d_ptr->windowlessRenderingEnabled_.canConvert<int>())
    settings->windowless_rendering_enabled = config->d_ptr->windowlessRenderingEnabled_.toInt();

#if !defined(Q_OS_MACOS)
  if (!config->d_ptr->browserSubProcessPath_.empty())
    CefString(&settings->browser_subprocess_path) = config->d_ptr->browserSubProcessPath_;

  if (!config->d_ptr->resourceDirectoryPath_.empty())
    CefString(&settings->resources_dir_path) = config->d_ptr->resourceDirectoryPath_;

  if (!config->d_ptr->localesDirectoryPath_.empty())
    CefString(&settings->locales_dir_path) = config->d_ptr->localesDirectoryPath_;
#endif

  if (!config->d_ptr->userAgent_.empty())
    CefString(&settings->user_agent) = config->d_ptr->userAgent_;

  if (!config->d_ptr->cachePath_.empty())
    CefString(&settings->cache_path) = config->d_ptr->cachePath_;

#if CEF_VERSION_MAJOR < 115
  if (!config->d_ptr->userDataPath_.empty())
    CefString(&settings->user_data_path) = config->d_ptr->userDataPath_;
#else
  if (!config->d_ptr->rootCachePath_.empty())
    CefString(&settings->root_cache_path) = config->d_ptr->rootCachePath_;
#endif

  if (!config->d_ptr->locale_.empty())
    CefString(&settings->locale) = config->d_ptr->locale_;

  if (!config->d_ptr->acceptLanguageList_.empty())
    CefString(&settings->accept_language_list) = config->d_ptr->acceptLanguageList_;

  if (config->d_ptr->persistSessionCookies_.canConvert<int>())
    settings->persist_session_cookies = config->d_ptr->persistSessionCookies_.toInt();

  if (config->d_ptr->persistUserPreferences_.canConvert<int>())
    settings->persist_user_preferences = config->d_ptr->persistUserPreferences_.toInt();

  if (config->d_ptr->backgroundColor_.canConvert<QColor>())
    settings->background_color = config->d_ptr->backgroundColor_.value<QColor>().rgba();

  if (config->d_ptr->remoteDebuggingport_.canConvert<int>())
    settings->remote_debugging_port = config->d_ptr->remoteDebuggingport_.toInt();

  settings->log_severity = (cef_log_severity_t)config->d_ptr->logLevel_;
}

QCefConfigPrivate::ArgsMap&
QCefConfigPrivate::GetCommandLineArgs(const QCefConfig* config)
{
  if (!config) {
    static ArgsMap emptyArgs;
    return emptyArgs;
  }

  return config->d_ptr->commandLineArgs_;
}
