#if defined(CEF_USE_OSR)
#include "KeyboardUtils.h"

#include <include/cef_app.h>

static QMap<int, qint32> keyMap = {
  { Qt::Key_Left, 0x25 },
  { Qt::Key_Up, 0x26 },
  { Qt::Key_Right, 0x27 },
  { Qt::Key_Down, 0x28 },
  { Qt::Key_Backspace, 0x08 },
  { Qt::Key_Tab, 0x09 },
  { Qt::Key_Backtab, 0x09 },
  { Qt::Key_Clear, 0x0C },
  { Qt::Key_Return, 0x0D },
  { Qt::Key_Enter, 0x0D },
  { Qt::Key_Shift, 0x10 },
  { Qt::Key_Control, 0x11 },
  { Qt::Key_Alt, 0x12 },
  { Qt::Key_Pause, 0x13 },
  { Qt::Key_CapsLock, 0x14 },
  { Qt::Key_Escape, 0x1B },
  { Qt::Key_Space, 0x20 },
  { Qt::Key_PageUp, 0x21 },
  { Qt::Key_PageDown, 0x22 },
  { Qt::Key_End, 0x23 },
  { Qt::Key_Home, 0x24 },
  { Qt::Key_Select, 0x29 },
  { Qt::Key_Print, 0x2A },
  { Qt::Key_Execute, 0x2B },
  { Qt::Key_Printer, 0x2C },
  { Qt::Key_Insert, 0x2D },
  { Qt::Key_Delete, 0x2E },
  { Qt::Key_Help, 0x2F },
  { Qt::Key_0, 0x30 },
  { Qt::Key_ParenRight, 0x30 }, // )
  { Qt::Key_1, 0x31 },
  { Qt::Key_Exclam, 0x31 }, // !
  { Qt::Key_2, 0x32 },
  { Qt::Key_At, 0x32 }, // @
  { Qt::Key_3, 0x33 },
  { Qt::Key_NumberSign, 0x33 }, // #
  { Qt::Key_4, 0x34 },
  { Qt::Key_Dollar, 0x34 }, // $
  { Qt::Key_5, 0x35 },
  { Qt::Key_Percent, 0x35 }, // %
  { Qt::Key_6, 0x36 },
  { Qt::Key_AsciiCircum, 0x36 }, // ^
  { Qt::Key_7, 0x37 },
  { Qt::Key_Ampersand, 0x37 }, // &
  { Qt::Key_8, 0x38 },
  { Qt::Key_Asterisk, 0x38 }, // *
  { Qt::Key_9, 0x39 },
  { Qt::Key_ParenLeft, 0x39 }, // (
  { Qt::Key_A, 0x41 },
  { Qt::Key_B, 0x42 },
  { Qt::Key_C, 0x43 },
  { Qt::Key_D, 0x44 },
  { Qt::Key_E, 0x45 },
  { Qt::Key_F, 0x46 },
  { Qt::Key_G, 0x47 },
  { Qt::Key_H, 0x48 },
  { Qt::Key_I, 0x49 },
  { Qt::Key_J, 0x4A },
  { Qt::Key_K, 0x4B },
  { Qt::Key_L, 0x4C },
  { Qt::Key_M, 0x4D },
  { Qt::Key_N, 0x4E },
  { Qt::Key_O, 0x4F },
  { Qt::Key_P, 0x50 },
  { Qt::Key_Q, 0x51 },
  { Qt::Key_R, 0x52 },
  { Qt::Key_S, 0x53 },
  { Qt::Key_T, 0x54 },
  { Qt::Key_U, 0x55 },
  { Qt::Key_V, 0x56 },
  { Qt::Key_W, 0x57 },
  { Qt::Key_X, 0x58 },
  { Qt::Key_Y, 0x59 },
  { Qt::Key_Z, 0x5A },
  { Qt::Key_multiply, 0x6A },
  { Qt::Key_F1, 0x70 },
  { Qt::Key_F2, 0x71 },
  { Qt::Key_F3, 0x72 },
  { Qt::Key_F4, 0x73 },
  { Qt::Key_F5, 0x74 },
  { Qt::Key_F6, 0x75 },
  { Qt::Key_F7, 0x76 },
  { Qt::Key_F8, 0x77 },
  { Qt::Key_F9, 0x78 },
  { Qt::Key_F10, 0x79 },
  { Qt::Key_F11, 0x7A },
  { Qt::Key_F12, 0x7B },
  { Qt::Key_F13, 0x7C },
  { Qt::Key_F14, 0x7D },
  { Qt::Key_F15, 0x7E },
  { Qt::Key_F16, 0x7F },
  { Qt::Key_F17, 0x80 },
  { Qt::Key_F18, 0x81 },
  { Qt::Key_F19, 0x82 },
  { Qt::Key_F20, 0x83 },
  { Qt::Key_F21, 0x84 },
  { Qt::Key_F22, 0x85 },
  { Qt::Key_F23, 0x86 },
  { Qt::Key_F24, 0x87 },
  { Qt::Key_NumLock, 0x90 },
  { Qt::Key_ScrollLock, 0x91 },
  { Qt::Key_VolumeDown, 0xAE },
  { Qt::Key_VolumeUp, 0xAF },
  { Qt::Key_VolumeMute, 0xAD },
  { Qt::Key_MediaStop, 0xB2 },
  { Qt::Key_MediaPlay, 0xB3 },
  { Qt::Key_Plus, 0xBB },         // +
  { Qt::Key_Minus, 0xBD },        // -
  { Qt::Key_Underscore, 0xBD },   // _
  { Qt::Key_Equal, 0xBB },        // =
  { Qt::Key_Semicolon, 0xBA },    // ;
  { Qt::Key_Colon, 0xBA },        // :
  { Qt::Key_Comma, 0xBC },        // ,
  { Qt::Key_Less, 0xBC },         // <
  { Qt::Key_Period, 0xBE },       // .
  { Qt::Key_Greater, 0xBE },      // >
  { Qt::Key_Slash, 0xBF },        // /
  { Qt::Key_Question, 0xBF },     // ?
  { Qt::Key_BracketLeft, 0xDB },  // [
  { Qt::Key_BraceLeft, 0xDB },    // {
  { Qt::Key_BracketRight, 0xDD }, // ]
  { Qt::Key_BraceRight, 0xDD },   // }
  { Qt::Key_Bar, 0xDC },          // |
  { Qt::Key_Backslash, 0xDC },    // "\"
  { Qt::Key_Apostrophe, 0xDE },   // '
  { Qt::Key_QuoteDbl, 0xDE },     // "
  { Qt::Key_QuoteLeft, 0xC0 },    // `
  { Qt::Key_AsciiTilde, 0xC0 },   // ~
};

