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

#if defined(Q_OS_WINDOWS) || defined(Q_OS_LINUX)
  std::string localesDirectoryPath_;
  std::string browserSubProcessPath_;
  std::string resourceDirectoryPath_;
#endif

  int logLevel_ = LOGSEVERITY_DEFAULT;

  /// <summary>
  /// /* bool   */
  /// Enable OSR mode by default
  /// </summary>
  QVariant windowlessRenderingEnabled_ = true;

  /// <summary>
  /// /* bool   */
  /// Enable standalone message loop (bool)
  /// </summary>
#if defined(Q_OS_MACOS)
  QVariant standaloneMessgeLoopEnabled_ = false;
#else
  QVariant standaloneMessgeLoopEnabled_ = true;
#endif

  /// <summary>
  /// /* bool   */
  /// Disable sandbox (enabled by default) (bool)
  /// </summary>
#if defined(CEF_USE_SANDBOX)
  QVariant sandboxDisabled_ = false;
#else
  QVariant sandboxDisabled_ = true;
#endif

  /// <summary>
  /// /* bool   */
  /// Disable command line pass through (enabled by default) (bool)
  /// </summary>
  QVariant commandLinePassthroughDisabled_ = false;

  /// <summary>
  /// /* bool   */
  ///
  /// </summary>
  QVariant persistSessionCookies_;

#if CEF_VERSION_MAJOR < 128
  /// <summary>
  /// /* bool   */
  ///
  /// </summary>
  QVariant persistUserPreferences_;
#endif

  /// <summary>
  /// /* int   */
  ///
  /// </summary>
  QVariant remoteDebuggingport_;

  /// <summary>
  /// /* QColor   */
  ///
  /// </summary>
  QVariant backgroundColor_;

  /// <summary>
  ///
  /// </summary>
  ArgsMap commandLineArgs_;
};
