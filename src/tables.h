/*
 * This file is part of the SED1200EMU Project.
 *
 * SED1200EMU is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

// ASCII Character Table
#define ASCII_SPACE                     0x20    // " "
#define ASCII_EXCLAMATION_MARK          0x21    // "!"
#define ASCII_QUOTATION_MARK            0x22    // "\""
#define ASCII_NUMBER_SIGN               0x23    // "#"
#define ASCII_DOLLAR_SIGN               0x24    // "$"
#define ASCII_PERCENT_SIGN              0x25    // "%"
#define ASCII_AMPERSAND                 0x26    // "&"
#define ASCII_APOSTROPHE                0x27    // "'"
#define ASCII_LEFT_PARENTHESIS          0x28    // "("
#define ASCII_RIGHT_PARENTHESIS         0x29    // ")"
#define ASCII_ASTERISK                  0x2A    // "*"
#define ASCII_PLUS                      0x2B    // "+"
#define ASCII_COMMA                     0x2C    // ","
#define ASCII_HYPHEN_MINUS              0x2D    // "-"
#define ASCII_PERIOD                    0x2E    // "."
#define ASCII_SLASH                     0x2F    // "/"

#define ASCII_0                         0x30    // "0"
#define ASCII_1                         0x31    // "1"
#define ASCII_2                         0x32    // "2"
#define ASCII_3                         0x33    // "3"
#define ASCII_4                         0x34    // "4"
#define ASCII_5                         0x35    // "5"
#define ASCII_6                         0x36    // "6"
#define ASCII_7                         0x37    // "7"
#define ASCII_8                         0x38    // "8"
#define ASCII_9                         0x39    // "9"

#define ASCII_COLON                     0x3A    // ":"
#define ASCII_SEMICOLON                 0x3B    // ";"
#define ASCII_LESS_THAN                 0x3C    // "<"
#define ASCII_EQUAL_SIGN                0x3D    // "="
#define ASCII_GREATER_THAN              0x3E    // ">"
#define ASCII_QUESTION_MARK             0x3F    // "?"

#define ASCII_AT                        0x40    // "@"
#define ASCII_A                         0x41    // "A"
#define ASCII_B                         0x42    // "B"
#define ASCII_C                         0x43    // "C"
#define ASCII_D                         0x44    // "D"
#define ASCII_E                         0x45    // "E"
#define ASCII_F                         0x46    // "F"
#define ASCII_G                         0x47    // "G"
#define ASCII_H                         0x48    // "H"
#define ASCII_I                         0x49    // "I"
#define ASCII_J                         0x4A    // "J"
#define ASCII_K                         0x4B    // "K"
#define ASCII_L                         0x4C    // "L"
#define ASCII_M                         0x4D    // "M"
#define ASCII_N                         0x4E    // "N"
#define ASCII_O                         0x4F    // "O"
#define ASCII_P                         0x50    // "P"
#define ASCII_Q                         0x51    // "Q"
#define ASCII_R                         0x52    // "R"
#define ASCII_S                         0x53    // "S"
#define ASCII_T                         0x54    // "T"
#define ASCII_U                         0x55    // "U"
#define ASCII_V                         0x56    // "V"
#define ASCII_W                         0x57    // "W"
#define ASCII_X                         0x58    // "X"
#define ASCII_Y                         0x59    // "Y"
#define ASCII_Z                         0x5A    // "Z"

#define ASCII_LEFT_BRACKET              0x5B    // "["
#define ASCII_BACKSLASH                 0x5C    // "\\"
#define ASCII_RIGHT_BRACKET             0x5D    // "]"
#define ASCII_CARET                     0x5E    // "^"
#define ASCII_UNDERSCORE                0x5F    // "_"
#define ASCII_GRAVE_ACCENT              0x60    // "`"

#define ASCII_a                         0x61    // "a"
#define ASCII_b                         0x62    // "b"
#define ASCII_c                         0x63    // "c"
#define ASCII_d                         0x64    // "d"
#define ASCII_e                         0x65    // "e"
#define ASCII_f                         0x66    // "f"
#define ASCII_g                         0x67    // "g"
#define ASCII_h                         0x68    // "h"
#define ASCII_i                         0x69    // "i"
#define ASCII_j                         0x6A    // "j"
#define ASCII_k                         0x6B    // "k"
#define ASCII_l                         0x6C    // "l"
#define ASCII_m                         0x6D    // "m"
#define ASCII_n                         0x6E    // "n"
#define ASCII_o                         0x6F    // "o"
#define ASCII_p                         0x70    // "p"
#define ASCII_q                         0x71    // "q"
#define ASCII_r                         0x72    // "r"
#define ASCII_s                         0x73    // "s"
#define ASCII_t                         0x74    // "t"
#define ASCII_u                         0x75    // "u"
#define ASCII_v                         0x76    // "v"
#define ASCII_w                         0x77    // "w"
#define ASCII_x                         0x78    // "x"
#define ASCII_y                         0x79    // "y"
#define ASCII_z                         0x7A    // "z"

#define ASCII_LEFT_CURLY_BRACE          0x7B    // "{"
#define ASCII_VERTICAL_BAR              0x7C    // "|"
#define ASCII_RIGHT_CURLY_BRACE         0x7D    // "}"
#define ASCII_TILDE                     0x7E    // "~"


// HD44780 Extended Character Table (Non-ASCII Characters)

// Japanese Katakana Characters
#define HD44780_KANA_A                  0xA0    // Katakana "A"
#define HD44780_KANA_I                  0xA1    // Katakana "I"
#define HD44780_KANA_U                  0xA2    // Katakana "U"
#define HD44780_KANA_E                  0xA3    // Katakana "E"
#define HD44780_KANA_O                  0xA4    // Katakana "O"
#define HD44780_KANA_YA                 0xA5    // Katakana "Ya"
#define HD44780_KANA_YU                 0xA6    // Katakana "Yu"
#define HD44780_KANA_YO                 0xA7    // Katakana "Yo"
#define HD44780_KANA_TSU                0xA8    // Katakana "Tsu"
#define HD44780_KANA_WO                 0xA9    // Katakana "Wo"
#define HD44780_KANA_N                  0xAA    // Katakana "N"

// Special Symbols
#define HD44780_DEGREE_SIGN             0xDF    // ° Degree symbol
#define HD44780_ARROW_RIGHT             0x7E    // → Right arrow
#define HD44780_ARROW_LEFT              0x7F    // ← Left arrow

// Box-Drawing Characters (Depending on ROM Code)
#define HD44780_FULL_BLOCK              0xFF    // █ Full block (0xFF is often used for a solid block)
#define HD44780_INVERTED_BLOCK          0xDB    // ▀ Half block (inverted or lower half block)

// Greek Letters (if supported by ROM)
#define HD44780_ALPHA                   0xE0    // α Greek alpha (only in certain ROM sets)
#define HD44780_BETA                    0xE2    // β Greek beta (only in certain ROM sets)
#define HD44780_SIGMA                   0xE4    // Σ Greek sigma (only in certain ROM sets)
#define HD44780_OMEGA                   0xEA    // Ω Greek omega (only in certain ROM sets)

// Currency Symbols
#define HD44780_YEN_SIGN                0x5C    // ¥ Yen sign (also commonly displayed as backslash)

// Miscellaneous
#define HD44780_ARROW_UP                0x5E    // ↑ Up arrow (appears as a caret)
#define HD44780_ARROW_DOWN              0x5F    // ↓ Down arrow (appears as underscore)


// HD44780 Command Table

// Basic Commands
#define HD44780_CLEAR_DISPLAY           0x01    // Clear the display and reset cursor to home position
#define HD44780_RETURN_HOME             0x02    // Return the cursor to home position (0,0)

// Entry Mode Set
#define HD44780_ENTRY_MODE_SET          0x04    // Set cursor move direction and enable display shift
#define HD44780_ENTRY_INCREMENT         0x06    // Increment cursor position after each character
#define HD44780_ENTRY_DECREMENT         0x04    // Decrement cursor position after each character
#define HD44780_ENTRY_SHIFT_ON          0x05    // Display shift on, cursor move
#define HD44780_ENTRY_SHIFT_OFF         0x04    // Display shift off, no cursor move

// Display Control Commands
#define HD44780_DISPLAY_OFF             0x08    // Turn off display
#define HD44780_DISPLAY_ON              0x0C    // Turn on display (cursor and blink off)
#define HD44780_DISPLAY_ON_CURSOR_ON    0x0E    // Display on, cursor on, no blink
#define HD44780_DISPLAY_ON_BLINK_ON     0x0D    // Display on, blinking cursor off
#define HD44780_DISPLAY_ON_CURSOR_BLINK 0x0F    // Display on, cursor on, and blink on

// Cursor or Display Shift
#define HD44780_CURSOR_SHIFT_LEFT       0x10    // Move cursor left without shifting display
#define HD44780_CURSOR_SHIFT_RIGHT      0x14    // Move cursor right without shifting display
#define HD44780_DISPLAY_SHIFT_LEFT      0x18    // Shift entire display left
#define HD44780_DISPLAY_SHIFT_RIGHT     0x1C    // Shift entire display right

// Function Set Commands
#define HD44780_FUNCTION_SET_4BIT_1LINE 0x20    // Set 4-bit mode, 1-line display, 5x8 dots
#define HD44780_FUNCTION_SET_4BIT_2LINE 0x28    // Set 4-bit mode, 2-line display, 5x8 dots
#define HD44780_FUNCTION_SET_8BIT_1LINE 0x30    // Set 8-bit mode, 1-line display, 5x8 dots
#define HD44780_FUNCTION_SET_8BIT_2LINE 0x38    // Set 8-bit mode, 2-line display, 5x8 dots

// Set DDRAM Address (Position Cursor)
#define HD44780_SET_DDRAM_ADDRESS       0x80    // Set DDRAM address for cursor position (0x80 + address)

// Set CGRAM Address (Define Custom Characters)
#define HD44780_SET_CGRAM_ADDRESS       0x40    // Set CGRAM address for custom character (0x40 + address)


// SED1200 Extended Character Table (Non-ASCII Characters)

// Box-Drawing Characters
#define SED_FULL_BLOCK                  0xDB    // █ Full block
#define SED_HALF_BLOCK                  0xDC    // ▄ Lower half block
#define SED_LEFT_HALF_BLOCK             0xDD    // ▌ Left half block
#define SED_RIGHT_HALF_BLOCK            0xDE    // ▐ Right half block

// Mathematical Symbols
#define SED_DIVISION_SIGN               0xF7    // ÷ Division sign
#define SED_DEGREE_SIGN                 0xDF    // ° Degree symbol

// Arrows
#define SED_ARROW_UP                    0xA1    // ↑ Up arrow
#define SED_ARROW_DOWN                  0xA2    // ↓ Down arrow
#define SED_ARROW_LEFT                  0xA3    // ← Left arrow
#define SED_ARROW_RIGHT                 0xA4    // → Right arrow

// Greek Letters (Commonly Available)
#define SED_ALPHA                       0xE0    // α Greek alpha
#define SED_BETA                        0xE2    // β Greek beta
#define SED_GAMMA                       0xE3    // γ Greek gamma
#define SED_OMEGA                       0xEA    // Ω Greek omega

// Special Symbols
#define SED_PI                          0xE4    // π Greek pi
#define SED_MICRO                       0xE5    // μ Greek mu (often used as "micro" in electronics)
#define SED_THETA                       0xF2    // θ Greek theta
#define SED_SIGMA                       0xE6    // Σ Greek sigma (sum)

#define SED_JAPANESE_KANA_A             0xB1    // Japanese Katakana A
#define SED_JAPANESE_KANA_I             0xB2    // Japanese Katakana I
#define SED_JAPANESE_KANA_U             0xB3    // Japanese Katakana U
#define SED_JAPANESE_KANA_E             0xB4    // Japanese Katakana E
#define SED_JAPANESE_KANA_O             0xB5    // Japanese Katakana O

// Fractions
#define SED_ONE_QUARTER                 0xB9    // ¼ One-quarter
#define SED_ONE_HALF                    0xBA    // ½ One-half
#define SED_THREE_QUARTERS              0xBB    // ¾ Three-quarters

// Currency Symbols
#define SED_YEN_SIGN                    0xA5    // ¥ Yen sign
#define SED_POUND_SIGN                  0xA3    // £ Pound sign

// Miscellaneous
#define SED_CELSIUS                     0xDF    // °C Celsius symbol
#define SED_SMILING_FACE                0x01    // ☺ Smiling face
#define SED_HEART                       0x03    // ♥ Heart
#define SED_DIAMOND                     0x04    // ♦ Diamond
#define SED_CLUB                        0x05    // ♣ Club
#define SED_SPADE                       0x06    // ♠ Spade


// SED1200 Command Table

// Basic Commands
#define SED1200_CLEAR_DISPLAY           0x01    // Clear the display and reset cursor to home position
#define SED1200_RETURN_HOME             0x02    // Return the cursor to home position (0,0)
#define SED1200_ENTRY_MODE_SET          0x04    // Set cursor movement direction and display shift

// Display Control Commands
#define SED1200_DISPLAY_OFF             0x08    // Turn the display off
#define SED1200_DISPLAY_ON              0x0C    // Turn the display on (cursor and blink off)
#define SED1200_DISPLAY_ON_CURSOR_ON    0x0E    // Turn display on with cursor on
#define SED1200_DISPLAY_ON_BLINK_ON     0x0D    // Turn display on with blinking cursor
#define SED1200_DISPLAY_ON_CURSOR_BLINK 0x0F    // Turn display on with cursor on and blinking

// Cursor and Display Shift
#define SED1200_CURSOR_SHIFT_LEFT       0x10    // Move the cursor left without changing display
#define SED1200_CURSOR_SHIFT_RIGHT      0x14    // Move the cursor right without changing display
#define SED1200_DISPLAY_SHIFT_LEFT      0x18    // Shift entire display to the left
#define SED1200_DISPLAY_SHIFT_RIGHT     0x1C    // Shift entire display to the right

// Function Set Commands
#define SED1200_FUNCTION_SET_8BIT       0x30    // Set 8-bit mode, 1-line display, 5x7 dots
#define SED1200_FUNCTION_SET_8BIT_2LINE 0x38    // Set 8-bit mode, 2-line display, 5x7 dots

// Set DDRAM Address (Position Cursor)
#define SED1200_SET_DDRAM_ADDRESS       0x80    // Set DDRAM address, add column offset (0x80 + address)

// Set CGRAM Address (Define Custom Characters)
#define SED1200_SET_CGRAM_ADDRESS       0x40    // Set CGRAM address, add offset (0x40 + address)
