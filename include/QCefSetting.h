/*
 * File: QCefSetting.h
 * Project: QCefView
 * Created: 29th March 2016
 * Author: Sheen
 * Source: https://github.com/cefview/qcefview
 * Docs: https://cefview.github.io/QCefView/
 */
#ifndef QCEFSETTING_H
#define QCEFSETTING_H
#pragma once
#include <QCefView_global.h>

#pragma region qt_headers
#include <QColor>
#include <QScopedPointer>
#include <QSize>
#include <QString>
#pragma endregion 

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
  /// Constructs the QCefSetting instance
  /// </summary>
  QCefSetting();

  /// <summary>
  /// Constructs the QCefSetting instance from existing one
  /// </summary>
  /// <param name="other">The existing QCefSetting instance</param>
  QCefSetting(const QCefSetting& other);

  /// <summary>
  /// Assigns the existing QCefSetting instance to current
  /// </summary>
  /// <param name="other"></param>
  QCefSetting& operator=(const QCefSetting& other);

  /// <summary>
  /// Destructs the instance
  /// </summary>
  ~QCefSetting();

  /// <summary>
  /// Sets the initial size of the browser
  /// </summary>
  /// <param name="size">The initial size</param>
  void setWindowInitialSize(const QSize& size);

  /// <summary>
  /// Gets the initial size of the browser
  /// </summary>
  /// <returns></returns>
  const QSize windowInitialSize() const;

  /// <summary>
  /// Sets the standard font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setStandardFontFamily(const QString& value);

  /// <summary>
  /// Gets the standard font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString standardFontFamily() const;

  /// <summary>
  /// Sets the fixed font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setFixedFontFamily(const QString& value);

  /// <summary>
  /// Gets the fixed font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString fixedFontFamily() const;

  /// <summary>
  /// Sets the serif font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setSerifFontFamily(const QString& value);

  /// <summary>
  /// Gets the serif font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString serifFontFamily() const;

  /// <summary>
  /// Sets the sans serif font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setSansSerifFontFamily(const QString& value);

  /// <summary>
  /// Gets the sans serif font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString sansSerifFontFamily() const;

  /// <summary>
  /// Sets the cursive font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setCursiveFontFamily(const QString& value);

  /// <summary>
  /// Gets the cursive font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString cursiveFontFamily() const;

  /// <summary>
  /// Sets the fantasy font family
  /// </summary>
  /// <param name="value">The font family</param>
  void setFantasyFontFamily(const QString& value);

  /// <summary>
  /// Gets the fantasy font family
  /// </summary>
  /// <returns>The font family</returns>
  const QString fantasyFontFamily() const;

  /// <summary>
  /// Sets the default encoding
  /// </summary>
  /// <param name="value">The encoding name</param>
  void setDefaultEncoding(const QString& value);

  /// <summary>
  /// Gets the default encoding
  /// </summary>
  /// <returns>The encoding name</returns>
  const QString defaultEncoding() const;

#if CEF_VERSION_MAJOR < 118
  /// <summary>
  /// Sets the acceptable language list
  /// </summary>
  /// <param name="value">The acceptable languate list</param>
  void setAcceptLanguageList(const QString& value);

  /// <summary>
  /// Gets the acceptable language list
  /// </summary>
  /// <returns>The acceptable languate list</returns>
  const QString acceptLanguageList() const;
