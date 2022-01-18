#pragma once

#pragma region stl_headers
#include <list>
#include <optional>
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
  enum LogLevel
  {
    ///
    // Default logging (currently INFO logging).
    ///
    LOGSEVERITY_DEFAULT,

    ///
    // Verbose logging.
    ///
    LOGSEVERITY_VERBOSE,

    ///
    // DEBUG logging.
    ///
    LOGSEVERITY_DEBUG = LOGSEVERITY_VERBOSE,

    ///
    // INFO logging.
    ///
    LOGSEVERITY_INFO,

    ///
    // WARNING logging.
    ///
    LOGSEVERITY_WARNING,

    ///
    // ERROR logging.
    ///
    LOGSEVERITY_ERROR,

    ///
    // FATAL logging.
    ///
    LOGSEVERITY_FATAL,

    ///
    // Disable logging to file for all messages, and to stderr for messages with
    // severity less than FATAL.
    ///
    LOGSEVERITY_DISABLE = 99
  };

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

  QVariant backgroundColor_;        // <uint32_t>
  QVariant remoteDebuggingport_;    //<short>
  QVariant persistSessionCookies_;  //<bool>
  QVariant persistUserPreferences_; //<bool>
  QVariant logLevel_;               //<int>
};
