#pragma once

#pragma region qt_headers
#include <QWidget>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

/// <summary>
/// Maps a CEF cursor type to a Qt cursor shape.
/// </summary>
/// <param name="type">The CEF cursor type to map.</param>
/// <returns>The corresponding Qt::CursorShape.</returns>
Qt::CursorShape
MapCursorShape(cef_cursor_type_t& type);
