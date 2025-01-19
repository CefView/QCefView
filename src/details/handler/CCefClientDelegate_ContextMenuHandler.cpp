#include "details/CCefClientDelegate.h"

#include <QDebug>
#include <QThread>

#include "details/utils/CommonUtils.h"
#include "details/utils/MenuBuilder.h"

#include "details/QCefViewPrivate.h"

void
CCefClientDelegate::onBeforeContextMenu(CefRefPtr<CefBrowser>& browser,
                                        CefRefPtr<CefFrame>& frame,
                                        CefRefPtr<CefContextMenuParams>& params,
                                        CefRefPtr<CefMenuModel>& model)
{
  FLog();

  if (!pCefViewPrivate_)
    return;

  // popup browser doesn't involve off-screen rendering
  if (browser->IsPopup()) {
    return;
  }

  // main browser
  auto policy = pCefViewPrivate_->q_ptr->contextMenuPolicy();
  if (Qt::DefaultContextMenu != policy) {
    // for all non-default value, we clear the menu info to prevent the showing
    model->Clear();
    return;
  }

  if (pCefViewPrivate_->isOSRModeEnabled_) {
    // OSR mode
    auto menuData = MenuBuilder::CreateMenuDataFromCefMenu(model.get());
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onBeforeCefContextMenu(menuData); });
  }
}

bool
CCefClientDelegate::onRunContextMenu(CefRefPtr<CefBrowser>& browser,
                                     CefRefPtr<CefFrame>& frame,
                                     CefRefPtr<CefContextMenuParams>& params,
                                     CefRefPtr<CefMenuModel>& model,
                                     CefRefPtr<CefRunContextMenuCallback>& callback)
{
  FLog();

  // popup browser doesn't involve off-screen rendering
  if (browser->IsPopup()) {
    return false;
  }

  if (pCefViewPrivate_->isOSRModeEnabled_) {
    // OSR mode, create context menu with CEF built-in menu mode and show as customized context menu
    QPoint pos(params->GetXCoord(), params->GetYCoord());
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onRunCefContextMenu(pos, callback); });
    return true;
  } else {
    // NCW mode, to allow CEF native context menu
    return false;
  }
}

bool
CCefClientDelegate::onContextMenuCommand(CefRefPtr<CefBrowser>& browser,
                                         CefRefPtr<CefFrame>& frame,
                                         CefRefPtr<CefContextMenuParams>& params,
                                         int command_id,
                                         CefContextMenuHandler::EventFlags event_flags)
{
  FLog();

  return false;
}

void
CCefClientDelegate::onContextMenuDismissed(CefRefPtr<CefBrowser>& browser, CefRefPtr<CefFrame>& frame)
{
  FLog();

  if (pCefViewPrivate_->isOSRModeEnabled_) {
    // OSR mode
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onCefContextMenuDismissed(); });
  }
}
