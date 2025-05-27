#pragma once

#pragma region qt_headers
#include <QVariant>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion

#include <CefViewCoreGlobal.h>

#include <QCefView.h>

/// <summary>
/// Provides static utility functions for converting between Qt and CEF value types,
/// including QVariant, CefValue, and frame ID types.
/// </summary>
class ValueConvertor
{
public:
  /// <summary>
  /// Converts a CEF CefValue to a Qt QVariant.
  /// </summary>
  /// <param name="qVariant">Pointer to the QVariant that will receive the converted value.</param>
  /// <param name="cValue">Pointer to the CefValue to convert from.</param>
  static void CefValueToQVariant(QVariant* qVariant, CefValue* cValue);

  /// <summary>
  /// Converts a Qt QVariant to a CEF CefValue.
  /// </summary>
  /// <param name="cValue">Pointer to the CefValue that will receive the converted value.</param>
  /// <param name="qVariant">Pointer to the QVariant to convert from.</param>
  static void QVariantToCefValue(CefValue* cValue, const QVariant* qVariant);

  /// <summary>
  /// Converts a CEF frame ID (CefFrameId) to a Qt frame ID (QCefFrameId).
  /// </summary>
  /// <param name="id">The CefFrameId to convert.</param>
  /// <returns>The corresponding QCefFrameId.</returns>
  static QCefFrameId FrameIdC2Q(const CefFrameId& id);

  /// <summary>
  /// Converts a Qt frame ID (QCefFrameId) to a CEF frame ID (CefFrameId).
  /// </summary>
  /// <param name="id">The QCefFrameId to convert.</param>
  /// <returns>The corresponding CefFrameId.</returns>
  static CefFrameId FrameIdQ2C(const QCefFrameId& id);
};
