#include "details/CCefClientDelegate.h"

#include <QDebug>

#include "details/QCefViewPrivate.h"

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser>& browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut)
{
  // qDebug("===== CefKeyEvent:type=%d, modifiers=0x%08x, windows_key_code=%d, native_key_code=%d, char=0x%04x, "
  //        "unmod_char=0x%04x",
  //        event.type,
  //        event.modifiers,
  //        event.windows_key_code,
  //        event.native_key_code,
  //        event.character,
  //        event.unmodified_character);

#if defined(Q_OS_MACOS)
  // it seems Qt breaks the macOS shortcut key, so we need to fix it
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0:  // A SelectAll
      case 6:  // Z Undo
      case 7:  // X Cut
      case 8:  // C Copy
      case 9:  // V Paste
      case 16: // Y Redo
        *is_keyboard_shortcut = true;
        break;
    }
  }
#endif

  return false;
}

bool
CCefClientDelegate::onKeyEvent(CefRefPtr<CefBrowser>& browser, const CefKeyEvent& event, CefEventHandle os_event)
{
#if defined(Q_OS_MACOS)
  // it seems Qt breaks the macOS shortcut key, so we need to fix it
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0: // A
        browser->GetFocusedFrame()->SelectAll();
        break;
      case 6: // Z
        browser->GetFocusedFrame()->Undo();
        break;
      case 7: // X
        browser->GetFocusedFrame()->Cut();
        break;
      case 8: // C
        browser->GetFocusedFrame()->Copy();
        break;
      case 9: // V
        browser->GetFocusedFrame()->Paste();
        break;
      case 16: // Y
        browser->GetFocusedFrame()->Redo();
        break;
    }
  }
#endif

  return false;
}
