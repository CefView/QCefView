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
  if (browser->IsPopup()) {
    if (pCefViewPrivate_->disablePopuContextMenu_) {
      model->Clear();
    }

    return;
  }

  // main browser
  auto policy = pCefViewPrivate_->q_ptr->contextMenuPolicy();
  if (Qt::DefaultContextMenu != policy) {
    model->Clear();
    return;
  }

  auto menuData = MenuBuilder::CreateMenuDataFromCefMenu(model.get());
  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onBeforeCefContextMenu(menuData); });

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

  if (browser->IsPopup()) {
    return false;
  }

  auto policy = pCefViewPrivate_->q_ptr->contextMenuPolicy();
  if (Qt::DefaultContextMenu != policy) {
    return false;
  }

  QPoint pos(params->GetXCoord(), params->GetYCoord());
  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onRunCefContextMenu(pos, callback); });

  return true;
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

  QMetaObject::invokeMethod(pCefViewPrivate_, [=]() { pCefViewPrivate_->onCefContextMenuDismissed(); });
}
