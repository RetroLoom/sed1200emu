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

#define E	PD7
#define RS	PD6

#define LCD_ROWS  2
#define LCD_COLS  20

extern volatile uint8_t LCDBuffer [2][20];

extern volatile uint8_t SEDBuffer [2][20];
extern volatile uint8_t SEDRow, SEDCol;

extern volatile uint8_t queueHead, queueTail;

#define DISP_ON                         0b00001100      //LCD control constants
#define DISP_OFF                        0b00001000      //
#define CLR_DISP                        0b00000001      //
#define CUR_HOME                        0b00000010      //

#define CGRAM0_SET                      0x40
#define CGRAM1_SET                      0x48

#define CGRAM_ON                        0x04
#define CGRAM_OFF                       0x00

#define CGRAM0                          0x00
#define CGRAM1                          0x01

#define LCD_SET_CURSOR 0x80     // Command to set cursor position

void LCD_Init ();
void LCD_SendCmd (uint8_t c);
void LCD_SendChar (uint8_t c);
void LCD_SendCharAt (uint8_t row, uint8_t col, uint8_t c);

uint8_t LCD_Dequeue();
void LCD_Enqueue(uint8_t c);

void LCD_GotoXY (uint8_t row, uint8_t col);
void LCD_Home ();

void LCD_SendStr(const char *format, ...);
void LCD_ScrollStr(const char *text, uint8_t row, uint16_t delay_ms, uint8_t length);

void updateDisplay();