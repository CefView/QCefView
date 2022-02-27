#pragma once
#pragma region qt_headers
#include <QString>
#include <QWindow>
#include <QVariant>
#pragma endregion qt_headers

#define CEF_JS_INVOKE_METHOD "_dsb.returnValue"

typedef std::function<void(const QString &result)> JsCallbackFn;

class QCefJsCallback
{
public:
  static std::string generateInjectJs(int callbackId, const QString js);
  QCefJsCallback();
  QCefJsCallback(const QVariantMap &map);

public:
  int id;
  bool completed;
  QString data;
};
