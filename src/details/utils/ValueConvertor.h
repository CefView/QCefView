#pragma once

#pragma region qt_headers
#include <QVariant>
#pragma endregion 

#pragma region cef_headers
#include <include/cef_app.h>
#pragma endregion 

#include <CefViewCoreGlobal.h>

#include <QCefView.h>

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

  /// <summary>
  ///
  /// </summary>
  /// <param name="id"></param>
  /// <returns></returns>
  static QCefFrameId FrameIdC2Q(const CefFrameId& id);

  /// <summary>
  ///
  /// </summary>
  /// <param name="id"></param>
  /// <returns></returns>
  static CefFrameId FrameIdQ2C(const QCefFrameId& id);
};
