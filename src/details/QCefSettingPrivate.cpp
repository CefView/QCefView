#include "QCefSettingPrivate.h"

#pragma region qt_headers
#include <QColor>
#include <QCoreApplication>
#include <QDir>
#include <QString>
#pragma endregion qt_headers

#include <CefViewCoreProtocol.h>

QCefSettingPrivate::QCefSettingPrivate() {}

void
QCefSettingPrivate::CopyToCefBrowserSettings(CefBrowserSettings& settings) const
{
  if (!standardFontFamily_.empty())
    CefString(&settings.standard_font_family) = standardFontFamily_;

  if (!fixedFontFamily_.empty())
    CefString(&settings.fixed_font_family) = fixedFontFamily_;

  if (!serifFontFamily_.empty())
    CefString(&settings.serif_font_family) = serifFontFamily_;

  if (!sansSerifFontFamily_.empty())
    CefString(&settings.sans_serif_font_family) = sansSerifFontFamily_;

  if (!cursiveFontFamily_.empty())
    CefString(&settings.cursive_font_family) = cursiveFontFamily_;

  if (!fantasyFontFamily_.empty())
    CefString(&settings.fantasy_font_family) = fantasyFontFamily_;

  if (!defaultEncoding_.empty())
    CefString(&settings.default_encoding) = defaultEncoding_;

  if (!acceptLanguageList_.empty())
    CefString(&settings.accept_language_list) = acceptLanguageList_;

  if (windowlessFrameRate_.canConvert<int>())
    settings.windowless_frame_rate = windowlessFrameRate_.toInt();

  if (defaultFontSize_.canConvert<int>())
    settings.default_font_size = defaultFontSize_.toInt();

  if (defaultFixedFontSize_.canConvert<int>())
    settings.default_fixed_font_size = defaultFixedFontSize_.toInt();

  if (minimumFontSize_.canConvert<int>())
    settings.minimum_font_size = minimumFontSize_.toInt();

  if (minimumLogicalFontSize_.canConvert<int>())
    settings.minimum_logical_font_size = minimumLogicalFontSize_.toInt();

  if (remoteFonts_.canConvert<bool>())
    settings.remote_fonts = remoteFonts_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (javascript_.canConvert<bool>())
    settings.javascript = javascript_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (javascriptCloseWindows_.canConvert<bool>())
    settings.javascript_close_windows = javascriptCloseWindows_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (javascriptAccessClipboard_.canConvert<bool>())
    settings.javascript_access_clipboard = javascriptAccessClipboard_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (javascriptDomPaste_.canConvert<bool>())
    settings.javascript_dom_paste = javascriptDomPaste_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (plugins_.canConvert<bool>())
    settings.plugins = plugins_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (universalAccessFromFileUrls_.canConvert<bool>())
    settings.universal_access_from_file_urls = universalAccessFromFileUrls_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (fileAccessFromFileUrls_.canConvert<bool>())
    settings.file_access_from_file_urls = fileAccessFromFileUrls_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (webSecurity_.canConvert<bool>())
    settings.web_security = webSecurity_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (imageLoading_.canConvert<bool>())
    settings.image_loading = imageLoading_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (imageShrinkStandaloneToFit_.canConvert<bool>())
    settings.image_shrink_standalone_to_fit = imageShrinkStandaloneToFit_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (textAreaResize_.canConvert<bool>())
    settings.text_area_resize = textAreaResize_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (tabToLinks_.canConvert<bool>())
    settings.tab_to_links = tabToLinks_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (localStorage_.canConvert<bool>())
    settings.local_storage = localStorage_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (databases_.canConvert<bool>())
    settings.databases = databases_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (applicationCache_.canConvert<bool>())
    settings.application_cache = applicationCache_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (webgl_.canConvert<bool>())
    settings.webgl = webgl_.toBool() ? STATE_ENABLED : STATE_DISABLED;

  if (backgroundColor_.canConvert<QColor>())
    settings.background_color = this->backgroundColor_.value<QColor>().value();
}
