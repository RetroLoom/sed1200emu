#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "atmega.h"
#include "lcd.h"
#include "tables.h"

volatile uint8_t SEDBuffer [2][20];
volatile uint8_t SEDRow = 0, SEDCol = 0;

uint8_t SEC_POS = 0;
uint8_t SEC_COL = 0;
uint8_t SEC_OFFSET = 0;
uint8_t SEC_SHOW = 0;
uint32_t SEC_LAST_UPDATE = 0;

extern volatile uint8_t queueHead, queueTail;

void updateDisplay()
{
    // Draw buffer to display
	uint8_t row;
    for (row = 0; row < 1; row++)  // Iterate both rows
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
	//

	// Draw buffer to display
	//uint8_t row;
    for (row = 1; row < 2; row++)  // Iterate both rows
    {
        // Set the cursor to the start of the current row
        uint8_t cursorPos = (row == 0 ? 0x00 : 0x40);  // DDRAM addresses
        LCD_SendCmd(LCD_SET_CURSOR | cursorPos);
        
        // Draw each column in the row
		uint8_t col;
		if (SEC_SHOW)
		{
			for (col = 0; col < SEC_COL + SEC_OFFSET; col++)
			{
				uint8_t c = SEDBuffer[0][SEC_POS + col];
				// if (c == 0x00) {
				//	c = ASCII_SPACE;  // Replace NULL bytes with spaces
				// }
				LCD_SendChar(c); // Send character to display
			}

			// Clear empty space
			for (col = 0; col < 20 - SEC_COL + SEC_OFFSET; col++)
            	LCD_SendChar(ASCII_SPACE);  // Send character to display
		}
		else
		{
			for (col = 0; col <= SEC_POS - SEC_OFFSET + 1; col++)
			{
				uint8_t c = SEDBuffer[0][col];
				// if (c == 0x00) {
				//	c = ASCII_SPACE;  // Replace NULL bytes with spaces
				// }
				LCD_SendChar(c); // Send character to display
			}
		}

		

	}
}

int main () 
{
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

	int page = 1;
	uint32_t last_update = 0; // Initialize a timestamp variable
    uint32_t delay_interval = 2000; // Set delay in milliseconds

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

				// Send the command to set the cursor position to this address
				//LCD_SendCmd(LCD_SET_CURSOR | address); // Send the cursor position command to the LCD

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

				if (SEDCol == 17)
				{
					SEC_OFFSET = 4;
					SEC_POS = SEDCol - SEC_OFFSET;
					SEC_COL = 0;
					SEC_SHOW = 1;
					SEC_LAST_UPDATE = millis;
				}
				//else
				//{
				//	SED_SEC_COL = 0;
				//}

				if (SEDCol == 0)
				{
					SEC_COL = 0;
					//SEC_SHOW = 0;
				}

				page = 1;

				//LCD_GotoXY (1,17);
				//LCD_SendStr ("%d", SEDCol);
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

				// Send the character to the LCD; if it's '|', send the CGRAM custom char 0 instead
				//LCD_SendChar((c == 0x7c) ? 0x00 : c); // Convert '|' (0x7c) to CGRAM character 0
				SEDBuffer[SEDRow][SEDCol] = c;

				if (SEC_POS)
					SEC_COL++;
				
				// Move the cursor within the buffer for the next character
				if (++SEDCol >= 20)
				{ // Wrap to the next line if at the end of the row
					SEDCol = 0;
					SEDRow = (SEDRow + 1) % 2; // Toggle between Row 0 and Row 1
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
			if ((millis - SEC_LAST_UPDATE) >= 2000)
			{
				SEC_SHOW = 0;
			}

			updateDisplay();

			//LCD_GotoXY (1,15);
			//LCD_SendStr ("%d", millis);			
		}
	}
}
