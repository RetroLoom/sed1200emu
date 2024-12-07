#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "atmega.h"
#include "lcd.h"
#include "tables.h"
#include "uart.h"

volatile uint8_t SEDBuffer [2][20];
volatile uint8_t SEDRow = 0, SEDCol = 0;
extern volatile uint8_t queueHead, queueTail;

int8_t bufferDisplay = 0;

void updateDisplay()
{
    // Draw buffer to display
	uint8_t row;
    for (row = 0; row < 2; row++)  // Iterate both rows
    {
        // Set the cursor to the start of the current row
        uint8_t cursorPos = (row == 0 ? 0x00 : 0x40);  // DDRAM addresses
        LCD_SendCmd(LCD_SET_CURSOR | cursorPos);
        
        // Draw each column in the row
		uint8_t col;
        for (col = 0; col < LCD_COLS; col++)
        {
            uint8_t c = SEDBuffer[row][col];
            //if (c == 0x00) {
            //	c = ASCII_SPACE;  // Replace NULL bytes with spaces
            //}
            LCD_SendChar(c);  // Send character to display
        }
	}
}

int main () 
{
	UART_Init();
	ATMEGA_Init();
	LCD_Init ();
		
	// Display initial message after initialization
	LCD_Home ();
	LCD_SendStr(" ** Roland MT-32 ** "); 
	//LCD_ScrollStr("                    ** Roland MT-32 **                    ", 0, 300, 20);  // 300ms delay for scrolling speed
    _delay_ms(2000);

	// program CGRAM symbol 00
    LCD_SendCmd(CGRAM1_SET);
    LCD_SendChar(CGRAM_ON); LCD_SendChar(CGRAM_ON); LCD_SendChar(CGRAM_ON);
    LCD_SendChar(CGRAM_OFF); LCD_SendChar(CGRAM_ON); LCD_SendChar(CGRAM_ON);
    LCD_SendChar(CGRAM_ON); LCD_SendChar(CGRAM_OFF);

	while (1)
	{
		// Check if there is data in the queue
		if (queueHead != queueTail)
		{
			// Dequeue the next character or command to process
			uint8_t c = LCD_Dequeue();

			// Check if the dequeued byte is a command for the LCD
			if ((c & 0xC0) == LCD_SET_CURSOR)
			{								// Check if top two bits of `c` are `10` (indicating a cursor set command)
				uint8_t address = c & 0x3F; // Extract the DDRAM address (lower 6 bits)

				// UART
	
				if (uartDebug)
				{
					if (address == 0x11)
						UART_NextRow();

					UART_SendStr(" (CUR: ");
					UART_SendHex(address);
					UART_SendStr(") ");
				}
				else
				{
					if (address == 0x00)
					{
						UART_ClearRow();
						UART_HideCursor();
					}
					
					UART_GotoY(address);
				}

				if (!bufferDisplay)
					// Send the command to set the cursor position to this address
					LCD_SendCmd(LCD_SET_CURSOR | address); // Send the cursor position command to the LCD
				else
				{
					// Determine the row and column based on the address
					if (address < 0x40)
					{ // Row 0
						SEDRow = 0;
						SEDCol = address; // Column is the address itself for Row 0
					}
					else
					{ // Row 1
						SEDRow = 1;
						SEDCol = address - 0x40; // Subtract 0x40 to get the column in Row 1
					}
				}
			}
			else
			{
				switch (c)
				{
				case 0x02:
					c = CGRAM1;
					break;

				case 0x01:
					c = 0xff; // Convert 0x01 to 0xff (special character in CGRAM)
					break;

				case 0x7c:
					c = CGRAM1;
					break;

				default:
					// No conversion needed for other values of c
					break;
				}

				if (!bufferDisplay)
					LCD_SendChar(c);
				else
				{
					SEDBuffer[SEDRow][SEDCol] = c;

					// Move the cursor within the buffer for the next character
					if (++SEDCol >= 20)
					{ // Wrap to the next line if at the end of the row
						SEDCol = 0;
						SEDRow = (SEDRow + 1) % 2; // Toggle between Row 0 and Row 1
					}
				}
			}

			// Check if the command involves turning on the cursor
			if ((c & 0x0e) == 0x0e)
			{ // Check if bits 1-3 of `c` match the cursor-on command for the SED1200
				// Send the equivalent HD44780 command to turn on the cursor
				LCD_SendCmd(0x0d | ((c & 0x01) << 1)); // Map cursor setting to HD44780 format
			}
		}
		else
		{
			//if ((millis - SEC_LAST_UPDATE) >= 2000)
			//{
			//	SEC_SHOW = 0;
			//}

			if (bufferDisplay)
				updateDisplay();

			//LCD_GotoXY (1,15);
			//LCD_SendStr ("%d", millis);			
		}
	}
}
