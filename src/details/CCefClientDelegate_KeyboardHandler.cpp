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
  qDebug() << "CefKeyEvent:" << event.type;
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
