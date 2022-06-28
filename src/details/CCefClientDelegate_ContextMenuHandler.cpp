#include "CCefClientDelegate.h"

#include <QDebug>

#include "QCefViewPrivate.h"

void
CCefClientDelegate::onBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model)
{
  if (!pCefViewPrivate_)
    return;
  
  qDebug() << "onBeforeContextMenu, menu item count:" << model->GetCount();
  
  if (browser->IsPopup()) {
    // pop-up browser doesn't involve off-screen renderring
    if (Qt::CefDisablePopupContextMenu & pCefViewPrivate_->cefContextMenuPolicy_) {
      model->Clear();
    }
  } else {
    // main browser
    if (Qt::CefDisableMainContextMenu & pCefViewPrivate_->cefContextMenuPolicy_) {
      model->Clear();
    }
  }
  
#if defined(CEF_USE_OSR)
  // Context menu will not disappear on left click under OSR
  // mode, so we just disable the default one. You need to
  // implement your own context menu.
#endif
}

bool
CCefClientDelegate::onRunContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model,
                                     CefRefPtr<CefRunContextMenuCallback> callback)
{
  return false;
}

bool
CCefClientDelegate::onContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         int command_id,
                                         CefContextMenuHandler::EventFlags event_flags)
{
  return false;
}

void
CCefClientDelegate::onContextMenuDismissed(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
  qDebug() << "onContextMenuDismissed";
}
