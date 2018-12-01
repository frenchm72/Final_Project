#ifndef __PROMAIN_H__
#define __PROMAIN_H__

#define TEMPSTR 6
#define TIMESTR 11

#define ONOFFUP_PORT P1
#define ONOFFUP_PIN BIT7

#define SDOWN_PORT P1
#define SDOWN_PIN BIT6

#define SETALARM_PORT P1
#define SETALARM_PIN BIT0

#define SET_PORT P1
#define SET_PIN BIT5

#define MINSEC_PORT P1
#define MINSEC_PIN BIT4

#define WAKE_PORT P2
#define WAKE_PIN BIT4
#define WAKE_INST 1

#define LCDBRIGHT_PORT P2
#define LCDBRIGHT_PIN BIT5
#define LCDBRIGHT_INST 2

#define MAXBRIGHT 7499

#define DEBOUN 100

void displayTime(void);
void displayTemp(void);
void convertTime(void);
void convertTimeSet(void);
void convertAlarmSet(void);

void initPins(void);

void delay_micro(unsigned mic);
void delay_ms(unsigned ms);
void SysTick_Init(void);

void configRTC(void);
void setRTC(void);

#endif
