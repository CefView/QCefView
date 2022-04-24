#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::onBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model)
{
#if defined(CEF_USE_OSR)
  // Context menu will not disappear on left click under OSR mode,
  // so we just disable the default one you need to implement your own context menu
  model->Clear();
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
{}
