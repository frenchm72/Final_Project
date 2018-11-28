#ifndef __LCD_H__
#define __LCD_H__

#define PULSE_PORT P6
#define PULSE_PIN BIT1

#define DATA_PORT P6
#define DATA_PIN 0xF0;

#define RS_PORT P6
#define RS_PIN BIT0

void initLCD(void);
void PulsEnablePin(void);
void pushNib(uint8_t nib);
void pushByt(uint8_t byte);
void comWrit(uint8_t com);
void dataWrit(uint8_t data);

#endif
