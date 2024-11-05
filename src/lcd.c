#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "lcd.h"

#define QUEUELEN	256

uint8_t LCDBuffer [20 * 2];
volatile uint8_t LCDRow = 0, LCDCol = 0;
volatile uint8_t lcdIdx = 0;


volatile uint8_t queueHead, queueTail;
uint8_t queue [QUEUELEN];


// Timer0 Configuration for Compare Match mode
#define OCR0    _SFR_IO8(0x3C)  // Compare Match register
#define TCCR0   _SFR_IO8(0x33)  // Timer/Counter Control register
#define TIMSK   _SFR_IO8(0x39)  // Timer Interrupt Mask register
#define WGM01   3               // Bit 3 in TCCR0 for CTC mode
#define OCIE0   1               // Bit 1 in TIMSK to enable compare match interrupt


/*

	E	RS  	D7  D6  D5  D4
	---	--- 	---------------
	PD7	PD6 	PB3 PB2 PB1 PB0

*/


/***************************************************
 *
 * _E
 *
 ***************************************************/
void _E () {	
	PORTD |= _BV(E);
	_delay_us (100);		// 100
	PORTD &= ~_BV(E);	
}


/***************************************************
 *
 * LCD_Enqueue
 *
 ***************************************************/
void LCD_Enqueue (uint8_t c) {

	queue [queueHead++] = c;
	if (queueHead >= QUEUELEN)
		queueHead = 0;

}

/***************************************************
 *
 * LCD_Dequeue
 *
 ***************************************************/
uint8_t LCD_Dequeue () {
uint8_t c;

	c = queue [queueTail++];
	if (queueTail >= QUEUELEN)
		queueTail = 0;

	return c;

}

/***************************************************
 *
 * LCD_Init
 *
 ***************************************************/
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

    // Timer0 Configuration for Compare Match mode
    OCR0 = 5;  // Set the compare match value (adjust as needed)
    TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);  // CTC mode, prescaler clk/1024
    TIMSK |= (1 << OCIE0);  // Enable Timer0 compare match interrupt
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


/***************************************************
 *
 * ISR TIMER0_COMPA_vect
 *
 ***************************************************/
ISR(TIMER0_COMP_vect) {


        //sei ();	
        
		lcdIdx ++;
		
		if (lcdIdx >= 40)
			lcdIdx = 0;

		if (lcdIdx == 0)
			LCD_SendCmd (0x80);

		if (lcdIdx == 20)
			LCD_SendCmd (0xc0);

		LCD_SendChar (LCDBuffer [lcdIdx]);
		

}

/***************************************************
 *
 * LCD_GotoXY
 *
 ***************************************************/
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

/***************************************************
 *
 * LCD_Home	
 *
 ***************************************************/
void LCD_Home () {
	LCD_GotoXY (0,0);
	LCDCol = LCDRow = 0;
};

/***************************************************
 *
 * LCD_Clear
 *
 ***************************************************/
void LCD_Clear () {
uint8_t i;
	for (i = 0; i < 40; i++)
		LCDBuffer [i] = 0x20;

	LCD_Home ();
};

/***************************************************
 *
 * LCD_PutChar
 *
 ***************************************************/
void LCD_PutChar (uint8_t c) {
	LCDBuffer [LCDRow * 16 + LCDCol] = c;
	if (++LCDCol >= 16) {
		LCDCol = 0;
		if (++LCDRow >= 2)
			LCDRow = 0;
	}
}


/***************************************************
 *
 * LCD_PutHex
 *
 ***************************************************/
void LCD_PutHex (uint8_t c) {
unsigned char hi, lo;

        hi = (c >> 4) & 0x0f;
        lo = c & 0x0f;

        LCD_PutChar (hi < 10 ? hi + 0x30 : hi + 55);
        LCD_PutChar (lo < 10 ? lo + 0x30 : lo + 55);

}


/***************************************************
 *
 * LCD_PutString
 *
 ***************************************************/
void LCD_PutString (uint8_t *s) {
	while (*s)
		LCD_PutChar (*s++);
}

/***************************************************
 *
 * LCD_PutPGMString
 *
 ***************************************************/
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

void LCD_SendStr(const char *text)
{
	uint8_t i = 0;
	while (i < 20)
	{
		LCD_SendChar(text[i]);
		i++;
	}
}

void LCD_ScrollStr(const char *text, uint8_t row, uint16_t delay_ms, uint8_t length) {
    
	if (strlen(text) <= length) {
        LCD_GotoXY(row, 0);
        LCD_SendStr(text);
    } else {
		uint8_t offset = 0;

        while (offset <= strlen(text) - length) {
            LCD_GotoXY(row, 0);  // Move cursor to the start of the row

            // Display 20 characters from the current offset
			uint8_t i = 0;
            while (i < 20) {
                LCD_SendChar(text[offset + i]);
				i++;
            }

            // Delay to control scrolling speed
            _delay_ms(delay_ms);
			offset++;
        }
    }
}

