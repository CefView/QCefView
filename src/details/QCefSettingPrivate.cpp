#include "QCefSettingPrivate.h"

#pragma region qt_headers
#include <QColor>
#pragma endregion 

#pragma region cef_headers
#include <include/cef_version.h>
#pragma endregion 

#include <QCefContext.h>

QCefSettingPrivate::QCefSettingPrivate()
{
  auto cefConfig = QCefContext::instance()->cefConfig();
  backgroundColor_ = cefConfig->backgroundColor();
}

void
QCefSettingPrivate::CopyFromCefBrowserSettings(QCefSetting* qs, const CefBrowserSettings* cs)
{
  if (!qs || !cs) {
    return;
  }

  qs->d_func()->copyFromCefBrowserSettings(cs);
}


void
QCefSettingPrivate::CopyFromCefBrowserSettings(QCefSettingPrivate* qsp, const CefBrowserSettings* cs)
{
  if (!qsp || !cs) {
    return;
  }

  qsp->copyFromCefBrowserSettings(cs);
}

void
QCefSettingPrivate::CopyToCefBrowserSettings(const QCefSetting* qs, CefBrowserSettings* cs)
{
  if (!cs) {
    return;
  }

  if (qs) {
    qs->d_func()->copyToCefBrowserSettings(cs);
  } else {
    QCefSettingPrivate defaultSettings;
    cs->background_color = defaultSettings.backgroundColor_.value<QColor>().rgba();
  }
}


void
QCefSettingPrivate::CopyToCefBrowserSettings(const QCefSettingPrivate* qsp, CefBrowserSettings* cs)
{
  if (!cs) {
    return;
  }

  if (qsp) {
    qsp->copyToCefBrowserSettings(cs);
  } else {
    QCefSettingPrivate defaultSettings;
    cs->background_color = defaultSettings.backgroundColor_.value<QColor>().rgba();
  }
}


void
QCefSettingPrivate::copyFromCefBrowserSettings(const CefBrowserSettings* cs)
{
  if (!cs) {
    return;
  }

  this->standardFontFamily_ = CefString(&cs->standard_font_family).ToString();

  this->fixedFontFamily_ = CefString(&cs->fixed_font_family).ToString();

  this->serifFontFamily_ = CefString(&cs->serif_font_family).ToString();

  this->sansSerifFontFamily_ = CefString(&cs->sans_serif_font_family).ToString();

  this->cursiveFontFamily_ = CefString(&cs->cursive_font_family).ToString();

  this->fantasyFontFamily_ = CefString(&cs->fantasy_font_family).ToString();

  this->defaultEncoding_ = CefString(&cs->default_encoding).ToString();

#if CEF_VERSION_MAJOR < 118
  this->acceptLanguageList_ = CefString(&cs->accept_language_list).ToString();
#endif

  this->windowlessFrameRate_ = cs->windowless_frame_rate;

  this->defaultFontSize_ = cs->default_font_size;

  this->defaultFixedFontSize_ = cs->default_fixed_font_size;

  this->minimumFontSize_ = cs->minimum_font_size;

  this->minimumLogicalFontSize_ = cs->minimum_logical_font_size;

  if (cs->remote_fonts != STATE_DEFAULT)
    this->remoteFonts_ = cs->remote_fonts == STATE_ENABLED;

  if (cs->javascript != STATE_DEFAULT)
    this->javascript_ = cs->javascript == STATE_ENABLED;

  if (cs->javascript_close_windows != STATE_DEFAULT)
    this->javascriptCloseWindows_ = cs->javascript_close_windows == STATE_ENABLED;

  if (cs->javascript_access_clipboard != STATE_DEFAULT)
    this->javascriptAccessClipboard_ = cs->javascript_access_clipboard == STATE_ENABLED;

  if (cs->javascript_dom_paste != STATE_DEFAULT)
    this->javascriptDomPaste_ = cs->javascript_dom_paste == STATE_ENABLED;

#if CEF_VERSION_MAJOR < 100
  if (cs->plugins != STATE_DEFAULT)
    this->plugins_ = cs->plugins == STATE_ENABLED;
#endif

  if (cs->image_loading != STATE_DEFAULT)
    this->imageLoading_ = cs->image_loading == STATE_ENABLED;

  if (cs->image_shrink_standalone_to_fit != STATE_DEFAULT)
    this->imageShrinkStandaloneToFit_ = cs->image_shrink_standalone_to_fit == STATE_ENABLED;

  if (cs->text_area_resize != STATE_DEFAULT)
    this->textAreaResize_ = cs->text_area_resize == STATE_ENABLED;

  if (cs->tab_to_links != STATE_DEFAULT)
    this->tabToLinks_ = cs->tab_to_links == STATE_ENABLED;

  if (cs->local_storage != STATE_DEFAULT)
    this->localStorage_ = cs->local_storage == STATE_ENABLED;

  if (cs->databases != STATE_DEFAULT)
    this->databases_ = cs->databases == STATE_ENABLED;

  if (cs->webgl != STATE_DEFAULT)
    this->webgl_ = cs->webgl == STATE_ENABLED;

  this->backgroundColor_ = QColor::fromRgba(cs->background_color);
}

