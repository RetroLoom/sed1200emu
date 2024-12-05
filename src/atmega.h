// Timer0 Configuration for Compare Match mode
#define OCR0    _SFR_IO8(0x3C)  // Compare Match register
#define TCCR0   _SFR_IO8(0x33)  // Timer/Counter Control register
//#define TIMSK   _SFR_IO8(0x39)  // Timer Interrupt Mask register
#define WGM01   3               // Bit 3 in TCCR0 for CTC mode
#define OCIE0   1               // Bit 1 in TIMSK to enable compare match interrupt

extern volatile uint32_t millis;

void ATMEGA_Init();
void UART_Init(void);
void UART_SendChar(char c);
void UART_SendStr(const char* format, ...);
void UART_SendHex(uint8_t value);
void Timer0_Init();
void Timer1_Init();
void Timer2_Init();