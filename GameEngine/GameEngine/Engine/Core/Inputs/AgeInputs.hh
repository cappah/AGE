#pragma once

#include <map>
#include <cstdint>

#define AGE_KEY_PRESSED(k)				(((k) & 0x1) == 0x1)
#define AGE_KEY_JUST_PRESSED(k)			(((k) & 0x2) == 0x2)
#define AGE_KEY_JUST_RELEASED(k)		(((k) & 0x4) == 0x4)

#define AGE_SET_KEY_PRESSED(k)			((k) | 0x1)
#define AGE_SET_KEY_JUST_PRESSED(k)		((k) | 0x2)
#define AGE_SET_KEY_JUST_RELEASED(k)	((k) | 0x4)

#define AGE_UNSET_KEY_JUST_PRESSED(k)	(((k) | 0x2) ^ 0x2)
#define AGE_UNSET_KEY_JUST_RELEASED(k)	(((k) | 0x4) ^ 0x4)

#define AGE_RESET_MAPPED_KEY_STATE(k)	(((k) | 0xF) ^ 0xF)

#define AGE_PHYSICAL_KEY_PRESSED(k)				(((k) & 0x10) == 0x10)
#define AGE_PHYSICAL_KEY_JUST_PRESSED(k)		(((k) & 0x20) == 0x20)
#define AGE_PHYSICAL_KEY_JUST_RELEASED(k)		(((k) & 0x40) == 0x40)

#define AGE_SET_PHYSICAL_KEY_PRESSED(k)			((k) | 0x10)
#define AGE_SET_PHYSICAL_KEY_JUST_PRESSED(k)	((k) | 0x20)
#define AGE_SET_PHYSICAL_KEY_JUST_RELEASED(k)	((k) | 0x40)

#define AGE_UNSET_PHYSICAL_KEY_JUST_PRESSED(k)	(((k) | 0x20) ^ 0x20)
#define AGE_UNSET_PHYSICAL_KEY_JUST_RELEASED(k)	(((k) | 0x40) ^ 0x40)

#define AGE_RESET_PHYSICAL_KEY_STATE(k)				(((k) | 0xF0) ^ 0xF0)

namespace AGE
{
	// --- KEYBOARD --- \\

