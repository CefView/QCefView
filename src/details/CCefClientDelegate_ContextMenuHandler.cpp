#include "CCefClientDelegate.h"

#include <QDebug>
#include <QThread>

#include "utils/CommonUtils.h"
#include "utils/MenuBuilder.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::onBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model)
{
  FLog();

  if (!pCefViewPrivate_)
    return;

  // popup browser doesn't involve off-screen rendering
  // if (browser->IsPopup()) {
  //  if (pCefViewPrivate_->disablePopuContextMenu_) {
  //    model->Clear();
  //  }

  //  return;
  //}

  // main browser
  auto policy = pCefViewPrivate_->q_ptr->contextMenuPolicy();
  if (Qt::DefaultContextMenu != policy) {
    // for all non-default value, we clear the menu info to prevent the showing
    model->Clear();
    return;
  }

  // #if defined(CEF_USE_OSR)
  if (pCefViewPrivate_->isOSRModeEnabled()) {
    auto menuData = MenuBuilder::CreateMenuDataFromCefMenu(model.get());
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onBeforeCefContextMenu(menuData); });
  }
  // #endif

  return;
}

bool
CCefClientDelegate::onRunContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model,
                                     CefRefPtr<CefRunContextMenuCallback> callback)
{
  FLog();

  // if (browser->IsPopup()) {
  //   return false;
  // }

  // #if defined(CEF_USE_OSR)
  if (pCefViewPrivate_->isOSRModeEnabled()) {
    // OSR mode, create context menu with CEF built-in menu mode and show as customized context menu
    QPoint pos(params->GetXCoord(), params->GetYCoord());
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onRunCefContextMenu(pos, callback); });
    return true;
  } else { // #else
    // NCW mode, allow allow CEF native context menu
    return false;
  }
  // #endif
}

bool
CCefClientDelegate::onContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         int command_id,
                                         CefContextMenuHandler::EventFlags event_flags)
{
  FLog();

  return false;
}

void
CCefClientDelegate::onContextMenuDismissed(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
  FLog();

  // #if defined(CEF_USE_OSR)
  if (pCefViewPrivate_->isOSRModeEnabled()) {
    QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onCefContextMenuDismissed(); });
  }
  // #endif
}
