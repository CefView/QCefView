#include "QCefJsCallback.h"

std::string
QCefJsCallback::generateInjectJs(int callbackId, const QString js)
{
  std::string jscode = "var ret = {id:" + std::to_string(callbackId) + ", complete:true}; ret.data = (function() { " +
                       js.toStdString() + " })(); window._dsbridge.call('_dsb.returnValue', ret);";

  return jscode;
}
QCefJsCallback::QCefJsCallback() {
  id = -1;
  completed = false;
}

QCefJsCallback::QCefJsCallback(const QVariantMap &map) {
  id = map.value("id", -1).toInt();
  completed = map.value("completed", true).toBool();
  data = map.value("data", "").toString();
}
