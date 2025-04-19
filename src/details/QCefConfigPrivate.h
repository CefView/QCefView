#pragma once

#pragma region stl_headers
#include <list>
#include <string>
#include <unordered_map>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

#pragma region qt_headers
#include <QVariant>
#pragma endregion

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
  std::string builtinSchemeName_;
  std::string acceptLanguageList_;

#if !defined(Q_OS_MACOS)
  std::string localesDirectoryPath_;
  std::string browserSubProcessPath_;
  std::string resourceDirectoryPath_;
#endif

  int logLevel_ = LOGSEVERITY_DEFAULT;

  // Enable OSR mode by default
  /* bool   */ QVariant windowlessRenderingEnabled_ = true;

  // Enable command line pass through by default
  /* bool   */ QVariant commandLinePassthroughDisabled_ = false;

  /* bool   */ QVariant persistSessionCookies_;
#if CEF_VERSION_MAJOR < 128
  /* bool   */ QVariant persistUserPreferences_;
#endif
  /* short  */ QVariant remoteDebuggingport_;
  /* QColor */ QVariant backgroundColor_;

  ArgsMap commandLineArgs_;
};