#endif

  /// <summary>
  /// Sets the frame rate in window less mode
  /// </summary>
  /// <param name="value">The frame rate</param>
  void setWindowlessFrameRate(const int value);

  /// <summary>
  /// Gets the frame rate in window less mode
  /// </summary>
  /// <returns>The frame rate</returns>
  const QVariant windowlessFrameRate() const;

  /// <summary>
  /// Sets the default font size
  /// </summary>
  /// <param name="value">The font size</param>
  void setDefaultFontSize(const int value);

  /// <summary>
  /// Gets the default font size
  /// </summary>
  /// <returns>The font size</returns>
  const QVariant defaultFontSize() const;

  /// <summary>
  /// Sets the default fixed font size
  /// </summary>
  /// <param name="value">The font size</param>
  void setDefaultFixedFontSize(const int value);

  /// <summary>
  /// Gets the default fixed font size
  /// </summary>
  /// <returns>The font size</returns>
  const QVariant defaultFixedFontSize() const;

  /// <summary>
  /// Sets the minimum font size
  /// </summary>
  /// <param name="value">The font size</param>
  void setMinimumFontSize(const int value);

  /// <summary>
  /// Gets the minimum font size
  /// </summary>
  /// <returns>The font size</returns>
  const QVariant minimumFontSize() const;

  /// <summary>
  /// Sets the minimum logical font size
  /// </summary>
  /// <param name="value">The font size</param>
  void setMinimumLogicalFontSize(const int value);

  /// <summary>
  /// Gets the minimum logical font size
  /// </summary>
  /// <returns>The font size</returns>
  const QVariant minimumLogicalFontSize() const;

  /// <summary>
  /// Sets to enable or disable remote fonts
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setRemoteFonts(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the remote fonts
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant remoteFonts() const;

  /// <summary>
  /// Sets to enable or disable Javascript
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setJavascript(const bool value);

  /// <summary>
  /// Gets whether to enable or disable Javascript
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant javascript() const;

  /// <summary>
  /// Sets to enable or disable the permission of closing window from Javascript
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setJavascriptCloseWindows(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the permission of closing window from Javascript
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant javascriptCloseWindows() const;

  /// <summary>
  /// Sets to enable or disable the permission of accessing clipboard from Javascript
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setJavascriptAccessClipboard(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the permission of accessing clipboard from Javascript
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant javascriptAccessClipboard() const;

  /// <summary>
  /// Sets to enable or disable the permission of pasting DOM in Javascript
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setJavascriptDomPaste(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the permission of pasting DOM in Javascript
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant javascriptDomPaste() const;

#if CEF_VERSION_MAJOR < 100
  /// <summary>
  /// Sets to enable or disable plugins
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setPlugins(const bool value);

  /// <summary>
  /// Gets whether to enable or disable plugins
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant plugins() const;
#endif

  /// <summary>
  /// Sets to enable or disable the permission of loading images
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setImageLoading(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the permission of loading images
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant imageLoading() const;

  /// <summary>
  /// Sets to enable or disable the shrinking image standalone to fit
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setImageShrinkStandaloneToFit(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the shrinking image standalone to fit
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant imageShrinkStandaloneToFit() const;

  /// <summary>
  /// Sets to enable or disable the resizing of text area
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setTextAreaResize(const bool value);

  /// <summary>
  /// Gets whether to enable or disable the resizing of text area
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant textAreaResize() const;

  /// <summary>
  /// Sets to enable or disable tab to links
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setTabToLinks(const bool value);

  /// <summary>
  /// Gets whether to enable or disable  tab to links
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant tabToLinks() const;

  /// <summary>
  /// Sets to enable or disable local storage
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setLocalStorage(const bool value);

  /// <summary>
  /// Gets whether to enable or disable local storage
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant localStorage() const;

  /// <summary>
  /// Sets to enable or disable database
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setDatabases(const bool value);

  /// <summary>
  /// Gets whether to enable or disable database
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant databases() const;

  /// <summary>
  /// Sets to enable or disable webGL
  /// </summary>
  /// <param name="value">True to enable; false to disable</param>
  void setWebGL(const bool value);

  /// <summary>
  /// Gets whether to enable or disable webGL
  /// </summary>
  /// <returns>True to enable; false to disable</returns>
  const QVariant webGL() const;

  /// <summary>
  /// Sets the background color
  /// </summary>
  /// <param name="value">The color</param>
  /// <remarks>
  /// This only works if the web page has no background color set. The alpha component value
  /// will be adjusted to 0 or 255, it means if you pass a value with alpha value
  /// in the range of [1, 255], it will be accepted as 255. The default value is inherited from
  /// <see cref="QCefConfig::backgroundColor()"/>
  /// </remarks>
  void setBackgroundColor(const QColor& value);

  /// <summary>
  /// Gets the background color
  /// </summary>
  /// <returns>The color</returns>
  const QVariant backgroundColor() const;

  #if CEF_VERSION_MAJOR >= 125
  /// <summary>
  /// 
  /// </summary>
  /// <param name="value"></param>
  void setHardwareAcceleration(const bool value);

  /// <summary>
  /// 
  /// </summary>
  /// <returns></returns>
  const bool hardwareAcceleration() const;
  #endif
};

Q_DECLARE_METATYPE(QCefSetting);

#endif
