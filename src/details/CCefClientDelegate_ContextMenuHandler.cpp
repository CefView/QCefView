#include "CCefClientDelegate.h"

#include "QCefViewPrivate.h"

void
CCefClientDelegate::onBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model)
{}

bool
CCefClientDelegate::onContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         int command_id,
                                         CefContextMenuHandler::EventFlags event_flags)
{
  return false;
}
