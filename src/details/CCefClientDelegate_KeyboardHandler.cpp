#include "CCefClientDelegate.h"

#include <QDebug>

#include "QCefViewPrivate.h"

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser> browser,
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
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.native_key_code) {
      case 0: // A
        browser->GetFocusedFrame()->SelectAll();
        *is_keyboard_shortcut = true;
        break;
      case 6: // Z
        browser->GetFocusedFrame()->Undo();
        *is_keyboard_shortcut = true;
        break;
      case 7: // X
        browser->GetFocusedFrame()->Cut();
        *is_keyboard_shortcut = true;
        break;
      case 8: // C
        browser->GetFocusedFrame()->Copy();
        *is_keyboard_shortcut = true;
        break;
      case 9: // V
        browser->GetFocusedFrame()->Paste();
        *is_keyboard_shortcut = true;
        break;
      case 16: // Y
        browser->GetFocusedFrame()->Redo();
        *is_keyboard_shortcut = true;
        break;
    }
  }
#endif

  return false;
}

bool
CCefClientDelegate::onKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
  return false;
}
