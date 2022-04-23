#pragma once

#pragma region qt_headers
#include <QVariant>
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion cef_headers

class ValueConvertor
{
public:
  /// <summary>
  ///
  /// </summary>
  /// <param name="qVariant"></param>
  /// <param name="cValue"></param>
  static void CefValueToQVariant(QVariant* qVariant, CefValue* cValue);

  /// <summary>
  ///
  /// </summary>
  /// <param name="cValue"></param>
  /// <param name="qVariant"></param>
  static void QVariantToCefValue(CefValue* cValue, const QVariant* qVariant);
};
