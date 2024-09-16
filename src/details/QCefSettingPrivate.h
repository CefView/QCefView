#pragma once

#pragma region stl_headers
#include <list>
#include <string>
#pragma endregion 

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion 

#pragma region qt_headers
#include <QVariant>
#pragma endregion 

#include <QCefSetting.h>

class QCefSettingPrivate
{
  Q_DECLARE_PUBLIC(QCefSetting)
  QCefSetting* q_ptr{};

  friend class CCefClientDelegate;

public:
  static void CopyFromCefBrowserSettings(QCefSetting* qs, const CefBrowserSettings* cs);

  static void CopyFromCefBrowserSettings(QCefSettingPrivate* qsp, const CefBrowserSettings* cs);

  static void CopyToCefBrowserSettings(const QCefSetting* qs, CefBrowserSettings* cs);

  static void CopyToCefBrowserSettings(const QCefSettingPrivate* qs, CefBrowserSettings* cs);

public:
  explicit QCefSettingPrivate();

  //////////////////////////////////////////////////////////////////////////
  // Non CEF browser setting values
  QSize windowInitialSize_;
  bool hardwareAcceleration_ = false;

  //////////////////////////////////////////////////////////////////////////
  // CEF browser setting values
  std::string standardFontFamily_;
  std::string fixedFontFamily_;
  std::string serifFontFamily_;
  std::string sansSerifFontFamily_;
  std::string cursiveFontFamily_;
  std::string fantasyFontFamily_;
  std::string defaultEncoding_;
#if CEF_VERSION_MAJOR < 118
  std::string acceptLanguageList_;
#endif

  /* int */ QVariant windowlessFrameRate_;
  /* int */ QVariant defaultFontSize_;
  /* int */ QVariant defaultFixedFontSize_;
  /* int */ QVariant minimumFontSize_;
  /* int */ QVariant minimumLogicalFontSize_;

  /* bool */ QVariant remoteFonts_;
  /* bool */ QVariant javascript_;
  /* bool */ QVariant javascriptCloseWindows_;
  /* bool */ QVariant javascriptAccessClipboard_;
  /* bool */ QVariant javascriptDomPaste_;
#if CEF_VERSION_MAJOR < 100
  /* bool */ QVariant plugins_;
#endif
  /* bool */ QVariant imageLoading_;
  /* bool */ QVariant imageShrinkStandaloneToFit_;
  /* bool */ QVariant textAreaResize_;
  /* bool */ QVariant tabToLinks_;
  /* bool */ QVariant localStorage_;
  /* bool */ QVariant databases_;
  /* bool */ QVariant webgl_;

  /* QColor */ QVariant backgroundColor_;

private:
  /// <summary>
  ///
  /// </summary>
  /// <param name="cs"></param>
  void copyFromCefBrowserSettings(const CefBrowserSettings* cs);

  /// <summary>
  ///
  /// </summary>
  /// <param name="cs"></param>
  void copyToCefBrowserSettings(CefBrowserSettings* cs) const;
};
