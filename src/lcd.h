#define E	PD7
#define RS	PD6

#define LCD_ROW_LENGTH  20

#define DISP_ON                         0b00001100      //LCD control constants
#define DISP_OFF                        0b00001000      //
#define CLR_DISP                        0b00000001      //
#define CUR_HOME                        0b00000010      //

#define LCD_SET_CURSOR 0x80     // Command to set cursor position

void LCD_Init ();
void LCD_SendCmd (uint8_t c);
void LCD_SendChar (uint8_t c);

extern volatile uint8_t LCDCol;

uint8_t LCD_Dequeue();
void LCD_Enqueue(uint8_t c);

void LCD_GotoXY (uint8_t row, uint8_t col);
void LCD_Home ();

void LCD_SendStr(const char *text);
void LCD_ScrollStr(const char *text, uint8_t row, uint16_t delay_ms, uint8_t length);