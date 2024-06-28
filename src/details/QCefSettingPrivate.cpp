#include "QCefSettingPrivate.h"

#pragma region qt_headers
#include <QColor>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_version.h>
#pragma endregion cef_headers

#include <QCefContext.h>

QCefSettingPrivate::QCefSettingPrivate()
{
  auto cefConfig = QCefContext::instance()->cefConfig();
  backgroundColor_ = cefConfig->backgroundColor();
}

void
QCefSettingPrivate::CopyFromCefBrowserSettings(QCefSetting* qs, const CefBrowserSettings* cs)
{
  if (!qs || !cs)
    return;

  qs->d_ptr->standardFontFamily_ = CefString(&cs->standard_font_family).ToString();

  qs->d_ptr->fixedFontFamily_ = CefString(&cs->fixed_font_family).ToString();

  qs->d_ptr->serifFontFamily_ = CefString(&cs->serif_font_family).ToString();

  qs->d_ptr->sansSerifFontFamily_ = CefString(&cs->sans_serif_font_family).ToString();

  qs->d_ptr->cursiveFontFamily_ = CefString(&cs->cursive_font_family).ToString();

  qs->d_ptr->fantasyFontFamily_ = CefString(&cs->fantasy_font_family).ToString();

  qs->d_ptr->defaultEncoding_ = CefString(&cs->default_encoding).ToString();

#if CEF_VERSION_MAJOR < 118
  qs->d_ptr->acceptLanguageList_ = CefString(&cs->accept_language_list).ToString();
#endif

  qs->d_ptr->windowlessFrameRate_ = cs->windowless_frame_rate;

  qs->d_ptr->defaultFontSize_ = cs->default_font_size;

  qs->d_ptr->defaultFixedFontSize_ = cs->default_fixed_font_size;

  qs->d_ptr->minimumFontSize_ = cs->minimum_font_size;

  qs->d_ptr->minimumLogicalFontSize_ = cs->minimum_logical_font_size;

  if (cs->remote_fonts != STATE_DEFAULT)
    qs->d_ptr->remoteFonts_ = cs->remote_fonts == STATE_ENABLED;

  if (cs->javascript != STATE_DEFAULT)
    qs->d_ptr->javascript_ = cs->javascript == STATE_ENABLED;

  if (cs->javascript_close_windows != STATE_DEFAULT)
    qs->d_ptr->javascriptCloseWindows_ = cs->javascript_close_windows == STATE_ENABLED;

  if (cs->javascript_access_clipboard != STATE_DEFAULT)
    qs->d_ptr->javascriptAccessClipboard_ = cs->javascript_access_clipboard == STATE_ENABLED;

  if (cs->javascript_dom_paste != STATE_DEFAULT)
    qs->d_ptr->javascriptDomPaste_ = cs->javascript_dom_paste == STATE_ENABLED;

#if CEF_VERSION_MAJOR < 100
  if (cs->plugins != STATE_DEFAULT)
    qs->d_ptr->plugins_ = cs->plugins == STATE_ENABLED;
#endif

  if (cs->image_loading != STATE_DEFAULT)
    qs->d_ptr->imageLoading_ = cs->image_loading == STATE_ENABLED;

  if (cs->image_shrink_standalone_to_fit != STATE_DEFAULT)
    qs->d_ptr->imageShrinkStandaloneToFit_ = cs->image_shrink_standalone_to_fit == STATE_ENABLED;

  if (cs->text_area_resize != STATE_DEFAULT)
    qs->d_ptr->textAreaResize_ = cs->text_area_resize == STATE_ENABLED;

  if (cs->tab_to_links != STATE_DEFAULT)
    qs->d_ptr->tabToLinks_ = cs->tab_to_links == STATE_ENABLED;

  if (cs->local_storage != STATE_DEFAULT)
    qs->d_ptr->localStorage_ = cs->local_storage == STATE_ENABLED;

  if (cs->databases != STATE_DEFAULT)
    qs->d_ptr->databases_ = cs->databases == STATE_ENABLED;

  if (cs->webgl != STATE_DEFAULT)
    qs->d_ptr->webgl_ = cs->webgl == STATE_ENABLED;

  qs->d_ptr->backgroundColor_ = QColor::fromRgba(cs->background_color);
}

