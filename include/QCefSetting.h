#ifndef QCEFSETTING_H
#define QCEFSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QString>
#include <QColor>
#include <QScopedPointer>
#pragma endregion qt_headers

class QCefSettingPrivate;

/// <summary>
///
/// </summary>
class QCEFVIEW_EXPORT QCefSetting
{
  Q_DECLARE_PRIVATE(QCefSetting)

  QScopedPointer<QCefSettingPrivate> d_ptr;

  friend class QCefContext;

public:
  /// <summary>
  ///
  /// </summary>
  QCefSetting(int argc, char* argv[]);

  /// <summary>
  ///
  /// </summary>
  ~QCefSetting();

#if !defined(OS_MACOS)
  /// <summary>
  ///
  /// </summary>
  void setBrowserSubProcessPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString browserSubProcessPath() const;

  /// <summary>
  ///
  /// </summary>
  void setResourceDirectoryPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString resourceDirectoryPath() const;

  /// <summary>
  ///
  /// </summary>
  void setLocalesDirectoryPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString localesDirectoryPath() const;
#endif

  /// <summary>
  ///
  /// </summary>
  void setLocale(const QString& locale);

  /// <summary>
  ///
  /// </summary>
  const QString locale() const;

  /// <summary>
  ///
  /// </summary>
  void setUserAgent(const QString& agent);

  /// <summary>
  ///
  /// </summary>
  const QString userAgent() const;

  /// <summary>
  ///
  /// </summary>
  void setCachePath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString cachePath() const;

  /// <summary>
  ///
  /// </summary>
  void setUserDataPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString userDataPath() const;

  /// <summary>
  ///
  /// </summary>
  void setBridgeObjectName(const QString& name);

  /// <summary>
  ///
  /// </summary>
  const QString bridgeObjectName() const;

  /// <summary>
  ///
  /// </summary>
  void setBackgroundColor(const QColor& color);

  /// <summary>
  ///
  /// </summary>
  const QColor backgroundColor() const;

  /// <summary>
  ///
  /// </summary>
  void setAcceptLanguageList(const QString& languages);

  /// <summary>
  ///
  /// </summary>
  const QString acceptLanguageList() const;

  /// <summary>
  ///
  /// </summary>
  void setPersistSessionCookies(bool enabled);

  /// <summary>
  ///
  /// </summary>
  const bool persistSessionCookies() const;

  /// <summary>
  ///
  /// </summary>
  void setPersistUserPreferences(bool enabled);

  /// <summary>
  ///
  /// </summary>
  const bool persistUserPreferences() const;

  /// <summary>
  ///
  /// </summary>
  void setRemoteDebuggingPort(short port);

  /// <summary>
  ///
  /// </summary>
  const short remoteDebuggingPort() const;

  /// <summary>
  ///
  /// </summary>
  void addGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url);
};

#endif