quint32
QtKeyToWindowsVirtualKey(int k)
{
  if (keyMap.contains(k))
    return keyMap[k];

  return 0;
}

uint32_t
GetPlatformKeyboardCode(QKeyEvent* event)
{
  return QtKeyToWindowsVirtualKey(event->key());
}

uint32_t
GetPlatformKeyboardModifiers(QKeyEvent* event)
{
  uint32_t cm = 0;

#if defined(Q_OS_WINDOWS)
  if (::GetKeyState(VK_NUMLOCK) & 1)
    cm |= EVENTFLAG_NUM_LOCK_ON;
  if (::GetKeyState(VK_CAPITAL) & 1)
    cm |= EVENTFLAG_CAPS_LOCK_ON;

  auto vk = event->nativeVirtualKey();
  switch (vk) {
    case VK_SHIFT:
      if (::GetKeyState(VK_LSHIFT) & 0x8000)
        cm |= EVENTFLAG_IS_LEFT;
      else if (::GetKeyState(VK_RSHIFT) & 0x8000)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_CONTROL:
      if (::GetKeyState(VK_LCONTROL) & 0x8000)
        cm |= EVENTFLAG_IS_LEFT;
      else if (::GetKeyState(VK_RCONTROL) & 0x8000)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_MENU:
      if (::GetKeyState(VK_LMENU) & 0x8000)
        cm |= EVENTFLAG_IS_LEFT;
      else if (::GetKeyState(VK_RMENU) & 0x8000)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case VK_LWIN:
      cm |= EVENTFLAG_IS_LEFT;
      break;
    case VK_RWIN:
      cm |= EVENTFLAG_IS_RIGHT;
      break;
  }

  // mimic alt-gr check behavior from
  if (!event->text().isEmpty() && (::GetKeyState(VK_RMENU) & 0x8000)) {
    // reverse AltGr detection taken from PlatformKeyMap::UsesAltGraph
    // instead of checking all combination for ctrl-alt, just check current char
    HKL current_layout = ::GetKeyboardLayout(0);

    // https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-vkkeyscanexw
    // ... high-order byte contains the shift state,
    // which can be a combination of the following flag bits.
    // 2 Either CTRL key is pressed.
    // 4 Either ALT key is pressed.
    SHORT scan_res = ::VkKeyScanExW(vk, current_layout);
    if (((scan_res >> 8) & 0xFF) == (2 | 4)) {
      // ctrl-alt pressed
      cm &= ~(EVENTFLAG_CONTROL_DOWN | EVENTFLAG_ALT_DOWN);
      cm |= EVENTFLAG_ALTGR_DOWN;
    }
  }
#elif defined(Q_OS_LINUX)
  typedef enum
  {
    GDK_SHIFT_MASK = 1 << 0,
    GDK_LOCK_MASK = 1 << 1,
    GDK_CONTROL_MASK = 1 << 2,
    GDK_MOD1_MASK = 1 << 3,
    GDK_MOD2_MASK = 1 << 4,
    GDK_MOD3_MASK = 1 << 5,
    GDK_MOD4_MASK = 1 << 6,
    GDK_MOD5_MASK = 1 << 7,
    GDK_BUTTON1_MASK = 1 << 8,
    GDK_BUTTON2_MASK = 1 << 9,
    GDK_BUTTON3_MASK = 1 << 10,
    GDK_BUTTON4_MASK = 1 << 11,
    GDK_BUTTON5_MASK = 1 << 12,
    GDK_MODIFIER_RESERVED_13_MASK = 1 << 13,
    GDK_MODIFIER_RESERVED_14_MASK = 1 << 14,
    GDK_MODIFIER_RESERVED_15_MASK = 1 << 15,
    GDK_MODIFIER_RESERVED_16_MASK = 1 << 16,
    GDK_MODIFIER_RESERVED_17_MASK = 1 << 17,
    GDK_MODIFIER_RESERVED_18_MASK = 1 << 18,
    GDK_MODIFIER_RESERVED_19_MASK = 1 << 19,
    GDK_MODIFIER_RESERVED_20_MASK = 1 << 20,
    GDK_MODIFIER_RESERVED_21_MASK = 1 << 21,
    GDK_MODIFIER_RESERVED_22_MASK = 1 << 22,
    GDK_MODIFIER_RESERVED_23_MASK = 1 << 23,
    GDK_MODIFIER_RESERVED_24_MASK = 1 << 24,
    GDK_MODIFIER_RESERVED_25_MASK = 1 << 25,
    /* The next few modifiers are used by XKB, so we skip to the end.
     * Bits 15 - 25 are currently unused. Bit 29 is used internally.
     */

    GDK_SUPER_MASK = 1 << 26,
    GDK_HYPER_MASK = 1 << 27,
    GDK_META_MASK = 1 << 28,

    GDK_MODIFIER_RESERVED_29_MASK = 1 << 29,
    GDK_RELEASE_MASK = 1 << 30,
    /* Combination of GDK_SHIFT_MASK..GDK_BUTTON5_MASK + GDK_SUPER_MASK
       + GDK_HYPER_MASK + GDK_META_MASK + GDK_RELEASE_MASK */
    GDK_MODIFIER_MASK = 0x5c001fff
  } GdkModifierType;

  auto state = event->nativeModifiers();
  if (state & GDK_SHIFT_MASK)
    cm |= EVENTFLAG_SHIFT_DOWN;
  if (state & GDK_LOCK_MASK)
    cm |= EVENTFLAG_CAPS_LOCK_ON;
  if (state & GDK_CONTROL_MASK)
    cm |= EVENTFLAG_CONTROL_DOWN;
  if (state & GDK_MOD1_MASK)
    cm |= EVENTFLAG_ALT_DOWN;
  if (state & GDK_BUTTON1_MASK)
    cm |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (state & GDK_BUTTON2_MASK)
    cm |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (state & GDK_BUTTON3_MASK)
    cm |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
#elif defined(Q_OS_MACOS)
  auto m = event->modifiers();
  cm |= m & Qt::ControlModifier ? EVENTFLAG_COMMAND_DOWN : 0;
#else
#endif

  return cm;
}
#endif
