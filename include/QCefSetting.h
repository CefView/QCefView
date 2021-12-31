#ifndef QCEFSETTING_H
#define QCEFSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QObject>
#include <QString>
#include <QColor>
#include <QScopedPointer>
#pragma endregion qt_headers

class CCefSetting;
// class CCefManager;

class QCEFVIEW_EXPORT QCefSetting : public QObject
{
  Q_OBJECT

public:
  QCefSetting();

  ~QCefSetting();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setBrowserSubProcessPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString browserSubProcessPath();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setResourceDirectoryPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString resourceDirectoryPath();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setLocalesDirectoryPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString localesDirectoryPath();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setLocale(const QString& locale);

  /// <summary>
  ///
  /// </summary>
  const QString locale();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setUserAgent(const QString& agent);

  /// <summary>
  ///
  /// </summary>
  const QString userAgent();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setCachePath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString cachePath();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setUserDataPath(const QString& path);

  /// <summary>
  ///
  /// </summary>
  const QString userDataPath();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setBridgeObjectName(const QString& name);

  /// <summary>
  ///
  /// </summary>
  const QString bridgeObjectName();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setBackgroundColor(const QColor& color);

  /// <summary>
  ///
  /// </summary>
  const QColor backgroundColor();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setAcceptLanguageList(const QString& languages);

  /// <summary>
  ///
  /// </summary>
  const QString acceptLanguageList();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setPersistSessionCookies(bool enabled);

  /// <summary>
  ///
  /// </summary>
  const bool persistSessionCookies();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setPersistUserPreferences(bool enabled);

  /// <summary>
  ///
  /// </summary>
  const bool persistUserPreferences();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& setRemoteDebuggingPort(short port);

  /// <summary>
  ///
  /// </summary>
  const short remoteDebuggingPort();

  /// <summary>
  ///
  /// </summary>
  QCefSetting& addGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url);

private:
  QScopedPointer<CCefSetting> d;

  friend class CCefManager;
}; // namespace QCefSetting

#endif
