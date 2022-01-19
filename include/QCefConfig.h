#ifndef QCEFCONFIG_H
#define QCEFCONFIG_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QColor>
#include <QScopedPointer>
#include <QString>
#include <QVariant>
#pragma endregion qt_headers

class QCefConfigPrivate;

/// <summary>
/// Represents the CEF setting. For more details please refer to:
/// https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-cefsettings
/// </summary>
class QCEFVIEW_EXPORT QCefConfig
{
  Q_DECLARE_PRIVATE(QCefConfig)
  QScopedPointer<QCefConfigPrivate> d_ptr;

  friend class QCefContext;

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
  /// <summary>
  /// Constructs a CEF config instance
  /// </summary>
  /// <param name="argc">The application arguments count</param>
  /// <param name="argv">The application arguments list</param>
  QCefConfig(int argc, char* argv[]);

  /// <summary>
  /// Constructs a CEF setting from existing one
  /// </summary>
  QCefConfig(const QCefConfig& other);

  /// <summary>
  /// Assigns an existing config to current
  /// </summary>
  QCefConfig& operator=(const QCefConfig& other);

  /// <summary>
  /// Destructs the config
  /// </summary>
  ~QCefConfig();

#if !defined(OS_MACOS)
  /// <summary>
  /// Sets the browser subprocess path
  /// </summary>
  /// <param name="path">The path to the sub process executable</param>
  void setBrowserSubProcessPath(const QString& path);

  /// <summary>
  /// Gets the browser subprocess path
  /// </summary>
  const QString browserSubProcessPath() const;

  /// <summary>
  /// Sets the resource directory path
  /// </summary>
  /// <param name="path">The resource directory path</param>
  void setResourceDirectoryPath(const QString& path);

  /// <summary>
  /// Gets the resource directory path
  /// </summary>
  const QString resourceDirectoryPath() const;

  /// <summary>
  /// Sets the locales directory path
  /// </summary>
  /// <param name="path">The locales directory path</param>
  void setLocalesDirectoryPath(const QString& path);

  /// <summary>
  /// Gets the locales directory path
  /// </summary>
  const QString localesDirectoryPath() const;
#endif

  /// <summary>
  /// Sets the log level
  /// </summary>
  /// <param name="lvl"></param>
  void setLogLevel(const LogLevel lvl);

  /// <summary>
  /// Gets the log level
  /// </summary>
  /// <returns>The current log level</returns>
  const QCefConfig::LogLevel logLevel() const;

  /// <summary>
  /// Sets the locale
  /// </summary>
  /// <param name="locale">The locale to use. If empty the default locale of "en-US" will be used. This value is ignored
  /// on Linux where locale is determined using environment variable parsing with the precedence order: LANGUAGE,
  /// LC_ALL, LC_MESSAGES and LANG. Also configurable using the "lang" command-line switch.</param>
  void setLocale(const QString& locale);

  /// <summary>
  /// Gets the locale
  /// </summary>
  const QString locale() const;

  /// <summary>
  /// Sets the user agent
  /// </summary>
  /// <param name="agent">The user agent</param>
  void setUserAgent(const QString& agent);

  /// <summary>
  /// Gets the user agent
  /// </summary>
  const QString userAgent() const;

  /// <summary>
  /// Sets the cache directory path
  /// </summary>
  /// <param name="path">The cache path</param>
  void setCachePath(const QString& path);

  /// <summary>
  /// Gets the cache directory path
  /// </summary>
  const QString cachePath() const;

  /// <summary>
  /// Sets the user data directory path
  /// </summary>
  /// <param name="path">The user data directory path</param>
  void setUserDataPath(const QString& path);

  /// <summary>
  /// Gets the user data directory path
  /// </summary>
  const QString userDataPath() const;

  /// <summary>
  /// Sets the bridge object name
  /// </summary>
  /// <param name="name">The bridge object name</param>
  /// <remarks>
  /// The bridge object represents a Javascript object which will be inserted
  /// into all browser and frames. This object is designated for communicating
  /// between Javascript in web content and native context(C/C++) code.
  /// This object is set as an property of window object. That means it can be
  /// obtained by calling window.bridgeObject in the Javascript code.
  /// </remarks>
  void setBridgeObjectName(const QString& name);

  /// <summary>
  /// Gets the bridge object name
  /// </summary>
  const QString bridgeObjectName() const;

  /// <summary>
  /// Sets the background color of the web page
  /// </summary>
  /// <param name="color">The color to be set</param>
  void setBackgroundColor(const QColor& color);

  /// <summary>
  /// Gets the background color
  /// </summary>
  const QVariant backgroundColor() const;

  /// <summary>
  /// Sets the acceptable language list
  /// </summary>
  /// <param name="languages"></param>
  void setAcceptLanguageList(const QString& languages);

  /// <summary>
  /// Get the acceptable language list
  /// </summary>
  const QString acceptLanguageList() const;

  /// <summary>
  /// Sets whether to persist session cookie
  /// </summary>
  /// <param name="enabled">True if to perssit session cookie</param>
  void setPersistSessionCookies(bool enabled);

  /// <summary>
  /// Gets whether to persist session cookie
  /// </summary>
  const QVariant persistSessionCookies() const;

  /// <summary>
  /// Sets whether to persist user preferences
  /// </summary>
  /// <param name="enabled">True if to persist user preferences</param>
  void setPersistUserPreferences(bool enabled);

  /// <summary>
  /// Gets whether to persist user preferences
  /// </summary>
  const QVariant persistUserPreferences() const;

  /// <summary>
  /// Sets the remote debugging port
  /// </summary>
  /// <param name="port">The port to use</param>
  /// <remarks>
  /// CEF supports the remote debugging with Dev Tools in Chrome/Edge.
  /// if this value is set then you can debug the web application by
  /// accessing http://127.0.0.1:port from Chrome/Edge
  /// </remarks>
  void setRemoteDebuggingPort(short port);

  /// <summary>
  /// Gets the remote debugging port
  /// </summary>
  const QVariant remoteDebuggingPort() const;
};

#endif
