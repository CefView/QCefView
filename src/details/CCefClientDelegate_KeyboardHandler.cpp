#include "CCefClientDelegate.h"

#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

bool
CCefClientDelegate::onPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                  const CefKeyEvent& event,
                                  CefEventHandle os_event,
                                  bool* is_keyboard_shortcut)
{
  // qDebug() << "CefKeyEvent:" << event.type;
#if defined(Q_OS_MACOS)
  if (event.modifiers & EVENTFLAG_COMMAND_DOWN && event.type == KEYEVENT_RAWKEYDOWN) {
    switch (event.windows_key_code) {
      case 0x41: // A
        browser->GetFocusedFrame()->SelectAll();
        *is_keyboard_shortcut = true;
        break;
      case 0x43: // C
        browser->GetFocusedFrame()->Copy();
        *is_keyboard_shortcut = true;
        break;
      case 0x56: // V
        browser->GetFocusedFrame()->Paste();
        *is_keyboard_shortcut = true;
        break;
      case 0x58: // X
        browser->GetFocusedFrame()->Cut();
        *is_keyboard_shortcut = true;
        break;
      case 0x59: // Y
        browser->GetFocusedFrame()->Redo();
        *is_keyboard_shortcut = true;
        break;
      case 0x5a: // Z
        browser->GetFocusedFrame()->Undo();
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
