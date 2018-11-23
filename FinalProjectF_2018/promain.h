#ifndef __PROMAIN_H__
#define __PROMAIN_H__

void initPins(void);
void delay_micro(unsigned mic);
void delay_ms(unsigned ms);
void SysTick_Init(void);

#define ALARM_BUTTON_IN   PORT1_IRQHandler


#endif
