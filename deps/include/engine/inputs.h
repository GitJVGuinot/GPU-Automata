#ifndef __INPUTS_H__
#define __INPUTS_H__ 1

namespace Inputs
{
  /**
   * @enum MouseButton
   *
   * @brief Enumeration that represents the buttons of a mouse.
   *
   * The MouseButton enumeration defines identifiers for the different
   * buttons on a mouse, providing a clear mapping between buttons and
   * numerical values.
   */
  enum class MouseButton : int
  {
    Mouse_Button_1 = 0,                   ///< Mouse button 1.
    Mouse_Button_2 = 1,                   ///< Mouse button 2.
    Mouse_Button_3 = 2,                   ///< Mouse button 3.
    Mouse_Button_4 = 3,                   ///< Mouse button 4.
    Mouse_Button_5 = 4,                   ///< Mouse button 5.
    Mouse_Button_6 = 5,                   ///< Mouse button 6.
    Mouse_Button_7 = 6,                   ///< Mouse button 7.
    Mouse_Button_8 = 7,                   ///< Mouse button 8.
    Mouse_Button_Last = Mouse_Button_8,   ///< Last mouse button.
    Mouse_Button_Left = Mouse_Button_1,   ///< Left mouse button.
    Mouse_Button_Right = Mouse_Button_2,  ///< Right mouse button.
    Mouse_Button_Middle = Mouse_Button_3, ///< Middle mouse button.
  };

