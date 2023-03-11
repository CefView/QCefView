#include "KeyboardUtils.h"

#if defined(Q_OS_MACOS)
/* MacRoman character codes*/
enum
{
  kNullCharCode = 0,
  kHomeCharCode = 1,
  kEnterCharCode = 3,
  kEndCharCode = 4,
  kHelpCharCode = 5,
  kBellCharCode = 7,
  kBackspaceCharCode = 8,
  kTabCharCode = 9,
  kLineFeedCharCode = 10,
  kVerticalTabCharCode = 11,
  kPageUpCharCode = 11,
  kFormFeedCharCode = 12,
  kPageDownCharCode = 12,
  kReturnCharCode = 13,
  kFunctionKeyCharCode = 16,
  kCommandCharCode = 17,   /* glyph available only in system fonts*/
  kCheckCharCode = 18,     /* glyph available only in system fonts*/
  kDiamondCharCode = 19,   /* glyph available only in system fonts*/
  kAppleLogoCharCode = 20, /* glyph available only in system fonts*/
  kEscapeCharCode = 27,
  kClearCharCode = 27,
  kLeftArrowCharCode = 28,
  kRightArrowCharCode = 29,
  kUpArrowCharCode = 30,
  kDownArrowCharCode = 31,
  kSpaceCharCode = 32,
  kDeleteCharCode = 127,
  kBulletCharCode = 165,
  kNonBreakingSpaceCharCode = 202
};

/* useful Unicode code points*/
enum
{
  kShiftUnicode = 0x21E7,      /* Unicode UPWARDS WHITE ARROW*/
  kControlUnicode = 0x2303,    /* Unicode UP ARROWHEAD*/
  kOptionUnicode = 0x2325,     /* Unicode OPTION KEY*/
  kCommandUnicode = 0x2318,    /* Unicode PLACE OF INTEREST SIGN*/
  kPencilUnicode = 0x270E,     /* Unicode LOWER RIGHT PENCIL; actually pointed left until Mac OS X 10.3*/
  kPencilLeftUnicode = 0xF802, /* Unicode LOWER LEFT PENCIL; available in Mac OS X 10.3 and later*/
  kCheckUnicode = 0x2713,      /* Unicode CHECK MARK*/
  kDiamondUnicode = 0x25C6,    /* Unicode BLACK DIAMOND*/
  kBulletUnicode = 0x2022,     /* Unicode BULLET*/
  kAppleLogoUnicode = 0xF8FF   /* Unicode APPLE LOGO*/
};

/*
 *  Summary:
 *    Virtual keycodes
 *
 *  Discussion:
 *    These constants are the virtual keycodes defined originally in
 *    Inside Mac Volume V, pg. V-191. They identify physical keys on a
 *    keyboard. Those constants with "ANSI" in the name are labeled
 *    according to the key position on an ANSI-standard US keyboard.
 *    For example, kVK_ANSI_A indicates the virtual keycode for the key
 *    with the letter 'A' in the US keyboard layout. Other keyboard
 *    layouts may have the 'A' key label on a different physical key;
 *    in this case, pressing 'A' will generate a different virtual
 *    keycode.
 */
