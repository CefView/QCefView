#include "../../details/QCefViewPrivate.h"

#include <windowsx.h>

bool
IsKeyDown(WPARAM wparam)
{
  return (GetKeyState(wparam) & 0x8000) != 0;
}

int
GetCefMouseModifiers(WPARAM wparam)
{
  int modifiers = 0;
  if (wparam & MK_CONTROL)
    modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (wparam & MK_SHIFT)
    modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (IsKeyDown(VK_MENU))
    modifiers |= EVENTFLAG_ALT_DOWN;
  if (wparam & MK_LBUTTON)
    modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (wparam & MK_MBUTTON)
    modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (wparam & MK_RBUTTON)
    modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

  // Low bit set from GetKeyState indicates "toggled".
  if (::GetKeyState(VK_NUMLOCK) & 1)
    modifiers |= EVENTFLAG_NUM_LOCK_ON;
  if (::GetKeyState(VK_CAPITAL) & 1)
    modifiers |= EVENTFLAG_CAPS_LOCK_ON;
  return modifiers;
}

int
GetCefKeyboardModifiers(quint32 vk, quint32 scode)
{
  int modifiers = 0;
  if (IsKeyDown(VK_SHIFT))
    modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (IsKeyDown(VK_CONTROL))
    modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (IsKeyDown(VK_MENU))
    modifiers |= EVENTFLAG_ALT_DOWN;

  // Low bit set from GetKeyState indicates "toggled".
  if (::GetKeyState(VK_NUMLOCK) & 1)
    modifiers |= EVENTFLAG_NUM_LOCK_ON;
  if (::GetKeyState(VK_CAPITAL) & 1)
    modifiers |= EVENTFLAG_CAPS_LOCK_ON;

  switch (vk) {
    case VK_RETURN:
      if ((scode >> 16) & KF_EXTENDED)
        modifiers |= EVENTFLAG_IS_KEY_PAD;
      break;
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
      if (!((scode >> 16) & KF_EXTENDED))
        modifiers |= EVENTFLAG_IS_KEY_PAD;
      break;
    case VK_NUMLOCK:
    case VK_NUMPAD0:
    case VK_NUMPAD1:
    case VK_NUMPAD2:
    case VK_NUMPAD3:
    case VK_NUMPAD4:
    case VK_NUMPAD5:
    case VK_NUMPAD6:
    case VK_NUMPAD7:
    case VK_NUMPAD8:
    case VK_NUMPAD9:
    case VK_DIVIDE:
    case VK_MULTIPLY:
    case VK_SUBTRACT:
    case VK_ADD:
    case VK_DECIMAL:
    case VK_CLEAR:
      modifiers |= EVENTFLAG_IS_KEY_PAD;
      break;
    case VK_SHIFT:
      if (IsKeyDown(VK_LSHIFT))
        modifiers |= EVENTFLAG_IS_LEFT;
      else if (IsKeyDown(VK_RSHIFT))
        modifiers |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_CONTROL:
      if (IsKeyDown(VK_LCONTROL))
        modifiers |= EVENTFLAG_IS_LEFT;
      else if (IsKeyDown(VK_RCONTROL))
        modifiers |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_MENU:
      if (IsKeyDown(VK_LMENU))
        modifiers |= EVENTFLAG_IS_LEFT;
      else if (IsKeyDown(VK_RMENU))
        modifiers |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_LWIN:
      modifiers |= EVENTFLAG_IS_LEFT;
      break;
    case VK_RWIN:
      modifiers |= EVENTFLAG_IS_RIGHT;
      break;
  }
  return modifiers;
}

bool
onKeyMessage(CefBrowserHost* host, UINT message, WPARAM wParam, LPARAM lParam)
{
  CefKeyEvent event;
  event.windows_key_code = wParam;
  event.native_key_code = lParam;
  event.is_system_key = message == WM_SYSCHAR || message == WM_SYSKEYDOWN || message == WM_SYSKEYUP;

  if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
    event.type = KEYEVENT_RAWKEYDOWN;
  else if (message == WM_KEYUP || message == WM_SYSKEYUP)
    event.type = KEYEVENT_KEYUP;
  else
    event.type = KEYEVENT_CHAR;
  event.modifiers = GetCefKeyboardModifiers(wParam, lParam);

  // mimic alt-gr check behavior from
  if ((event.type == KEYEVENT_CHAR) && IsKeyDown(VK_RMENU)) {
    // reverse AltGr detection taken from PlatformKeyMap::UsesAltGraph
    // instead of checking all combination for ctrl-alt, just check current char
    HKL current_layout = ::GetKeyboardLayout(0);

    // https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-vkkeyscanexw
    // ... high-order byte contains the shift state,
    // which can be a combination of the following flag bits.
    // 2 Either CTRL key is pressed.
    // 4 Either ALT key is pressed.
    SHORT scan_res = ::VkKeyScanExW(wParam, current_layout);
    if (((scan_res >> 8) & 0xFF) == (2 | 4)) {
      // ctrl-alt pressed
      event.modifiers &= ~(EVENTFLAG_CONTROL_DOWN | EVENTFLAG_ALT_DOWN);
      event.modifiers |= EVENTFLAG_ALTGR_DOWN;
    }
  }
  host->SendKeyEvent(event);

  // if the key is TAB, we just return true to terminate
  // the propagation of the message
  return event.windows_key_code == VK_TAB;
}

bool
QCefViewPrivate::nativeEventFilter(const QByteArray& eventType,
                                   void* m,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                   qintptr* result
#else
                                   long* result
#endif
)
{
#if defined(CEF_USE_OSR)
  if (!pCefBrowser_ || !q_ptr->hasFocus())
    return false;

  auto host = pCefBrowser_->GetHost();
  bool ret = false;
  MSG* msg = reinterpret_cast<MSG*>(m);
  UINT message = msg->message;
  WPARAM wParam = msg->wParam;
  LPARAM lParam = msg->lParam;

  switch (message) {
    case WM_SYSCHAR:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_CHAR:
      ret = onKeyMessage(host.get(), message, wParam, lParam);
      break;
    default:
      break;
  }
  return ret;
#else
  return false;
#endif
}