	// Enum for the keyboard keys
	enum AgeKeys
	{
		AGE_KEY_UNKNOWN = 0,
		AGE_RETURN,
		AGE_ESCAPE,
		AGE_BACKSPACE,
		AGE_TAB,
		AGE_SPACE,
		AGE_EXCLAIM,
		AGE_QUOTEDBL,
		AGE_HASH,
		AGE_PERCENT,
		AGE_DOLLAR,
		AGE_AMPERSAND,
		AGE_QUOTE,
		AGE_LEFTPAREN,
		AGE_RIGHTPAREN,
		AGE_ASTERISK,
		AGE_PLUS,
		AGE_COMMA,
		AGE_MINUS,
		AGE_PERIOD,
		AGE_SLASH,
		AGE_0,
		AGE_1,
		AGE_2,
		AGE_3,
		AGE_4,
		AGE_5,
		AGE_6,
		AGE_7,
		AGE_8,
		AGE_9,
		AGE_COLON,
		AGE_SEMICOLON,
		AGE_LESS,
		AGE_EQUALS,
		AGE_GREATER,
		AGE_QUESTION,
		AGE_AT,
		AGE_LEFTBRACKET,
		AGE_BACKSLASH,
		AGE_RIGHTBRACKET,
		AGE_CARET,
		AGE_UNDERSCORE,
		AGE_BACKQUOTE,
		AGE_a,
		AGE_b,
		AGE_c,
		AGE_d,
		AGE_e,
		AGE_f,
		AGE_g,
		AGE_h,
		AGE_i,
		AGE_j,
		AGE_k,
		AGE_l,
		AGE_m,
		AGE_n,
		AGE_o,
		AGE_p,
		AGE_q,
		AGE_r,
		AGE_s,
		AGE_t,
		AGE_u,
		AGE_v,
		AGE_w,
		AGE_x,
		AGE_y,
		AGE_z,
		AGE_KEYPAD_DIVIDE,
		AGE_KEYPAD_MULTIPLY,
		AGE_KEYPAD_MINUS,
		AGE_KEYPAD_PLUS,
		AGE_KEYPAD_ENTER,
		AGE_KEYPAD_1,
		AGE_KEYPAD_2,
		AGE_KEYPAD_3,
		AGE_KEYPAD_4,
		AGE_KEYPAD_5,
		AGE_KEYPAD_6,
		AGE_KEYPAD_7,
		AGE_KEYPAD_8,
		AGE_KEYPAD_9,
		AGE_KEYPAD_0,
		AGE_KEYPAD_PERIOD,
		AGE_KEYPAD_EQUALS,
		AGE_CAPSLOCK,
		AGE_F1,
		AGE_F2,
		AGE_F3,
		AGE_F4,
		AGE_F5,
		AGE_F6,
		AGE_F7,
		AGE_F8,
		AGE_F9,
		AGE_F10,
		AGE_F11,
		AGE_F12,
		AGE_PRINTSCREEN,
		AGE_SCROLLLOCK,
		AGE_PAUSE,
		AGE_INSERT,
		AGE_HOME,
		AGE_PAGEUP,
		AGE_DELETE,
		AGE_END,
		AGE_PAGEDOWN,
		AGE_RIGHT,
		AGE_LEFT,
		AGE_DOWN,
		AGE_UP,
		AGE_NUMLOCKCLEAR,
		AGE_APPLICATION,
		AGE_POWER,
		AGE_F13,
		AGE_F14,
		AGE_F15,
		AGE_F16,
		AGE_F17,
		AGE_F18,
		AGE_F19,
		AGE_F20,
		AGE_F21,
		AGE_F22,
		AGE_F23,
		AGE_F24,
		AGE_EXECUTE,
		AGE_HELP,
		AGE_MENU,
		AGE_SELECT,
		AGE_STOP,
		AGE_AGAIN,
		AGE_UNDO,
		AGE_CUT,
		AGE_COPY,
		AGE_PASTE,
		AGE_FIND,
		AGE_MUTE,
		AGE_VOLUMEUP,
		AGE_VOLUMEDOWN,
		AGE_ALTERASE,
		AGE_SYSREQ,
		AGE_CANCEL,
		AGE_CLEAR,
		AGE_PRIOR,
		AGE_RETURN2,
		AGE_SEPARATOR,
		AGE_OUT,
		AGE_OPER,
		AGE_CLEARAGAIN,
		AGE_CRSEL,
		AGE_EXSEL,
		AGE_LCTRL,
		AGE_LSHIFT,
		AGE_LALT,
		AGE_LGUI,
		AGE_RCTRL,
		AGE_RSHIFT,
		AGE_RALT,
		AGE_RGUI,
		AGE_MODE,
		AGE_AUDIONEXT,
		AGE_AUDIOPREV,
		AGE_AUDIOSTOP,
		AGE_AUDIOPLAY,
		AGE_AUDIOMUTE,
		AGE_MEDIASELECT,
		AGE_WWW,
		AGE_MAIL,
		AGE_CALCULATOR,
		AGE_COMPUTER,
		AGE_AC_SEARCH,
		AGE_AC_HOME,
		AGE_AC_BACK,
		AGE_AC_FORWARD,
		AGE_AC_STOP,
		AGE_AC_REFRESH,
		AGE_AC_BOOKMARKS,
		AGE_BRIGHTNESSDOWN,
		AGE_BRIGHTNESSUP,
		AGE_DISPLAYSWITCH,
		AGE_KBDILLUMTOGGLE,
		AGE_KBDILLUMDOWN,
		AGE_KBDILLUMUP,
		AGE_EJECT,
		AGE_SLEEP,
		AGE_KEY_NUMBER
	};

	// --- INPUTS --- \\

