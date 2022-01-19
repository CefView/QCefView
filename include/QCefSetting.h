#ifndef QCEFSETTING_H
#define QCEFSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QColor>
#include <QScopedPointer>
#include <QString>
#pragma endregion qt_headers

class QCefSettingPrivate;

/// <summary>
/// Represents the settings for individual browser
/// </summary>
class QCEFVIEW_EXPORT QCefSetting
{
  Q_DECLARE_PRIVATE(QCefSetting)
  QScopedPointer<QCefSettingPrivate> d_ptr;

  friend class QCefView;

public:
  /// <summary>
  ///
  /// </summary>
  QCefSetting();

  /// <summary>
  ///
  /// </summary>
  /// <param name="other"></param>
  QCefSetting(const QCefSetting& other);

  /// <summary>
  ///
  /// </summary>
  /// <param name="other"></param>
  QCefSetting& operator=(const QCefSetting& other);

  /// <summary>
  ///
  /// </summary>
  ~QCefSetting();

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setStandardFontFamily(const QString value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString standardFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setFixedFontFamily(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString fixedFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setSerifFontFamily(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString serifFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setSansSerifFontFamily(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString sansSerifFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setCursiveFontFamily(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString cursiveFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setFantasyFontFamily(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString fantasyFontFamily() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setDefaultEncoding(const QString& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QString defaultEncoding() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setAcceptLanguageList(const QString& value);
  const QString acceptLanguageList() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setWindowlessFrameRate(const int value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant windowlessFrameRate() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setDefaultFontSize(const int value);
  const QVariant defaultFontSize() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setDefaultFixedFontSize(const int value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant defaultFixedFontSize() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setMinimumFontSize(const int value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant minimumFontSize() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setMinimumLogicalFontSize(const int value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant minimumLogicalFontSize() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setRemoteFonts(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant remoteFonts() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setJavascript(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant javascript() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setJavascriptCloseWindows(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant javascriptCloseWindows() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setJavascriptAccessClipboard(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant javascriptAccessClipboard() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setJavascriptDomPaste(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant javascriptDomPaste() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setPlugins(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant plugins() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setUniversalAccessFromFileUrls(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant universalAccessFromFileUrls() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setFileAccessFromFileUrls(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant fileAccessFromFileUrls() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setWebSecurity(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant webSecurity() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setImageLoading(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant imageLoading() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setImageShrinkStandaloneToFit(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant imageShrinkStandaloneToFit() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setTextAreaResize(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant textAreaResize() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setTabToLinks(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant tabToLinks() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setLocalStorage(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant localStorage() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setDatabases(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant databases() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setApplicationCache(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant applicationCache() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setWebGL(const bool value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant webGL() const;

  /// <summary>
  ///
  /// </summary>
  /// <param name="value"></param>
  void setBackgroundColor(const QColor& value);

  /// <summary>
  ///
  /// </summary>
  /// <returns></returns>
  const QVariant backgroundColor() const;
};

#endif
