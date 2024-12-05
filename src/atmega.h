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

// Timer0 Configuration for Compare Match mode
#define OCR0    _SFR_IO8(0x3C)  // Compare Match register
#define TCCR0   _SFR_IO8(0x33)  // Timer/Counter Control register
//#define TIMSK   _SFR_IO8(0x39)  // Timer Interrupt Mask register
#define WGM01   3               // Bit 3 in TCCR0 for CTC mode
#define OCIE0   1               // Bit 1 in TIMSK to enable compare match interrupt

extern volatile uint32_t millis;

void ATMEGA_Init();
void UART_Init(void);
void UART_SendChar(char c);
void UART_SendStr(const char* format, ...);
void UART_SendHex(uint8_t value);
void Timer0_Init();
void Timer1_Init();
void Timer2_Init();