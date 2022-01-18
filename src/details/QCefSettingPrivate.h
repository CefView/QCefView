#pragma once

#pragma region stl_headers
#include <list>
#include <optional>
#include <string>
#pragma endregion stl_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

#include <QCefSetting.h>

class QCefSettingPrivate
{
public:
  explicit QCefSettingPrivate();

  void CopyToCefBrowserSettings(CefBrowserSettings& settings) const;

  int windowless_frame_rate;
  int default_font_size;
  int default_fixed_font_size;
  int minimum_font_size;
  int minimum_logical_font_size;

  std::string standard_font_family;
  std::string fixed_font_family;
  std::string serif_font_family;
  std::string sans_serif_font_family;
  std::string cursive_font_family;
  std::string fantasy_font_family;
  std::string default_encoding;
  std::string accept_language_list;

  std::optional<bool> remote_fonts;
  std::optional<bool> javascript;
  std::optional<bool> javascript_close_windows;
  std::optional<bool> javascript_access_clipboard;
  std::optional<bool> javascript_dom_paste;
  std::optional<bool> plugins;
  std::optional<bool> universal_access_from_file_urls;
  std::optional<bool> file_access_from_file_urls;
  std::optional<bool> web_security;
  std::optional<bool> image_loading;
  std::optional<bool> image_shrink_standalone_to_fit;
  std::optional<bool> text_area_resize;
  std::optional<bool> tab_to_links;
  std::optional<bool> local_storage;
  std::optional<bool> databases;
  std::optional<bool> application_cache;
  std::optional<bool> webgl;

  uint32_t background_color;
};
