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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // popup browser doesn't involve off-screen rendering
  if (browser->IsPopup()) {
    return;
  }

  // main browser
  Qt::ContextMenuPolicy policy = Qt::DefaultContextMenu;
  runInMainThreadAndWait([&]() {
    policy = Qt::DefaultContextMenu;
    if (pCefViewPrivate->q_ptr) {
      policy = pCefViewPrivate->q_ptr->contextMenuPolicy();
    }
  });
  if (Qt::DefaultContextMenu != policy) {
    // for all non-default value, we clear the menu info to prevent the showing
    model->Clear();
    return;
  }

  if (pCefViewPrivate->isOSRModeEnabled_) {
    // OSR mode
    auto menuData = MenuBuilder::CreateMenuDataFromCefMenu(model.get());
    runInMainThread(                  //
      [pCefViewPrivate, menuData]() { //
        pCefViewPrivate->onBeforeCefContextMenu(menuData);
      });
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
  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, false);

  // popup browser doesn't involve off-screen rendering
  if (browser->IsPopup()) {
    return false;
  }

  if (pCefViewPrivate->isOSRModeEnabled_) {
    // OSR mode, create context menu with CEF built-in menu mode and show as customized context menu
    QPoint pos(params->GetXCoord(), params->GetYCoord());
    runInMainThread(                       //
      [pCefViewPrivate, pos, callback]() { //
        pCefViewPrivate->onRunCefContextMenu(pos, callback);
      });
    return true;
  }

  // NCW mode, to allow CEF native context menu
  return false;
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  if (pCefViewPrivate->isOSRModeEnabled_) {
    // OSR mode
    runInMainThread(        //
      [pCefViewPrivate]() { //
        pCefViewPrivate->onCefContextMenuDismissed();
      });
  }
}