  /**
   * @enum Key
   *
   * @brief Enumeration that represents keyboard keys.
   *
   * The Key enumeration defines identifiers for different keyboard keys,
   * providing a clear mapping between keys and numerical values.
   */
  enum class Key : int
  {
    Key_Unknown = -1,        ///< Unknown key.
    Key_Space = 32,          ///< Space key.
    Key_Apostrophe = 39,     ///< Apostrophe key (').
    Key_Comma = 44,          ///< Comma key (,).
    Key_Minus = 45,          ///< Minus key (-).
    Key_Period = 46,         ///< Period key (.).
    Key_Slash = 47,          ///< Slash key (/).
    Key_0 = 48,              ///< 0 key.
    Key_1 = 49,              ///< 1 key.
    Key_2 = 50,              ///< 2 key.
    Key_3 = 51,              ///< 3 key.
    Key_4 = 52,              ///< 4 key.
    Key_5 = 53,              ///< 5 key.
    Key_6 = 54,              ///< 6 key.
    Key_7 = 55,              ///< 7 key.
    Key_8 = 56,              ///< 8 key.
    Key_9 = 57,              ///< 9 key.
    Key_Semicolon = 59,      ///< Semicolon key (;).
    Key_Equal = 61,          ///< Equal key (=).
    Key_A = 65,              ///< A key.
    Key_B = 66,              ///< B key.
    Key_C = 67,              ///< C key.
    Key_D = 68,              ///< D key.
    Key_E = 69,              ///< E key.
    Key_F = 70,              ///< F key.
    Key_G = 71,              ///< G key.
    Key_H = 72,              ///< H key.
    Key_I = 73,              ///< I key.
    Key_J = 74,              ///< J key.
    Key_K = 75,              ///< K key.
    Key_L = 76,              ///< L key.
    Key_M = 77,              ///< M key.
    Key_N = 78,              ///< N key.
    Key_O = 79,              ///< O key.
    Key_P = 80,              ///< P key.
    Key_Q = 81,              ///< Q key.
    Key_R = 82,              ///< R key.
    Key_S = 83,              ///< S key.
    Key_T = 84,              ///< T key.
    Key_U = 85,              ///< U key.
    Key_V = 86,              ///< V key.
    Key_W = 87,              ///< W key.
    Key_X = 88,              ///< X key.
    Key_Y = 89,              ///< Y key.
    Key_Z = 90,              ///< Z key.
    Key_Left_Bracket = 91,   ///< Left Bracket key ([).
    Key_Blackslash = 92,     ///< Backslash key (\).
    Key_Right_Bracket = 93,  ///< Right Bracket key (]).
    Key_Grave_Accent = 96,   ///< Grave Accent key (`).
    Key_World_1 = 161,       ///< World 1 key (Non used).
    Key_World_2 = 162,       ///< World 2 key (Non used).
    Key_Escape = 256,        ///< Escape key.
    Key_Enter = 257,         ///< Enter key.
    Key_Tab = 258,           ///< Tab key.
    Key_Backspace = 259,     ///< Backspace key.
    Key_Insert = 260,        ///< Insert key.
    Key_Delete = 261,        ///< Delete key.
    Key_Right = 262,         ///< Right arrow key.
    Key_Left = 263,          ///< Left arrow key.
    Key_Down = 264,          ///< Down arrow key.
    Key_Up = 265,            ///< Up arrow key.
    Key_Page_Up = 266,       ///< Page Up key.
    Key_Page_Down = 267,     ///< Page Down key.
    Key_Home = 268,          ///< Home key.
    Key_End = 269,           ///< End key.
    Key_Caps_Lock = 280,     ///< Caps Lock key.
    Key_Scroll_Lock = 281,   ///< Scroll Lock key.
    Key_Num_Lock = 282,      ///< Num Lock key.
    Key_Print_Screen = 283,  ///< Print Screen key.
    Key_Pause = 284,         ///< Pause key.
    Key_F1 = 290,            ///< F1 key.
    Key_F2 = 291,            ///< F2 key.
    Key_F3 = 292,            ///< F3 key.
    Key_F4 = 293,            ///< F4 key.
    Key_F5 = 294,            ///< F5 key.
    Key_F6 = 295,            ///< F6 key.
    Key_F7 = 296,            ///< F7 key.
    Key_F8 = 297,            ///< F8 key.
    Key_F9 = 298,            ///< F9 key.
    Key_F10 = 299,           ///< F10 key.
    Key_F11 = 300,           ///< F11 key.
    Key_F12 = 301,           ///< F12 key.
    Key_F13 = 302,           ///< F13 key.
    Key_F14 = 303,           ///< F14 key.
    Key_F15 = 304,           ///< F15 key.
    Key_F16 = 305,           ///< F16 key.
    Key_F17 = 306,           ///< F17 key.
    Key_F18 = 307,           ///< F18 key.
    Key_F19 = 308,           ///< F19 key.
    Key_F20 = 309,           ///< F20 key.
    Key_F21 = 310,           ///< F21 key.
    Key_F22 = 311,           ///< F22 key.
    Key_F23 = 312,           ///< F23 key.
    Key_F24 = 313,           ///< F24 key.
    Key_F25 = 314,           ///< F25 key.
    Key_KP_0 = 320,          ///< Keypad 0 key.
    Key_KP_1 = 321,          ///< Keypad 1 key.
    Key_KP_2 = 322,          ///< Keypad 2 key.
    Key_KP_3 = 323,          ///< Keypad 3 key.
    Key_KP_4 = 324,          ///< Keypad 4 key.
    Key_KP_5 = 325,          ///< Keypad 5 key.
    Key_KP_6 = 326,          ///< Keypad 6 key.
    Key_KP_7 = 327,          ///< Keypad 7 key.
    Key_KP_8 = 328,          ///< Keypad 8 key.
    Key_KP_9 = 329,          ///< Keypad 9 key.
    Key_KP_Decimal = 330,    ///< Keypad Decimal key.
    Key_KP_Divide = 331,     ///< Keypad Divide key.
    Key_KP_Multiply = 332,   ///< Keypad Multiply key.
    Key_KP_Subtract = 333,   ///< Keypad Subtract key.
    Key_KP_Add = 334,        ///< Keypad Add key.
    Key_KP_Enter = 335,      ///< Keypad Enter key.
    Key_KP_Equal = 336,      ///< Keypad Equal key.
    Key_Left_Shift = 340,    ///< Left Shift key.
    Key_Left_Control = 341,  ///< Left Control key.
    Key_Left_Alt = 342,      ///< Left Alt key.
    Key_Left_Super = 343,    ///< Left Super key.
    Key_Right_Shift = 344,   ///< Right Shift key.
    Key_Right_Control = 345, ///< Right Control key.
    Key_Right_Alt = 346,     ///< Right Alt key.
    Key_Right_Super = 347,   ///< Right Super key.
    Key_Menu = 348,          ///< Menu key.
    Key_Last = Key_Menu,     ///< Last keyboard key.
  };
}
#endif /* __INPUTS_H__ */