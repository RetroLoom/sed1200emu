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


#define uartDebug 0

void UART_Init(void);
void UART_SendChar(char c);
char UART_ReceiveChar(void);
void UART_NextRow (void);
void UART_SendStr(const char* format, ...);
void UART_SendHex(uint8_t value);
void UART_GotoXY(uint8_t row, uint8_t col);
void UART_GotoY(uint8_t col);
void UART_ClearScreen();
void UART_ClearRow();
void UART_ShowCursor();
void UART_HideCursor();