enum
{
  kVK_ANSI_A = 0x00,
  kVK_ANSI_S = 0x01,
  kVK_ANSI_D = 0x02,
  kVK_ANSI_F = 0x03,
  kVK_ANSI_H = 0x04,
  kVK_ANSI_G = 0x05,
  kVK_ANSI_Z = 0x06,
  kVK_ANSI_X = 0x07,
  kVK_ANSI_C = 0x08,
  kVK_ANSI_V = 0x09,
  kVK_ANSI_B = 0x0B,
  kVK_ANSI_Q = 0x0C,
  kVK_ANSI_W = 0x0D,
  kVK_ANSI_E = 0x0E,
  kVK_ANSI_R = 0x0F,
  kVK_ANSI_Y = 0x10,
  kVK_ANSI_T = 0x11,
  kVK_ANSI_1 = 0x12,
  kVK_ANSI_2 = 0x13,
  kVK_ANSI_3 = 0x14,
  kVK_ANSI_4 = 0x15,
  kVK_ANSI_6 = 0x16,
  kVK_ANSI_5 = 0x17,
  kVK_ANSI_Equal = 0x18,
  kVK_ANSI_9 = 0x19,
  kVK_ANSI_7 = 0x1A,
  kVK_ANSI_Minus = 0x1B,
  kVK_ANSI_8 = 0x1C,
  kVK_ANSI_0 = 0x1D,
  kVK_ANSI_RightBracket = 0x1E,
  kVK_ANSI_O = 0x1F,
  kVK_ANSI_U = 0x20,
  kVK_ANSI_LeftBracket = 0x21,
  kVK_ANSI_I = 0x22,
  kVK_ANSI_P = 0x23,
  kVK_Return = 0x24,
  kVK_ANSI_L = 0x25,
  kVK_ANSI_J = 0x26,
  kVK_ANSI_Quote = 0x27,
  kVK_ANSI_K = 0x28,
  kVK_ANSI_Semicolon = 0x29,
  kVK_ANSI_Backslash = 0x2A,
  kVK_ANSI_Comma = 0x2B,
  kVK_ANSI_Slash = 0x2C,
  kVK_ANSI_N = 0x2D,
  kVK_ANSI_M = 0x2E,
  kVK_ANSI_Period = 0x2F,
  kVK_Space = 0x31,
  kVK_ANSI_Grave = 0x32,
  kVK_Command = 0x37,
  kVK_Shift = 0x38,
  kVK_CapsLock = 0x39,
  kVK_Option = 0x3A,
  kVK_Control = 0x3B,
  kVK_RightShift = 0x3C,
  kVK_RightOption = 0x3D,
  kVK_RightControl = 0x3E,
  kVK_ANSI_KeypadDecimal = 0x41,
  kVK_ANSI_KeypadMultiply = 0x43,
  kVK_ANSI_KeypadPlus = 0x45,
  kVK_ANSI_KeypadClear = 0x47,
  kVK_ANSI_KeypadDivide = 0x4B,
  kVK_ANSI_KeypadEnter = 0x4C,
  kVK_ANSI_KeypadMinus = 0x4E,
  kVK_ANSI_KeypadEquals = 0x51,
  kVK_ANSI_Keypad0 = 0x52,
  kVK_ANSI_Keypad1 = 0x53,
  kVK_ANSI_Keypad2 = 0x54,
  kVK_ANSI_Keypad3 = 0x55,
  kVK_ANSI_Keypad4 = 0x56,
  kVK_ANSI_Keypad5 = 0x57,
  kVK_ANSI_Keypad6 = 0x58,
  kVK_ANSI_Keypad7 = 0x59,
  kVK_ANSI_Keypad8 = 0x5B,
  kVK_ANSI_Keypad9 = 0x5C
};

/* keycodes for keys that are independent of keyboard layout*/
enum
{
  kVK_Tab = 0x30,
  kVK_Delete = 0x33,
  kVK_Escape = 0x35,
  kVK_Function = 0x3F,
  kVK_F17 = 0x40,
  kVK_VolumeUp = 0x48,
  kVK_VolumeDown = 0x49,
  kVK_Mute = 0x4A,
  kVK_F18 = 0x4F,
  kVK_F19 = 0x50,
  kVK_F20 = 0x5A,
  kVK_F5 = 0x60,
  kVK_F6 = 0x61,
  kVK_F7 = 0x62,
  kVK_F3 = 0x63,
  kVK_F8 = 0x64,
  kVK_F9 = 0x65,
  kVK_F11 = 0x67,
  kVK_F13 = 0x69,
  kVK_F16 = 0x6A,
  kVK_F14 = 0x6B,
  kVK_F10 = 0x6D,
  kVK_F12 = 0x6F,
  kVK_F15 = 0x71,
  kVK_Help = 0x72,
  kVK_Home = 0x73,
  kVK_PageUp = 0x74,
  kVK_ForwardDelete = 0x75,
  kVK_F4 = 0x76,
  kVK_End = 0x77,
  kVK_F2 = 0x78,
  kVK_PageDown = 0x79,
  kVK_F1 = 0x7A,
  kVK_LeftArrow = 0x7B,
  kVK_RightArrow = 0x7C,
  kVK_DownArrow = 0x7D,
  kVK_UpArrow = 0x7E
};

