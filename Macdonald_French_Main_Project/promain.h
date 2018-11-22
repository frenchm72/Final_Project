#ifndef __PROMAIN_H__
#define __PROMAIN_H__

#define TIMEOUT 2400//20 is 1 second 1200 is one min

#define BUTTON_PORT P1
#define BUTTON_PIN  BIT5
#define BUTTON_IN   PORT1_IRQHandler

void StsTick_Init(void);
void delay_micro(int mic);
void delay_ms(int ms);

void convertINTmoto(void);
void convertINTred(void);
void convertINTblue(void);
void convertINTgreen(void);

#endif