void
QCefSettingPrivate::copyToCefBrowserSettings(CefBrowserSettings* cs) const
{
  if (!cs) {
    return;
  }

  if (!this->standardFontFamily_.empty())
    CefString(&cs->standard_font_family) = this->standardFontFamily_;

  if (!this->fixedFontFamily_.empty())
    CefString(&cs->fixed_font_family) = this->fixedFontFamily_;

  if (!this->serifFontFamily_.empty())
    CefString(&cs->serif_font_family) = this->serifFontFamily_;

  if (!this->sansSerifFontFamily_.empty())
    CefString(&cs->sans_serif_font_family) = this->sansSerifFontFamily_;

  if (!this->cursiveFontFamily_.empty())
    CefString(&cs->cursive_font_family) = this->cursiveFontFamily_;

  if (!this->fantasyFontFamily_.empty())
    CefString(&cs->fantasy_font_family) = this->fantasyFontFamily_;

  if (!this->defaultEncoding_.empty())
    CefString(&cs->default_encoding) = this->defaultEncoding_;

#if CEF_VERSION_MAJOR < 118
  if (!this->acceptLanguageList_.empty())
    CefString(&cs->accept_language_list) = this->acceptLanguageList_;
#endif

  if (this->windowlessFrameRate_.canConvert<int>())
    cs->windowless_frame_rate = this->windowlessFrameRate_.toInt();

  if (this->defaultFontSize_.canConvert<int>())
    cs->default_font_size = this->defaultFontSize_.toInt();

  if (this->defaultFixedFontSize_.canConvert<int>())
    cs->default_fixed_font_size = this->defaultFixedFontSize_.toInt();

  if (this->minimumFontSize_.canConvert<int>())
    cs->minimum_font_size = this->minimumFontSize_.toInt();

  if (this->minimumLogicalFontSize_.canConvert<int>())
    cs->minimum_logical_font_size = this->minimumLogicalFontSize_.toInt();

  if (this->remoteFonts_.canConvert<bool>())
    cs->remote_fonts = this->remoteFonts_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->javascript_.canConvert<bool>())
    cs->javascript = this->javascript_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->javascriptCloseWindows_.canConvert<bool>())
    cs->javascript_close_windows = this->javascriptCloseWindows_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->javascriptAccessClipboard_.canConvert<bool>())
    cs->javascript_access_clipboard = this->javascriptAccessClipboard_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->javascriptDomPaste_.canConvert<bool>())
    cs->javascript_dom_paste = this->javascriptDomPaste_.toBool() ? STATE_ENABLED : STATE_DISABLED;

#if CEF_VERSION_MAJOR < 100
  if (this->plugins_.canConvert<bool>())
    cs->plugins = this->plugins_.toBool() ? STATE_ENABLED : STATE_DISABLED;
#endif

  if (this->imageLoading_.canConvert<bool>())
    cs->image_loading = this->imageLoading_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->imageShrinkStandaloneToFit_.canConvert<bool>())
    cs->image_shrink_standalone_to_fit = this->imageShrinkStandaloneToFit_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->textAreaResize_.canConvert<bool>())
    cs->text_area_resize = this->textAreaResize_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->tabToLinks_.canConvert<bool>())
    cs->tab_to_links = this->tabToLinks_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->localStorage_.canConvert<bool>())
    cs->local_storage = this->localStorage_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->databases_.canConvert<bool>())
    cs->databases = this->databases_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->webgl_.canConvert<bool>())
    cs->webgl = this->webgl_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (this->backgroundColor_.canConvert<QColor>())
    cs->background_color = this->backgroundColor_.value<QColor>().rgba();
}