quint32
macGetNativeKeyCode(QKeyEvent* ke)
{
  static QMap<int, qint32> macOSKeyMap = {
    { Qt::Key_Control, kVK_Command },   //
    { Qt::Key_Shift, kVK_Shift },       //
    { Qt::Key_CapsLock, kVK_CapsLock }, //
    { Qt::Key_Alt, kVK_Option },        //
    { Qt::Key_Meta, kVK_Control },
  };

  if (macOSKeyMap.contains(ke->key())) {
    return macOSKeyMap[ke->key()];
  }

  return ke->nativeVirtualKey();
}

static QMap<int, qint16> macOSCharCodeMap = {
  { Qt::Key_Up, 0xF700 },          // NSUpArrowFunctionKey
  { Qt::Key_Down, 0xF701 },        // NSDownArrowFunctionKey
  { Qt::Key_Left, 0xF702 },        // NSLeftArrowFunctionKey
  { Qt::Key_Right, 0xF703 },       // NSRightArrowFunctionKey
  { Qt::Key_Insert, 0xF727 },      // NSInsertFunctionKey
  { Qt::Key_Delete, 0xF728 },      // NSDeleteFunctionKey
  { Qt::Key_Home, 0xF729 },        // NSHomeFunctionKey
  { Qt::Key_End, 0xF72B },         // NSEndFunctionKey
  { Qt::Key_PageUp, 0xF72C },      // NSPageUpFunctionKey
  { Qt::Key_PageDown, 0xF72D },    // NSPageDownFunctionKey
  { Qt::Key_ScrollLock, 0xF72F },  // NSScrollLockFunctionKey
  { Qt::Key_Pause, 0xF730 },       // NSPauseFunctionKey
  { Qt::Key_SysReq, 0xF731 },      // NSSysReqFunctionKey
  { Qt::Key_Menu, 0xF735 },        // NSMenuFunctionKey
  { Qt::Key_Print, 0xF738 },       // NSPrintFunctionKey
  { Qt::Key_Clear, 0xF73A },       // NSClearDisplayFunctionKey
  { Qt::Key_Insert, 0xF73D },      // NSInsertCharFunctionKey
  { Qt::Key_Delete, 0xF73E },      // NSDeleteCharFunctionKey
  { Qt::Key_Select, 0xF741 },      // NSSelectFunctionKey
  { Qt::Key_Execute, 0xF742 },     // NSExecuteFunctionKey
  { Qt::Key_Help, 0xF746 },        // NSHelpFunctionKey
  { Qt::Key_Mode_switch, 0xF747 }, // NSModeSwitchFunctionKey
};

static quint16
macGetKeyCharacter(QKeyEvent* qe)
{
  if (qe->key() == Qt::Key_Meta)
    return 0;

  if (qe->key() >= Qt::Key_F1 && qe->key() <= Qt::Key_F35) {
    return qe->key() + 0xf704 - Qt::Key_F1;
  }

  if (macOSCharCodeMap.contains(qe->key())) {
    return macOSCharCodeMap[qe->key()];
  }

  if (!qe->text().isEmpty()) {
    return qe->text().at(0).unicode();
  }

  return 0;
}

static quint16
macGetUnmodifiedKeyCharacter(QKeyEvent* qe)
{
  if (qe->key() == Qt::Key_Meta)
    return 0;

  if (qe->key() >= Qt::Key_F1 && qe->key() <= Qt::Key_F35) {
    return qe->key() + 0xf704 - Qt::Key_F1;
  }

  if (macOSCharCodeMap.contains(qe->key())) {
    return macOSCharCodeMap[qe->key()];
  }

  return qe->key();
}

static bool
macShouldSendKeyCharEvent(QKeyEvent* qe)
{
  if (qe->modifiers() & Qt::KeypadModifier)
    return false;

  switch (qe->key()) {
    case Qt::Key_Meta:
    case Qt::Key_Control:
    case Qt::Key_Shift:
    case Qt::Key_Alt:
    case Qt::Key_CapsLock:
      return false;
      break;
    default:
      break;
  }
  return true;
}

