#pragma once

#pragma region qt_headers
#include <QEvent>
#include <QKeyEvent>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

/// <summary>
/// Maps a Qt QKeyEvent to a CEF CefKeyEvent structure.
/// This function translates the Qt key event properties to their CEF equivalents,
/// enabling proper keyboard event handling in CEF-based components.
/// </summary>
/// <param name="qe">Pointer to the QKeyEvent to map from.</param>
/// <param name="ce">Reference to the CefKeyEvent to populate.</param>
void
MapQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce);

/// <summary>
/// Determines whether a QKeyEvent should result in sending a CEF key character event.
/// Typically used to filter out non-character or system key events.
/// </summary>
/// <param name="qe">Pointer to the QKeyEvent to evaluate.</param>
/// <returns>True if a key character event should be sent; otherwise, false.</returns>
bool
ShouldSendKeyCharEvent(QKeyEvent* qe);

/// <summary>
/// Adjusts a CEF CefKeyEvent to ensure it correctly represents a key character event
/// based on the provided QKeyEvent. This may modify event type, character code, or modifiers.
/// </summary>
/// <param name="qe">Pointer to the QKeyEvent providing the source data.</param>
/// <param name="ce">Reference to the CefKeyEvent to adjust.</param>
void
AdjustCefKeyCharEvent(QKeyEvent* qe, CefKeyEvent& ce);
