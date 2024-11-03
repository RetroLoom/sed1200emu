#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"


volatile uint8_t sed_byte;
volatile uint8_t rs;
volatile uint8_t count_e;
volatile uint8_t gotData = 0;

extern volatile uint8_t queueHead, queueTail;


//
// 
//
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

	DDRB =	0b00001111;

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

	DDRD = 0b11000000;

//	// Timer 0
//	// -------
//	TCCR0A = 2;
//	TCCR0B = 5;		// prescaler 101, clk/1024
//	OCR0A = 5;

//	TIMSK0 = _BV(OCIE0A);


	// INT0 (CS) ---\___/---	
	// INT1 (E)      ___/---

	MCUCR = _BV(ISC11) | _BV(ISC10) |		// 11 - INT1 rising edge
			_BV(ISC01);						// 10 - INT0 falling edge

	GICR = _BV(INT1) | _BV(INT0);			// enable INT1 and INT0



	sei ();


	LCD_Init ();

	LCD_SendCmd (CLR_DISP);

	LCD_SendCmd (CUR_HOME);


	// program CGRAM symbol 00
	LCD_SendCmd (0x40);
	LCD_SendChar (0x04);
	LCD_SendChar (0x04);
	LCD_SendChar (0x04);
	LCD_SendChar (0x00);
	LCD_SendChar (0x04);
	LCD_SendChar (0x04);
	LCD_SendChar (0x04);
	LCD_SendChar (0x00);



//	LCD_SendChar ('7');
//	LCD_SendChar ('7');


//	_delay_ms (500);

//	LCD_SendCmd (CLR_DISP);
//	LCD_SendCmd (CUR_HOME);


//	if (gotData)
//		LCD_SendChar (sed_byte);
	

//	LCD_Home ();


	while (1) {

/*
		if (gotData) {
			gotData = 0;
		
			if (rs) {			
				LCD_Enqueue (sed_byte);
			}	
		}
*/

 		if (queueHead != queueTail) {
			uint8_t c = LCD_Dequeue ();
			
			if ((c & 0xc0) == 0x80)
				LCD_SendCmd (c);
			else {

				if (c == 0x02)
					c = 0x7c;
				
				if (c == 0x01)
					c = 0xff;


				LCD_SendChar ((c == 0x7c) ? 0x00: c);	// instead '|' (0x7c) send CGRAM char 0
			}


			if ((c & 0x0e) == 0x0e) {	// sed1200 CURSOR ON
				LCD_SendCmd (0x0d | ((c & 0x01) << 1));	// hd44780 
			}



		}
		


		
/*


		if (gotData) {
			gotData = 0;

			if (rs) {
				//LCD_SendChar (sed_byte);
				LCD_PutChar (sed_byte);
			}	

			if (LCDCol > 12)
				LCD_SendChar ('!');


		}
*/

	}

}

/***********************************************************************
 *
 * INT0 (CS)
 *
 ***********************************************************************/
ISR(INT0_vect) {
	sei ();

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