typedef enum NSEventModifierFlags
{
  NX_DEVICELCTLKEYMASK = 0x00000001,   // left ctrl
  NX_DEVICELSHIFTKEYMASK = 0x00000002, // left shift
  NX_DEVICERSHIFTKEYMASK = 0x00000004, // right shift
  NX_DEVICELCMDKEYMASK = 0x00000008,   // left cmd
  NX_DEVICERCMDKEYMASK = 0x00000010,   // right cmd
  NX_DEVICELALTKEYMASK = 0x00000020,   // left option
  NX_DEVICERALTKEYMASK = 0x00000040,   // right option
  NX_DEVICE_ALPHASHIFT_STATELESS_MASK = 0x00000080,
  NX_DEVICERCTLKEYMASK = 0x00002000, // right ctrl

  NSEventModifierFlagCapsLock = 1 << 16,   // Set if Caps Lock key is pressed.
  NSEventModifierFlagShift = 1 << 17,      // Set if Shift key is pressed.
  NSEventModifierFlagControl = 1 << 18,    // Set if Control key is pressed.
  NSEventModifierFlagOption = 1 << 19,     // Set if Option or Alternate key is pressed.
  NSEventModifierFlagCommand = 1 << 20,    // Set if Command key is pressed.
  NSEventModifierFlagNumericPad = 1 << 21, // Set if any key in the numeric keypad is pressed.
  NSEventModifierFlagHelp = 1 << 22,       // Set if the Help key is pressed.
  NSEventModifierFlagFunction = 1 << 23,   // Set if any function key is pressed.

  // Used to retrieve only the device-independent modifier flags, allowing applications to mask off the
  // device-dependent modifier flags, including event coalescing information.
  NSEventModifierFlagDeviceIndependentFlagsMask = 0xffff0000UL
} NSEventModifierFlags;

static uint32_t
macGetKeyboardModifiers(QKeyEvent* qe)
{
  uint32_t cm = 0;
  auto m = qe->modifiers();
  cm |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  cm |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  cm |= m & Qt::KeypadModifier ? EVENTFLAG_IS_KEY_PAD : 0;
  cm |= m & Qt::MetaModifier ? EVENTFLAG_CONTROL_DOWN : 0;
  cm |= m & Qt::ControlModifier ? EVENTFLAG_COMMAND_DOWN : 0;

  auto state = qe->nativeModifiers();
  switch (qe->key()) {
    case Qt::Key_CapsLock:
      if (state & NSEventModifierFlagCapsLock)
        cm |= EVENTFLAG_CAPS_LOCK_ON;
      break;
    case Qt::Key_Control:
      if (state & NX_DEVICELCMDKEYMASK)
        cm |= EVENTFLAG_IS_LEFT;
      else if (state & NX_DEVICERCMDKEYMASK)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case Qt::Key_Shift:
      if (state & NX_DEVICELSHIFTKEYMASK)
        cm |= EVENTFLAG_IS_LEFT;
      else if (state & NX_DEVICERSHIFTKEYMASK)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case Qt::Key_Meta:
      if (state & NX_DEVICELCTLKEYMASK)
        cm |= EVENTFLAG_IS_LEFT;
      else if (state & NX_DEVICERCTLKEYMASK)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    case Qt::Key_Option:
      if (state & NX_DEVICELALTKEYMASK)
        cm |= EVENTFLAG_IS_LEFT;
      else if (state & NX_DEVICERALTKEYMASK)
        cm |= EVENTFLAG_IS_RIGHT;
      break;
    default:
      break;
  }

  return cm;
}

static void
macQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce)
{
  ce.native_key_code = macGetNativeKeyCode(qe);
  ce.modifiers = macGetKeyboardModifiers(qe);
  ce.character = macGetKeyCharacter(qe);
  ce.unmodified_character = macGetUnmodifiedKeyCharacter(qe);

  if (ce.modifiers & EVENTFLAG_ALT_DOWN)
    ce.is_system_key = true;
}

