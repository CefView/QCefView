#ifndef QCEFSETTINGS_H
#define QCEFSETTINGS_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QString>
#include <QMetaType>
#pragma endregion qt_headers

namespace QCefSetting {
/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setBrowserSubProcessPath(const QString& path);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
browserSubProcessPath();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setResourceDirectoryPath(const QString& path);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
resourceDirectoryPath();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setLocalesDirectoryPath(const QString& path);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
localesDirectoryPath();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setUserAgent(const QString& agent);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
userAgent();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setCachePath(const QString& path);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
cachePath();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setUserDataPath(const QString& path);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
userDataPath();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setBridgeObjectName(const QString& name);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
bridgeObjectName();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setPersistSessionCookies(bool enabled);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT bool
persistSessionCookies();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setPersistUserPreferences(bool enabled);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT bool
persistUserPreferences();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setLocale(const QString& locale);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
locale();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setRemoteDebuggingPort(int port);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT int
remoteDebuggingPort();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setBackgroundColor(const QColor& color);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QColor
backgroundColor();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setAcceptLanguageList(const QString& languages);

/// <summary>
///
/// </summary>
const QCEFVIEW_EXPORT QString
acceptLanguageList();

/// <summary>
///
/// </summary>
void QCEFVIEW_EXPORT
setGlobalCookie(const QString& name, const QString& value, const QString& domain, const QString& url);

}; // namespace QCefSetting

#endif
