#define E	PD7
#define RS	PD6

#define LCD_CHAR_SPEED	3	// 2

#define DISP_ON                         0b00001100      //LCD control constants
#define DISP_OFF                        0b00001000      //
#define CLR_DISP                        0b00000001      //
#define CUR_HOME                        0b00000010      //

void LCD_Init ();
void LCD_SendCmd (uint8_t c);
void LCD_SendChar (uint8_t c);

extern volatile uint8_t LCDCol;

uint8_t LCD_Dequeue();
void LCD_Enqueue(uint8_t c);

