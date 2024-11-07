#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "atmega.h"
#include "lcd.h"
#include "tables.h"

#define QUEUELEN	256

volatile uint8_t LCDBuffer [2][20];
volatile uint8_t LCDRow = 0, LCDCol = 0;
volatile uint8_t lcdIdx = 0;

volatile uint8_t queueHead, queueTail;
uint8_t queue [QUEUELEN];

/*

	E	RS  	D7  D6  D5  D4
	---	--- 	---------------
	PD7	PD6 	PB3 PB2 PB1 PB0

*/

void _E () {	
	PORTD |= _BV(E);
	_delay_us (100);		// 100
	PORTD &= ~_BV(E);	
}

void LCD_Enqueue (uint8_t c) {

	queue [queueHead++] = c;
	if (queueHead >= QUEUELEN)
		queueHead = 0;

}

uint8_t LCD_Dequeue () {
uint8_t c;

	c = queue [queueTail++];
	if (queueTail >= QUEUELEN)
		queueTail = 0;

	return c;

}

void LCD_Init() {	
	
    // Initial delay for LCD power-up
    _delay_ms(50);

	  // Send the command 0x03 three times to initialize the LCD
    PORTB = 0x03;  // First command to set 8-bit mode
    _E();          
    _delay_ms(5);  

    PORTB = 0x03;  // Second command
    _E();          
    _delay_ms(5);  

    PORTB = 0x03;  // Third command
    _E();          
    _delay_ms(2);  
	
    PORTB = 0x02;  // Send 0x02 to set to 4-bit mode
    _E();          
    _delay_ms(2);  

    PORTB = 0x02;  // Re-send 0x02 to ensure 4-bit mode is set
    _E();          
    _delay_ms(2);  

    PORTB = 0x08;  // configures the display for 1 line with a 5x8 font
    _E();          
    _delay_ms(2);  

    PORTB = 0x00;  // Prepare to turn display off
    _E();          
    _delay_ms(2);  

    PORTB = 0x08;  // Turn Off the display
    _E();          
    _delay_ms(2);  

	PORTB = 0x00;  // Prepare to clear display
    _E();          
    _delay_ms(2);  

	PORTB = 0x01;  // Clear display
    _E();          
    _delay_ms(2);  

	PORTB = 0x00;  // Prepare for entry mode set
    _E();          
    _delay_ms(2);  

    // Set display control: Display ON, Cursor OFF, Blink OFF
    PORTB = 0x0C;  // Send 0x0C for display control (Display ON)
    _E();          
    _delay_ms(2);  
}


void LCD_SendChar (uint8_t c) {

        //_delay_ms (LCD_CHAR_SPEED);              //Delay for LCD char

		_delay_us (100);

        PORTB = ((c >> 4) & 0xF);
		PORTD |= _BV(RS);                                              //set LCD to data mode

        _E();


        PORTB = (c & 0xF);

        _E();
}

void LCD_SendCharAt(uint8_t row, uint8_t col, uint8_t c)
{
	if (row < LCD_ROWS && col < LCD_COLS)
	{
		LCD_GotoXY(col, row);	 // Set cursor position
		LCD_SendChar(c);		 // Send character to LCD
		LCDBuffer[row][col] = c; // Update buffer
	}
}

//--------Send command to LCD-----------------------------
void LCD_SendCmd (uint8_t c) {

        //_delay_ms (LCD_CHAR_SPEED);                      //Delay for LCD char
		_delay_us (100);

//      data = 0b00001111 | c;                                  //get upper nibble
//      PORTC = (PORTC | 0b11110000) & data;        //set D4-D7

        PORTB = (c >> 4) & 0xF;

        PORTD &= ~_BV(RS);                                              //set LCD to command mode
        _E();

//      data = c << 4;                                                  //get down nibble
//      PORTC = (PORTC & 0b00001111) | data;        //set D4-D7 (only PORTC4-PORTC7)

        PORTB = c & 0xF;

        PORTD &= ~_BV(RS);                                                      //set LCD to command mode
        _E();
}


void LCD_GotoXY (uint8_t row, uint8_t col) {
	uint8_t address = 0x00;
    if (row == 0) {
        address = 0x00 + col;  // First row
    } else if (row == 1) {
        address = 0x40 + col;  // Second row
    }
    LCD_SendCmd(LCD_SET_CURSOR | address);

	LCDRow = row;
	LCDCol = col;
}

void LCD_Home () {
	LCD_GotoXY (0,0);
	LCDCol = LCDRow = 0;
};

void LCD_Clear() {
    // Clear the buffer by setting each cell to a space (0x20)
	uint8_t row;
    for (row = 0; row < LCD_ROWS; row++) {
		uint8_t col;
        for (col = 0; col < LCD_COLS; col++) {
            LCDBuffer[row][col] = ASCII_SPACE;  // 0x20 is the ASCII code for space
        }
    }

    // Clear the physical LCD and reset the cursor to the home position
    LCD_SendCmd(CLR_DISP);  // Clear display command for the HD44780
    LCD_Home();              // Move the cursor to the home position (0,0)
}

void LCD_PutChar(uint8_t c) {
    // Place the character in the buffer at the current row and column
    if (LCDRow < LCD_ROWS && LCDCol < LCD_COLS) {
        LCDBuffer[LCDRow][LCDCol] = c;
    }

    // Update the column position and handle wrapping
    if (++LCDCol >= LCD_COLS) {  // Wrap to the next row if we exceed the column limit
        LCDCol = 0;
        if (++LCDRow >= LCD_ROWS) {
            LCDRow = 0;  // Wrap to the top row if we exceed the row limit
        }
    }
}

void LCD_PutHex (uint8_t c) {
unsigned char hi, lo;

        hi = (c >> 4) & 0x0f;
        lo = c & 0x0f;

        LCD_PutChar (hi < 10 ? hi + 0x30 : hi + 55);
        LCD_PutChar (lo < 10 ? lo + 0x30 : lo + 55);

}

void LCD_PutString (uint8_t *s) {
	while (*s)
		LCD_PutChar (*s++);
}

void LCD_PutPGMString (const char *s) {
uint8_t b, i = 0;	
	while (1) {
		b = pgm_read_byte (s + i);
    	if ((b == 0) || (b == 0xff))
    		break;    
		LCD_PutChar (b);
	i++;
	}
}

void LCD_SendStr(const char *format, ...)
{
	char text[64] = "";
	va_list args;
	va_start ( args, format );
	vsprintf ( text, format, args );

	uint8_t i;
	for (i = 0; i < strlen(text); i++)
		LCD_SendChar(text[i]);
}

void LCD_ScrollStr(const char *text, uint8_t row, uint16_t delay_ms, uint8_t length) 
{    
	if (strlen(text) <= length) {
        LCD_GotoXY(row, 0);
        LCD_SendStr(text);
    } else {
		uint8_t offset;
		for (offset = 0; offset <= strlen(text) - length; offset++)
		{
            LCD_GotoXY(row, 0);  // Move cursor to the start of the row

            // Display 20 characters from the current offset
			uint8_t i;
			for (i = 0; i < LCD_COLS; i++)
			{
			
                LCD_SendChar(text[offset + i]);
            }

            // Delay to control scrolling speed
            uint16_t count;
            for (count = 0; count < delay_ms; count++) {
                _delay_ms(1);  // Delay by 1 ms each iteration
            }
        }
    }
}

