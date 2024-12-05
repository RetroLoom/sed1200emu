#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "atmega.h"
#include "lcd.h"
#include "tables.h"

#define F_CPU 8000000UL
#define BAUD 9600        // Desired baud rate
#define MYUBRR (F_CPU/16/BAUD-1)  // UBRR value for baud rate

uint8_t xlat [16] = {
	0b0000, 0b1000, 0b0010, 0b1010,
	0b0100, 0b1100, 0b0110, 0b1110,
	0b0001, 0b1001, 0b0011, 0b1011,
	0b0101, 0b1101, 0b0111, 0b1111
};

volatile uint8_t sed_byte;
volatile uint8_t rs;
volatile uint8_t count_e;

volatile uint32_t millis = 0; // Millisecond counter, volatile because it's modified in ISR

void ATMEGA_Init()
{
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

    //Timer0_Init();
	Timer1_Init();
	//Timer2_Init();

    sei ();
}

void UART_Init(void) {
    // Set baud rate
    uint16_t ubrr_value = MYUBRR;
    UBRRH = (uint8_t)(ubrr_value >> 8);  // Upper 8 bits of UBRR
    UBRRL = (uint8_t)ubrr_value;        // Lower 8 bits of UBRR

    // Enable transmitter and receiver
    UCSRB = (1 << TXEN) | (1 << RXEN);

    // Set frame format: 8 data bits, no parity, 1 stop bit
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

	UART_SendString("UART Initialized\r\n");
}

void UART_SendChar(char c) {
    // Wait for the transmit buffer to be empty
    while (!(UCSRA & (1 << UDRE))) {
        // Do nothing
    }
    // Put the character into the buffer
    UDR = c;
}

void UART_SendString(const char* str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

void Timer0_Init() {
    // Timer0 Configuration for Compare Match mode
    OCR0 = 249;  // Set the compare match value (adjust as needed)
    TCCR0 = (1 << WGM01) | (1 << CS02) | (1 << CS00);  // CTC mode, prescaler clk/1024
    TIMSK |= (1 << OCIE0);  // Enable Timer0 compare match interrupt
}

void Timer1_Init() {
    /* Set up Timer1 for 1 ms overflow
    TCCR1A = 0;                     // Set Timer1 to normal mode
    TCCR1B = (1 << WGM12) | (1 << CS11); // CTC mode, prescaler of 8
    OCR1A = 999;                    // Output compare match value for 1ms at 8 MHz/8 prescaler
    TIMSK = (1 << OCIE1A);         // Enable output compare A match interrupt
	*/

	// Set up Timer1 for direct updateDisplay()
	TCCR1B |= (1 << WGM12);      // Configure Timer1 in CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10); // Set prescaler to 64
    OCR1A = 12500;               // Set compare value for 100 ms interval
    TIMSK |= (1 << OCIE1A);      // Enable Timer1 compare match A interrupt
	//

	/* Set up Timer1 for a 1-second interval
	TCCR1A = 0;				// Set Timer1 to normal mode (CTC)
	TCCR1B |= (1 << WGM12); // Configure Timer1 in CTC mode
	TCCR1B |= (1 << CS12);	// Set prescaler to 256
	OCR1A = 31249;			// Compare value for 1 second interval at 8 MHz / 256 prescaler
	TIMSK |= (1 << OCIE1A); // Enable Timer1 compare match A interrupt
	*/
}

void Timer2_Init() {
	// Set up Timer2 for 1ms overflow (8-bit timer)
	TCCR2 = (1 << WGM21) | (1 << CS21); // CTC mode, prescaler of 8
	OCR2 = 249;							// Output compare match value for 1ms at 8 MHz/8 prescaler
	TIMSK |= (1 << OCIE2);				// Enable output compare A match interrupt
}

//ISR(TIMER0_COMPA_vect) 
//{
        
        /*
		lcdIdx ++;
		
		if (lcdIdx >= 40)
			lcdIdx = 0;

		if (lcdIdx == 0)
			LCD_SendCmd (0x80);

		if (lcdIdx == 20)
			LCD_SendCmd (0xc0);
		*/

		//LCD_SendChar (LCDBuffer [lcdIdx]);
		
		//updateDisplay();
//}

// Timer1 compare match ISR, triggers every 1 ms
ISR(TIMER1_COMPA_vect) {
    millis += 100;
    //updateDisplay();
}

//ISR(TIMER2_COMPA_vect) {
//    millis++;
//}

ISR(INT0_vect) {
	count_e = 0;
}

ISR(INT1_vect) {
	//sei ();
	

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
