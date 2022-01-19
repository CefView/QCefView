#pragma once

#pragma region stl_headers
#include <list>
#include <string>
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
public:
  explicit QCefConfigPrivate();

  void CopyToCefSettings(CefSettings& settings) const;

  int argc;
  char** argv;

  std::string locale_;
  std::string userAgent_;
  std::string cachePath_;
  std::string userDataPath_;
  std::string bridgeObjectName_;
  std::string acceptLanguageList_;

#if !defined(OS_MACOS)
  std::string localesDirectoryPath_;
  std::string browserSubProcessPath_;
  std::string resourceDirectoryPath_;
#endif

  int logLevel_ = LOGSEVERITY_DEFAULT;

  /* bool   */ QVariant persistSessionCookies_;
  /* bool   */ QVariant persistUserPreferences_;
  /* short  */ QVariant remoteDebuggingport_;
  /* QColor */ QVariant backgroundColor_;
};
