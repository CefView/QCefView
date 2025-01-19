#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QSharedPointer>
#include <QThread>

#include "details/QCefViewPrivate.h"
#include "details/utils/CommonUtils.h"
#include "details/utils/ValueConvertor.h"

bool
CCefClientDelegate::onJSDialog(CefRefPtr<CefBrowser>& browser,
                               const CefString& origin_url,
                               CefJSDialogHandler::JSDialogType dialog_type,
                               const CefString& message_text,
                               const CefString& default_prompt_text,
                               CefRefPtr<CefJSDialogCallback>& callback,
                               bool& suppress_message)
{
  /// Called to run a JavaScript dialog.
  ///
  /// If |origin_url| is non-empty it can be passed to the CefFormatUrlForSecurityDisplay
  /// function to retrieve a secureand user-friendly display string.
  ///
  /// The |default_prompt_text| value will be specified for prompt dialogs only.
  ///
  /// Set |suppress_message| to true and return false to suppress the message
  /// (suppressing messages is preferable to immediately executing the callback
  /// as this is used to detect presumably malicious behavior like spamming alert
  /// messages in onbeforeunload).
  ///
  /// Set |suppress_message| to false and return false to use the default
  /// implementation (the default implementation will show one modal dialog at a
  /// time and suppress any additional dialog requests until the displayed
  /// dialog is dismissed).
  ///
  /// Return true if the application will use a custom dialog or if the callback has
  /// been executed immediately. Custom dialogs may be either modal or modeless. If a
  /// custom dialog is used the application must execute |callback| once the custom
  /// dialog is dismissed.

  switch (dialog_type) {
    case CefJSDialogHandler::JSDialogType::JSDIALOGTYPE_ALERT: {
      QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
        QMessageBox msgBox(pCefViewPrivate_->q_func());
        auto title = CefFormatUrlForSecurityDisplay(origin_url);
        msgBox.setWindowTitle(title.ToString().c_str());
        msgBox.setText(message_text.ToString().c_str());
        msgBox.setStandardButtons(QMessageBox::StandardButton::Ok);

        // insert to the pending map
        pendingJSDialogMap_[&msgBox] = QPointer<QDialog>(&msgBox);
        msgBox.exec();
        // remove from pending map
        pendingJSDialogMap_.remove(&msgBox);

        callback->Continue(true, "");
      });
      return true;
    } break;
    case CefJSDialogHandler::JSDialogType::JSDIALOGTYPE_CONFIRM: {
      QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
        QMessageBox msgBox(pCefViewPrivate_->q_func());
        auto title = CefFormatUrlForSecurityDisplay(origin_url);
        msgBox.setWindowTitle(title.ToString().c_str());
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(message_text.ToString().c_str());
        msgBox.setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);

        // insert to the pending map
        pendingJSDialogMap_[&msgBox] = QPointer<QDialog>(&msgBox);
        auto ret = msgBox.exec();
        // remove from pending map
        pendingJSDialogMap_.remove(&msgBox);

        callback->Continue(QMessageBox::Ok == ret, "");
      });
      return true;
    } break;
    case CefJSDialogHandler::JSDialogType::JSDIALOGTYPE_PROMPT: {
      QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
        QInputDialog inputDialog(pCefViewPrivate_->q_func());

        auto title = CefFormatUrlForSecurityDisplay(origin_url);
        inputDialog.setWindowTitle(title.ToString().c_str());
        inputDialog.setLabelText(message_text.ToString().c_str());
        inputDialog.setTextValue(default_prompt_text.ToString().c_str());
        inputDialog.setTextEchoMode(QLineEdit::Normal);

        // insert to the pending map
        pendingJSDialogMap_[&inputDialog] = QPointer<QDialog>(&inputDialog);
        auto ret = inputDialog.exec();
        // remove from pending map
        pendingJSDialogMap_.remove(&inputDialog);

        CefString userInput(inputDialog.textValue().toStdString());
        callback->Continue(!!ret, (!!ret) ? userInput : "");
      });
      return true;
    } break;
    default:
      break;
  }

  return false;
}

bool
CCefClientDelegate::onBeforeUnloadDialog(CefRefPtr<CefBrowser>& browser,
                                         const CefString& message_text,
                                         bool is_reload,
                                         CefRefPtr<CefJSDialogCallback>& callback)
{
  /// Called to run a dialog asking the user if they want to leave a page.
  /// Return false to use the default dialog implementation. Return true if the
  /// application will use a custom dialog or if the callback has been executed
  /// immediately. Custom dialogs may be either modal or modeless. If a custom
  /// dialog is used the application must execute |callback| once the custom
  /// dialog is dismissed.

  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    QMessageBox msgBox(pCefViewPrivate_->q_func());
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(message_text.ToString().c_str());
    msgBox.setStandardButtons(QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel);

    // insert to the pending map
    pendingJSDialogMap_[&msgBox] = QPointer<QDialog>(&msgBox);
    auto ret = msgBox.exec();
    // remove from pending map
    pendingJSDialogMap_.remove(&msgBox);

    callback->Continue(QMessageBox::Ok == ret, "");
  });
  return true;
}

void
CCefClientDelegate::onResetDialogState(CefRefPtr<CefBrowser>& browser)
{
  /// Called to cancel any pending dialogs and reset any saved dialog state.
  /// Will be called due to events like page navigation irregardless of whether
  /// any dialogs are currently pending.

  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() {
    for (auto dlg : pendingJSDialogMap_) {
      if (dlg) {
        dlg->reject();
      }
    }
    pendingJSDialogMap_.clear();
  });
}

void
CCefClientDelegate::onDialogClosed(CefRefPtr<CefBrowser>& browser)
{
  /// Called when the dialog is closed.
}