	// Other inputs
	enum AgeMouseButtons
	{
		AGE_MOUSE_UNKNOWN = 0,
		AGE_MOUSE_LEFT,
		AGE_MOUSE_MIDDLE,
		AGE_MOUSE_RIGHT,
		AGE_MOUSE_BUTTONS_NUMBER
	};

	enum AgeWindowInputs
	{
		AGE_WINDOW_UNKNOWN = 0,
		AGE_WINDOW_SHOWN,
		AGE_WINDOW_HIDDEN,
		AGE_WINDOW_EXPOSED,
		AGE_WINDOW_MOVED,
		AGE_WINDOW_RESIZED,
		AGE_WINDOW_MINIMIZED,
		AGE_WINDOW_MAXIMIZED,
		AGE_WINDOW_RESTORED,
		AGE_WINDOW_ENTER,
		AGE_WINDOW_LEAVE,
		AGE_WINDOW_FOCUS_GAINED,
		AGE_WINDOW_FOCUS_LOST,
		AGE_WINDOW_CLOSE,
		AGE_WINDOW_INPUT_NUMBER
	};

	// --- JOYSTICK --- \\

	static const uint32_t AGE_JOYSTICK_MAX_NUMBER = 8;
	static const uint32_t AGE_JOYSTICK_HAT_MAX_NUMBER = 4;
	static const uint32_t AGE_JOYSTICK_TRACKBALL_MAX_NUMBER = 4;

	// Joystick axis
	enum AgeJoystickAxis
	{
		AGE_JOYSTICK_AXIS_UNKNOWN = 0,
		AGE_JOYSTICK_AXIS_LEFTX,
		AGE_JOYSTICK_AXIS_LEFTY,
		AGE_JOYSTICK_AXIS_RIGHTX,
		AGE_JOYSTICK_AXIS_RIGHTY,
		AGE_JOYSTICK_AXIS_TRIGGERLEFT,
		AGE_JOYSTICK_AXIS_TRIGGERRIGHT,
		AGE_JOYSTICK_AXIS_NUMBER
	};

	// Joystick buttons
	enum AgeJoystickButtons
	{
		AGE_JOYSTICK_BUTTON_UNKNOWN = 0,
		AGE_JOYSTICK_BUTTON_A,
		AGE_JOYSTICK_BUTTON_B,
		AGE_JOYSTICK_BUTTON_X,
		AGE_JOYSTICK_BUTTON_Y,
		AGE_JOYSTICK_BUTTON_BACK,
		AGE_JOYSTICK_BUTTON_GUIDE,
		AGE_JOYSTICK_BUTTON_START,
		AGE_JOYSTICK_BUTTON_LEFTSTICK,
		AGE_JOYSTICK_BUTTON_RIGHTSTICK,
		AGE_JOYSTICK_BUTTON_LEFTSHOULDER,
		AGE_JOYSTICK_BUTTON_RIGHTSHOULDER,
		AGE_JOYSTICK_BUTTON_DPAD_UP,
		AGE_JOYSTICK_BUTTON_DPAD_DOWN,
		AGE_JOYSTICK_BUTTON_DPAD_LEFT,
		AGE_JOYSTICK_BUTTON_DPAD_RIGHT,
		AGE_JOYSTICK_BUTTON_NUMBER
	};

	enum AgeJoystickHatDirections
	{
		AGE_JOYSTICK_HAT_CENTERED	= 0,
		AGE_JOYSTICK_HAT_UP			= (1 << 0),
		AGE_JOYSTICK_HAT_RIGHT		= (1 << 1),
		AGE_JOYSTICK_HAT_DOWN		= (1 << 2),
		AGE_JOYSTICK_HAT_LEFT		= (1 << 3),
		AGE_JOYSTICK_HAT_LEFT_UP	= (AGE_JOYSTICK_HAT_LEFT | AGE_JOYSTICK_HAT_UP),
		AGE_JOYSTICK_HAT_LEFT_DOWN	= (AGE_JOYSTICK_HAT_LEFT | AGE_JOYSTICK_HAT_DOWN),
		AGE_JOYSTICK_HAT_RIGHT_UP	= (AGE_JOYSTICK_HAT_RIGHT | AGE_JOYSTICK_HAT_UP),
		AGE_JOYSTICK_HAT_RIGHT_DOWN = (AGE_JOYSTICK_HAT_RIGHT | AGE_JOYSTICK_HAT_DOWN),
	};

