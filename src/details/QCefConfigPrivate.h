#pragma once

#pragma region stl_headers
#include <list>
#include <string>
#include <unordered_map>
#pragma endregion stl_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#pragma region qt_headers
#include <QVariant>
#pragma endregion qt_headers

#include <QCefConfig.h>

class QCefConfigPrivate
{
  typedef std::unordered_map<std::string, std::string> ArgsMap;

public:
  explicit QCefConfigPrivate();

  static void CopyToCefSettings(const QCefConfig* config, CefSettings* settings);

  static ArgsMap& GetCommandLineArgs(const QCefConfig* config);

public:
  std::string locale_;
  std::string userAgent_;
  std::string cachePath_;

#if CEF_VERSION_MAJOR < 115
  std::string userDataPath_;
#else
  std::string rootCachePath_;
#endif

  std::string bridgeObjectName_;
  std::string acceptLanguageList_;

#if !defined(Q_OS_MACOS)
  std::string localesDirectoryPath_;
  std::string browserSubProcessPath_;
  std::string resourceDirectoryPath_;
#endif

  int logLevel_ = LOGSEVERITY_DEFAULT;

  // Enable OSR mode by default
  /* bool   */ QVariant windowlessRenderingEnabled_ = true;

  /* bool   */ QVariant persistSessionCookies_;
  /* bool   */ QVariant persistUserPreferences_;
  /* short  */ QVariant remoteDebuggingport_;
  /* QColor */ QVariant backgroundColor_;

  ArgsMap commandLineArgs_;
};
