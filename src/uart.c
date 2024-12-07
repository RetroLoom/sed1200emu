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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "atmega.h"
#include "uart.h"
#include "tables.h"

#define F_CPU 8000000UL
#define BAUD 38400        // Desired baud rate
#define MYUBRR (F_CPU/16/BAUD-1)  // UBRR value for baud rate

void UART_Init(void) {
    // Set baud rate
    uint16_t ubrr_value = MYUBRR;
    UBRRH = (uint8_t)(ubrr_value >> 8);  // Upper 8 bits of UBRR
    UBRRL = (uint8_t)ubrr_value;        // Lower 8 bits of UBRR

    // Enable transmitter and receiver
    UCSRB = (1 << TXEN) | (1 << RXEN);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

	_delay_us (100);
	UART_ClearScreen();
	UART_GotoXY(0,0);
	UART_HideCursor();

	if (uartDebug)
	{
		UART_SendStr("UART Initialized");
		UART_NextRow();
	}
}

void UART_SendChar(char c) {
    // Wait for the transmit buffer to be empty
    while (!(UCSRA & (1 << UDRE))) {
        
    }

    switch (c)
    {
    case 0x01:
        c = ASCII_SPACE;
        break;
    default:
        break;
    }
    
    UDR = c;
}

char UART_ReceiveChar(void) {
    // Wait for data to be received
    while (!(UCSRA & (1 << RXC))) {
        // Do nothing
    }
    // Get and return received data from the buffer
    return UDR;
}

void UART_NextRow (void) {
	UART_SendChar('\r');
	UART_SendChar('\n');
}

void UART_SendStr(const char* format, ...) {
	char text[64] = "";
	va_list args;
	va_start ( args, format );
	vsprintf ( text, format, args );

	uint8_t i;
	for (i = 0; i < strlen(text); i++)
		UART_SendChar(text[i]);
}

void UART_SendHex(uint8_t value) {
    const char hexDigits[] = "0123456789ABCDEF";

    // Convert high nibble to ASCII and send
    UART_SendChar(hexDigits[(value >> 4) & 0x0F]);  // High nibble
    // Convert low nibble to ASCII and send
    UART_SendChar(hexDigits[value & 0x0F]);         // Low nibble
}

void UART_GotoXY(uint8_t row, uint8_t col) {
    row++;
    col++;
    UART_SendChar(0x1B);           // ESC character
    UART_SendChar('[');            // Start of command
    UART_SendChar('0' + (row / 10)); // Tens place of row
    UART_SendChar('0' + (row % 10)); // Units place of row
    UART_SendChar(';');            // Separator
    UART_SendChar('0' + (col / 10)); // Tens place of col
    UART_SendChar('0' + (col % 10)); // Units place of col
    UART_SendChar('H');            // Command terminator
}

void UART_GotoY(uint8_t col) {
    col++;
    UART_SendChar(0x1B);           // ESC character
    UART_SendChar('[');            // Start of command
    UART_SendChar('0' + (col / 10)); // Tens place of col
    UART_SendChar('0' + (col % 10)); // Units place of col
    UART_SendChar('G');            // Command terminator
}

void UART_ClearScreen() {
    UART_SendChar(0x1B);  // ESC
    UART_SendChar('[');
    UART_SendChar('2');
    UART_SendChar('J');   // Clear screen command
}

void UART_ClearRow() {
    UART_SendChar(0x1B);  // ESC character
    UART_SendChar('[');   // Start of command
    UART_SendChar('2');   // Clear entire line
    UART_SendChar('K');   // Command terminator
}

void UART_ShowCursor() {
    UART_SendChar(0x1B);  // ESC
    UART_SendChar('[');
    UART_SendChar('?');
    UART_SendChar('2');
    UART_SendChar('5');
    UART_SendChar('h');   // Command to show the cursor
}

void UART_HideCursor() {
    UART_SendChar(0x1B);  // ESC
    UART_SendChar('[');
    UART_SendChar('?');
    UART_SendChar('2');
    UART_SendChar('5');
    UART_SendChar('l');   // Command to hide the cursor
}

void UART_EnableCursorBlink() {
    UART_SendChar(0x1B);  // ESC
    UART_SendChar('[');
    UART_SendChar('?');
    UART_SendChar('1');
    UART_SendChar('2');
    UART_SendChar('h');   // Enable cursor blink
}

void UART_DisableCursorBlink() {
    UART_SendChar(0x1B);  // ESC
    UART_SendChar('[');
    UART_SendChar('?');
    UART_SendChar('1');
    UART_SendChar('2');
    UART_SendChar('l');   // Disable cursor blink
}