void
QCefSettingPrivate::CopyToCefBrowserSettings(const QCefSetting* qs, CefBrowserSettings* cs)
{
  if (!cs)
    return;

  if (!qs) {
    QCefSettingPrivate defaultSettings;
    cs->background_color = defaultSettings.backgroundColor_.value<QColor>().rgba();
    return;
  }

  if (!qs->d_ptr->standardFontFamily_.empty())
    CefString(&cs->standard_font_family) = qs->d_ptr->standardFontFamily_;

  if (!qs->d_ptr->fixedFontFamily_.empty())
    CefString(&cs->fixed_font_family) = qs->d_ptr->fixedFontFamily_;

  if (!qs->d_ptr->serifFontFamily_.empty())
    CefString(&cs->serif_font_family) = qs->d_ptr->serifFontFamily_;

  if (!qs->d_ptr->sansSerifFontFamily_.empty())
    CefString(&cs->sans_serif_font_family) = qs->d_ptr->sansSerifFontFamily_;

  if (!qs->d_ptr->cursiveFontFamily_.empty())
    CefString(&cs->cursive_font_family) = qs->d_ptr->cursiveFontFamily_;

  if (!qs->d_ptr->fantasyFontFamily_.empty())
    CefString(&cs->fantasy_font_family) = qs->d_ptr->fantasyFontFamily_;

  if (!qs->d_ptr->defaultEncoding_.empty())
    CefString(&cs->default_encoding) = qs->d_ptr->defaultEncoding_;

#if CEF_VERSION_MAJOR < 118
  if (!qs->d_ptr->acceptLanguageList_.empty())
    CefString(&cs->accept_language_list) = qs->d_ptr->acceptLanguageList_;
#endif

  if (qs->d_ptr->windowlessFrameRate_.canConvert<int>())
    cs->windowless_frame_rate = qs->d_ptr->windowlessFrameRate_.toInt();

  if (qs->d_ptr->defaultFontSize_.canConvert<int>())
    cs->default_font_size = qs->d_ptr->defaultFontSize_.toInt();

  if (qs->d_ptr->defaultFixedFontSize_.canConvert<int>())
    cs->default_fixed_font_size = qs->d_ptr->defaultFixedFontSize_.toInt();

  if (qs->d_ptr->minimumFontSize_.canConvert<int>())
    cs->minimum_font_size = qs->d_ptr->minimumFontSize_.toInt();

  if (qs->d_ptr->minimumLogicalFontSize_.canConvert<int>())
    cs->minimum_logical_font_size = qs->d_ptr->minimumLogicalFontSize_.toInt();

  if (qs->d_ptr->remoteFonts_.canConvert<bool>())
    cs->remote_fonts = qs->d_ptr->remoteFonts_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->javascript_.canConvert<bool>())
    cs->javascript = qs->d_ptr->javascript_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->javascriptCloseWindows_.canConvert<bool>())
    cs->javascript_close_windows = qs->d_ptr->javascriptCloseWindows_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->javascriptAccessClipboard_.canConvert<bool>())
    cs->javascript_access_clipboard = qs->d_ptr->javascriptAccessClipboard_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->javascriptDomPaste_.canConvert<bool>())
    cs->javascript_dom_paste = qs->d_ptr->javascriptDomPaste_.toBool() ? STATE_ENABLED : STATE_DISABLED;

#if CEF_VERSION_MAJOR < 100
  if (qs->d_ptr->plugins_.canConvert<bool>())
    cs->plugins = qs->d_ptr->plugins_.toBool() ? STATE_ENABLED : STATE_DISABLED;
#endif

  if (qs->d_ptr->imageLoading_.canConvert<bool>())
    cs->image_loading = qs->d_ptr->imageLoading_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->imageShrinkStandaloneToFit_.canConvert<bool>())
    cs->image_shrink_standalone_to_fit =
      qs->d_ptr->imageShrinkStandaloneToFit_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->textAreaResize_.canConvert<bool>())
    cs->text_area_resize = qs->d_ptr->textAreaResize_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->tabToLinks_.canConvert<bool>())
    cs->tab_to_links = qs->d_ptr->tabToLinks_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->localStorage_.canConvert<bool>())
    cs->local_storage = qs->d_ptr->localStorage_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->databases_.canConvert<bool>())
    cs->databases = qs->d_ptr->databases_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->webgl_.canConvert<bool>())
    cs->webgl = qs->d_ptr->webgl_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (qs->d_ptr->backgroundColor_.canConvert<QColor>())
    cs->background_color = qs->d_ptr->backgroundColor_.value<QColor>().rgba();
}