	// --- ASCII TABLE --- \\

	static const char UNDEFINED_CHARACTER = -1;

	// From key enum to ascii value
	static const std::map<AgeKeys, char> keyToAscii =
	{
		{ AGE_KEY_UNKNOWN, UNDEFINED_CHARACTER },
		{ AGE_RETURN, '\n' },
		{ AGE_ESCAPE, '\033' },
		{ AGE_BACKSPACE, '\b' },
		{ AGE_TAB, '\t' },
		{ AGE_SPACE, ' ' },
		{ AGE_EXCLAIM, '!' },
		{ AGE_QUOTEDBL, '"' },
		{ AGE_HASH, '#' },
		{ AGE_PERCENT, '%' },
		{ AGE_DOLLAR, '$' },
		{ AGE_AMPERSAND, '&' },
		{ AGE_QUOTE, '\'' },
		{ AGE_LEFTPAREN, '(' },
		{ AGE_RIGHTPAREN, ')' },
		{ AGE_ASTERISK, '*' },
		{ AGE_PLUS, '+' },
		{ AGE_COMMA, ',' },
		{ AGE_MINUS, '-' },
		{ AGE_PERIOD, '.' },
		{ AGE_SLASH, '/' },
		{ AGE_0, '0' },
		{ AGE_1, '1' },
		{ AGE_2, '2' },
		{ AGE_3, '3' },
		{ AGE_4, '4' },
		{ AGE_5, '5' },
		{ AGE_6, '6' },
		{ AGE_7, '7' },
		{ AGE_8, '8' },
		{ AGE_9, '9' },
		{ AGE_COLON, ':' },
		{ AGE_SEMICOLON, ';' },
		{ AGE_LESS, '<' },
		{ AGE_EQUALS, '=' },
		{ AGE_GREATER, '>' },
		{ AGE_QUESTION, '?' },
		{ AGE_AT, '@' },
		{ AGE_LEFTBRACKET, '[' },
		{ AGE_BACKSLASH, '\\' },
		{ AGE_RIGHTBRACKET, ']' },
		{ AGE_CARET, '^' },
		{ AGE_UNDERSCORE, '_' },
		{ AGE_BACKQUOTE, '`' },
		{ AGE_a, 'a' },
		{ AGE_b, 'b' },
		{ AGE_c, 'c' },
		{ AGE_d, 'd' },
		{ AGE_e, 'e' },
		{ AGE_f, 'f' },
		{ AGE_g, 'g' },
		{ AGE_h, 'h' },
		{ AGE_i, 'i' },
		{ AGE_j, 'j' },
		{ AGE_k, 'k' },
		{ AGE_l, 'l' },
		{ AGE_m, 'm' },
		{ AGE_n, 'n' },
		{ AGE_o, 'o' },
		{ AGE_p, 'p' },
		{ AGE_q, 'q' },
		{ AGE_r, 'r' },
		{ AGE_s, 's' },
		{ AGE_t, 't' },
		{ AGE_u, 'u' },
		{ AGE_v, 'v' },
		{ AGE_w, 'w' },
		{ AGE_x, 'x' },
		{ AGE_y, 'y' },
		{ AGE_z, 'z' },
		{ AGE_KEYPAD_DIVIDE, '/' },
		{ AGE_KEYPAD_MULTIPLY, '*' },
		{ AGE_KEYPAD_MINUS, '-' },
		{ AGE_KEYPAD_PLUS, '+' },
		{ AGE_KEYPAD_ENTER, '\n' },
		{ AGE_KEYPAD_1, '1' },
		{ AGE_KEYPAD_2, '2' },
		{ AGE_KEYPAD_3, '3' },
		{ AGE_KEYPAD_4, '4' },
		{ AGE_KEYPAD_5, '5' },
		{ AGE_KEYPAD_6, '6' },
		{ AGE_KEYPAD_7, '7' },
		{ AGE_KEYPAD_8, '8' },
		{ AGE_KEYPAD_9, '9' },
		{ AGE_KEYPAD_0, '0' },
		{ AGE_KEYPAD_PERIOD, '.' },
		{ AGE_KEYPAD_EQUALS, '=' },
		{ AGE_CAPSLOCK, UNDEFINED_CHARACTER },
		{ AGE_F1, UNDEFINED_CHARACTER },
		{ AGE_F2, UNDEFINED_CHARACTER },
		{ AGE_F3, UNDEFINED_CHARACTER },
		{ AGE_F4, UNDEFINED_CHARACTER },
		{ AGE_F5, UNDEFINED_CHARACTER },
		{ AGE_F6, UNDEFINED_CHARACTER },
		{ AGE_F7, UNDEFINED_CHARACTER },
		{ AGE_F8, UNDEFINED_CHARACTER },
		{ AGE_F9, UNDEFINED_CHARACTER },
		{ AGE_F10, UNDEFINED_CHARACTER },
		{ AGE_F11, UNDEFINED_CHARACTER },
		{ AGE_F12, UNDEFINED_CHARACTER },
		{ AGE_PRINTSCREEN, UNDEFINED_CHARACTER },
		{ AGE_SCROLLLOCK, UNDEFINED_CHARACTER },
		{ AGE_PAUSE, UNDEFINED_CHARACTER },
		{ AGE_INSERT, UNDEFINED_CHARACTER },
		{ AGE_HOME, UNDEFINED_CHARACTER },
		{ AGE_PAGEUP, UNDEFINED_CHARACTER },
		{ AGE_DELETE, '\177' },
		{ AGE_END, UNDEFINED_CHARACTER },
		{ AGE_PAGEDOWN, UNDEFINED_CHARACTER },
		{ AGE_RIGHT, UNDEFINED_CHARACTER },
		{ AGE_LEFT, UNDEFINED_CHARACTER },
		{ AGE_DOWN, UNDEFINED_CHARACTER },
		{ AGE_UP, UNDEFINED_CHARACTER },
		{ AGE_NUMLOCKCLEAR, UNDEFINED_CHARACTER },
		{ AGE_APPLICATION, UNDEFINED_CHARACTER },
		{ AGE_POWER, UNDEFINED_CHARACTER },
		{ AGE_F13, UNDEFINED_CHARACTER },
		{ AGE_F14, UNDEFINED_CHARACTER },
		{ AGE_F15, UNDEFINED_CHARACTER },
		{ AGE_F16, UNDEFINED_CHARACTER },
		{ AGE_F17, UNDEFINED_CHARACTER },
		{ AGE_F18, UNDEFINED_CHARACTER },
		{ AGE_F19, UNDEFINED_CHARACTER },
		{ AGE_F20, UNDEFINED_CHARACTER },
		{ AGE_F21, UNDEFINED_CHARACTER },
		{ AGE_F22, UNDEFINED_CHARACTER },
		{ AGE_F23, UNDEFINED_CHARACTER },
		{ AGE_F24, UNDEFINED_CHARACTER },
		{ AGE_EXECUTE, UNDEFINED_CHARACTER },
		{ AGE_HELP, UNDEFINED_CHARACTER },
		{ AGE_MENU, UNDEFINED_CHARACTER },
		{ AGE_SELECT, UNDEFINED_CHARACTER },
		{ AGE_STOP, UNDEFINED_CHARACTER },
		{ AGE_AGAIN, UNDEFINED_CHARACTER },
		{ AGE_UNDO, UNDEFINED_CHARACTER },
		{ AGE_CUT, UNDEFINED_CHARACTER },
		{ AGE_COPY, UNDEFINED_CHARACTER },
		{ AGE_PASTE, UNDEFINED_CHARACTER },
		{ AGE_FIND, UNDEFINED_CHARACTER },
		{ AGE_MUTE, UNDEFINED_CHARACTER },
		{ AGE_VOLUMEUP, UNDEFINED_CHARACTER },
		{ AGE_VOLUMEDOWN, UNDEFINED_CHARACTER },
		{ AGE_ALTERASE, UNDEFINED_CHARACTER },
		{ AGE_SYSREQ, UNDEFINED_CHARACTER },
		{ AGE_CANCEL, UNDEFINED_CHARACTER },
		{ AGE_CLEAR, UNDEFINED_CHARACTER },
		{ AGE_PRIOR, UNDEFINED_CHARACTER },
		{ AGE_RETURN2, UNDEFINED_CHARACTER },
		{ AGE_SEPARATOR, UNDEFINED_CHARACTER },
		{ AGE_OUT, UNDEFINED_CHARACTER },
		{ AGE_OPER, UNDEFINED_CHARACTER },
		{ AGE_CLEARAGAIN, UNDEFINED_CHARACTER },
		{ AGE_CRSEL, UNDEFINED_CHARACTER },
		{ AGE_EXSEL, UNDEFINED_CHARACTER },
		{ AGE_LCTRL, UNDEFINED_CHARACTER },
		{ AGE_LSHIFT, UNDEFINED_CHARACTER },
		{ AGE_LALT, UNDEFINED_CHARACTER },
		{ AGE_LGUI, UNDEFINED_CHARACTER },
		{ AGE_RCTRL, UNDEFINED_CHARACTER },
		{ AGE_RSHIFT, UNDEFINED_CHARACTER },
		{ AGE_RALT, UNDEFINED_CHARACTER },
		{ AGE_RGUI, UNDEFINED_CHARACTER },
		{ AGE_MODE, UNDEFINED_CHARACTER },
		{ AGE_AUDIONEXT, UNDEFINED_CHARACTER },
		{ AGE_AUDIOPREV, UNDEFINED_CHARACTER },
		{ AGE_AUDIOSTOP, UNDEFINED_CHARACTER },
		{ AGE_AUDIOPLAY, UNDEFINED_CHARACTER },
		{ AGE_AUDIOMUTE, UNDEFINED_CHARACTER },
		{ AGE_MEDIASELECT, UNDEFINED_CHARACTER },
		{ AGE_WWW, UNDEFINED_CHARACTER },
		{ AGE_MAIL, UNDEFINED_CHARACTER },
		{ AGE_CALCULATOR, UNDEFINED_CHARACTER },
		{ AGE_COMPUTER, UNDEFINED_CHARACTER },
		{ AGE_AC_SEARCH, UNDEFINED_CHARACTER },
		{ AGE_AC_HOME, UNDEFINED_CHARACTER },
		{ AGE_AC_BACK, UNDEFINED_CHARACTER },
		{ AGE_AC_FORWARD, UNDEFINED_CHARACTER },
		{ AGE_AC_STOP, UNDEFINED_CHARACTER },
		{ AGE_AC_REFRESH, UNDEFINED_CHARACTER },
		{ AGE_AC_BOOKMARKS, UNDEFINED_CHARACTER },
		{ AGE_BRIGHTNESSDOWN, UNDEFINED_CHARACTER },
		{ AGE_BRIGHTNESSUP, UNDEFINED_CHARACTER },
		{ AGE_DISPLAYSWITCH, UNDEFINED_CHARACTER },
		{ AGE_KBDILLUMTOGGLE, UNDEFINED_CHARACTER },
		{ AGE_KBDILLUMDOWN, UNDEFINED_CHARACTER },
		{ AGE_KBDILLUMUP, UNDEFINED_CHARACTER },
		{ AGE_EJECT, UNDEFINED_CHARACTER },
		{ AGE_SLEEP, UNDEFINED_CHARACTER },
	};
}