#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"
#include "tables.h"

volatile uint8_t sed_byte;
volatile uint8_t rs;
volatile uint8_t count_e;

int SED_ROWS = 2;
int SED_COLS = 20;
uint8_t SEDBuffer [2][20];
volatile uint8_t SEDRow = 0, SEDCol = 0;

extern volatile uint8_t queueHead, queueTail;

uint8_t xlat [16] = {
	0b0000, 0b1000, 0b0010, 0b1010,
	0b0100, 0b1100, 0b0110, 0b1110,
	0b0001, 0b1001, 0b0011, 0b1011,
	0b0101, 0b1101, 0b0111, 0b1111
};

int main () {



	// PORT B
	// ---------------------------
	// PB3		(LCD) D7		o
	// PB2		(LCD) D6		o
	// PB1		(LCD) D5		o
	// PB0		(LCD) D4		o

	DDRB =	0xF;

	// PORT C
	// ---------------------------
	// PC4		(SED1200) RS	i
	// PC3		(SED1200) D0	i
	// PC2		(SED1200) D2	i
	// PC1		(SED1200) D1	i
	// PC0		(SED1200) D3	i

	DDRC = 0;

	// PORT D
	// --------------------------
	// PD7		(LCD) E			o
	// PD6		(LCD) RS 		o
	// PD3		(SED1200) E 	i aka INT1
	// PD2		(SED1200) CS 	i aka INT0

	DDRD = 0xC0;

	MCUCR = _BV(ISC11) | _BV(ISC10) |		// 11 - INT1 rising edge
			_BV(ISC01);						// 10 - INT0 falling edge

	GICR = _BV(INT1) | _BV(INT0);			// enable INT1 and INT0

	LCD_Init ();
	sei ();
	
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

				// Send the command to set the cursor position to this address
				LCD_SendCmd(LCD_SET_CURSOR | address); // Send the cursor position command to the LCD

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
				
				// Move the cursor within the buffer for the next character
				if (++SEDCol >= SED_COLS)
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
			// Draw buffer to display
			uint8_t row;
			for (row = 0; row < 2; row++)
			{
				// Set the cursor to the start of the current row
				LCD_SendCmd(LCD_SET_CURSOR | (row == 0 ? 0x00 : 0x40));

				// Draw the row
				uint8_t col;
				for (col = 0; col < SED_COLS; col++)
				{
					uint8_t c = SEDBuffer[row][col];
					switch (c)
					{
						case 0x00:
						c = ASCII_SPACE;
						break;

					default:
						// No conversion needed for other values of c
						break;
					}

					LCD_SendChar(c);
				}
			}
		}
	}
}

/***********************************************************************
 *
 * INT0 (CS)
 *
 ***********************************************************************/
ISR(INT0_vect) {
	count_e = 0;
}


/***********************************************************************
 *
 * INT1 (E)
 *
 ***********************************************************************/
ISR(INT1_vect) {
//	sei ();

	if (!(count_e & 0x01)) {
		// upper data D7...D4
		sed_byte = xlat [(PINC & 0x0f)] << 4;
	}
	else {
		// lower data D3...D0
		sed_byte |= xlat [(PINC & 0x0f)];
		rs = PINC & _BV(PC4);		

		if (rs) {
			// data
			LCD_Enqueue (sed_byte);
		}
		else {
			// command
			switch (sed_byte & 0xc0) {				
				case 0x80:
					LCD_Enqueue (sed_byte);
					break;
				case 0xc0:
					LCD_Enqueue (0x80 + 10 + (sed_byte & 0x3f));
					break;
				default:
					break;

			}

//			if ((sed_byte & 0xc0) == 0x80) {	// SET CURSOR ADDR
//				LCD_Enqueue (sed_byte);
//			}
		}	
	}

	count_e ++;
	
}


