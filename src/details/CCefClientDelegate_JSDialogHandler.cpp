#include "CCefClientDelegate.h"

#include <QDebug>
#include <QSharedPointer>
#include <QThread>

#include "QCefViewPrivate.h"
#include "utils/CommonUtils.h"
#include "utils/ValueConvertor.h"

bool
CCefClientDelegate::OnJSDialog(CefRefPtr<CefBrowser> browser,
                               const CefString& origin_url,
                               CefJSDialogHandler::JSDialogType dialog_type,
                               const CefString& message_text,
                               const CefString& default_prompt_text,
                               CefRefPtr<CefJSDialogCallback> callback,
                               bool& suppress_message)

{
  bool ret = false;

  QString originUrl = QString::fromStdString(origin_url.ToString());
  QString messageText = QString::fromStdString(message_text.ToString());
  QString defaultPromptText = QString::fromStdString(default_prompt_text.ToString());

  Qt::ConnectionType c =
    pCefViewPrivate_->q_ptr->thread() == QThread::currentThread() ? Qt::DirectConnection : Qt::BlockingQueuedConnection;
  QMetaObject::invokeMethod(
    pCefViewPrivate_,
    [&]() {
      ret = pCefViewPrivate_->onJsDialog(
        originUrl, dialog_type, messageText, defaultPromptText, callback, suppress_message);
    },
    c);
  return ret;
}