#elif defined(Q_OS_WINDOWS) || defined(Q_OS_WIN)
static uint32_t
winGetKeyboardModifiers(QKeyEvent* qe)
{
  auto cm = 0;
  auto m = qe->modifiers();
  cm |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  cm |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  cm |= m & Qt::KeypadModifier ? EVENTFLAG_IS_KEY_PAD : 0;
  cm |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;

  if (::GetKeyState(VK_NUMLOCK) & 1)
    cm |= EVENTFLAG_NUM_LOCK_ON;
  if (::GetKeyState(VK_CAPITAL) & 1)
    cm |= EVENTFLAG_CAPS_LOCK_ON;

  auto vk = qe->nativeVirtualKey();
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

  // mimic alt-gr check behavior
  if (!qe->text().isEmpty() && (::GetKeyState(VK_RMENU) & 0x8000)) {
    if ((m & (Qt::ControlModifier | Qt::AltModifier)) == (Qt::ControlModifier | Qt::AltModifier)) {
      // ctrl-alt pressed
      cm &= ~(EVENTFLAG_CONTROL_DOWN | EVENTFLAG_ALT_DOWN);
      cm |= EVENTFLAG_ALTGR_DOWN;
    }

    //////////////////////////////////////////////////////////////////////////
    // because ::VkKeyScanExW needs wParam from Windows message as the parameter to
    // extract the scan code and here we cannot get it so the following code doesn't
    // work for windows

    //// reverse AltGr detection taken from PlatformKeyMap::UsesAltGraph
    //// instead of checking all combination for ctrl-alt, just check current char
    // HKL current_layout = ::GetKeyboardLayout(0);

    //// https://docs.microsoft.com/en-gb/windows/win32/api/winuser/nf-winuser-vkkeyscanexw
    //// ... high-order byte contains the shift state,
    //// which can be a combination of the following flag bits.
    //// 1 Either SHIFT key is pressed.
    //// 2 Either CTRL key is pressed.
    //// 4 Either ALT key is pressed.
    // SHORT scan_res = ::VkKeyScanExW(vk, current_layout);
    // constexpr auto ctrlAlt = (2 | 4);
    // if (((scan_res >> 8) & ctrlAlt) == ctrlAlt) { // ctrl-alt pressed
    //   // ctrl-alt pressed
    //   cm &= ~(EVENTFLAG_CONTROL_DOWN | EVENTFLAG_ALT_DOWN);
    //   cm |= EVENTFLAG_ALTGR_DOWN;
    // }
  }

  return cm;
}

static void
winQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce)
{
  ce.windows_key_code = qe->nativeVirtualKey();
  ce.modifiers = winGetKeyboardModifiers(qe);

  if (ce.modifiers & EVENTFLAG_ALT_DOWN)
    ce.is_system_key = true;
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

static uint32_t
linuxGetKeyboardModifiers(QKeyEvent* qe)
{
  auto cm = 0;
  auto m = qe->modifiers();
  cm |= m & Qt::ShiftModifier ? EVENTFLAG_SHIFT_DOWN : 0;
  cm |= m & Qt::AltModifier ? EVENTFLAG_ALT_DOWN : 0;
  cm |= m & Qt::KeypadModifier ? EVENTFLAG_IS_KEY_PAD : 0;
  cm |= m & Qt::ControlModifier ? EVENTFLAG_CONTROL_DOWN : 0;

  auto state = qe->nativeModifiers();
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

  return cm;
}

static quint32
linuxGetWindowsKeyCode(QKeyEvent* ke)
{
  static QMap<int, qint32> windowsKeyMap = {
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

  if (windowsKeyMap.contains(ke->key()))
    return windowsKeyMap[ke->key()];

  return 0;
}

static void
linuxQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce)
{
  ce.windows_key_code = linuxGetWindowsKeyCode(qe);
  ce.modifiers = linuxGetKeyboardModifiers(qe);

  if (ce.modifiers & EVENTFLAG_ALT_DOWN)
    ce.is_system_key = true;
}

#else

#endif

void
MapQKeyEventToCefKeyEvent(QKeyEvent* qe, CefKeyEvent& ce)
{
#if defined(Q_OS_MACOS)
  macQKeyEventToCefKeyEvent(qe, ce);
#elif defined(Q_OS_WINDOWS) || defined(Q_OS_WIN)
  winQKeyEventToCefKeyEvent(qe, ce);
#elif defined(Q_OS_LINUX)
  linuxQKeyEventToCefKeyEvent(qe, ce);
#endif
}

bool
ShouldSendKeyCharEvent(QKeyEvent* qe)
{
#if defined(Q_OS_MACOS)
  return macShouldSendKeyCharEvent(qe);
#else
  return !qe->text().isEmpty();
#endif
}

void
AdjustCefKeyCharEvent(QKeyEvent* qe, CefKeyEvent& ce)
{
#if defined(Q_OS_MACOS)
  return;
#else
  if (!qe->text().isEmpty()) {
    ce.windows_key_code = qe->text().at(0).unicode();
    ce.unmodified_character = qe->key();
    ce.character = ce.windows_key_code;
  }
#endif
}
