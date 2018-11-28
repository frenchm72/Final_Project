/****************************************************************************
  Title:       EGR 226
  Filename:
  Author(s):   Mathew Yerian-French
  Date:        11/11/2018
  Instructor:  Professor Scott Zuidema
  Description:
  Note:
*****************************************************************************/
#include "msp.h"
#include "LCD.h"
#include "serial.h"
#include "temp.h"
#include "promain.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void initPins(void)
{
    PULSE_PORT->SEL0 &= ~PULSE_PIN;                  //setting up pulse pin
    PULSE_PORT->SEL1 &= ~PULSE_PIN;
    PULSE_PORT->DIR |= PULSE_PIN;
    PULSE_PORT->OUT |= PULSE_PIN;

    RS_PORT->SEL0 &= ~RS_PIN;                  //setting up RS pin
    RS_PORT->SEL1 &= ~RS_PIN;
    RS_PORT->DIR |= RS_PIN;
    RS_PORT->OUT |= RS_PIN;

    DATA_PORT->SEL0 &= ~DATA_PIN;                  //setting up DATA pins
    DATA_PORT->SEL1 &= ~DATA_PIN;
    DATA_PORT->DIR |= DATA_PIN;
    DATA_PORT->OUT |= DATA_PIN;

    /*  SEC_PORT -> SEL0 &= ~ONOFFUP_PIN;
    ONOFFUP_PORT -> SEL1 &= ~ONOFFUP_PIN;
    ONOFFUP_PORT -> DIR  |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> REN  |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> OUT  |=  ONOFFUP_PIN;*/
    ONOFFUP_PORT -> SEL0 &= ~ONOFFUP_PIN;
    ONOFFUP_PORT -> SEL1 &= ~ONOFFUP_PIN;
    ONOFFUP_PORT -> DIR  &= ~ONOFFUP_PIN;
    ONOFFUP_PORT -> REN  |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> OUT  |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> IES  |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> IE   |=  ONOFFUP_PIN;
    ONOFFUP_PORT -> IFG  = 0;

    /*  SDOWN_PORT -> SEL0 &= ~SDOWN_PIN;
    SDOWN_PORT -> SEL1 &= ~SDOWN_PIN;
    SDOWN_PORT -> DIR  |=  SDOWN_PIN;
    SDOWN_PORT -> REN  |=  SDOWN_PIN;
    SDOWN_PORT -> OUT  |=  SDOWN_PIN;*/
    SDOWN_PORT -> SEL0 &= ~SDOWN_PIN;
    SDOWN_PORT -> SEL1 &= ~SDOWN_PIN;
    SDOWN_PORT -> DIR  &= ~SDOWN_PIN;
    SDOWN_PORT -> REN  |=  SDOWN_PIN;
    SDOWN_PORT -> OUT  |=  SDOWN_PIN;
    SDOWN_PORT -> IES  |=  SDOWN_PIN;
    SDOWN_PORT -> IE   |=  SDOWN_PIN;
    SDOWN_PORT -> IFG  = 0;

    /*  SEC_PORT -> SEL0 &= ~SEC_PIN;
    SET_PORT -> SEL1 &= ~SET_PIN;
    SET_PORT -> DIR  |=  SET_PIN;
    SET_PORT -> REN  |=  SET_PIN;
    SET_PORT -> OUT  |=  SET_PIN;*/
    SET_PORT -> SEL0 &= ~SET_PIN;
    SET_PORT -> SEL1 &= ~SET_PIN;
    SET_PORT -> DIR  &= ~SET_PIN;
    SET_PORT -> REN  |=  SET_PIN;
    SET_PORT -> OUT  |=  SET_PIN;
    SET_PORT -> IES  |=  SET_PIN;
    SET_PORT -> IE   |=  SET_PIN;
    SET_PORT -> IFG  = 0;

    /*  SETALARM_PORT -> SEL0 &= ~SETALARM_PIN;
    SETALARM_PORT -> SEL1 &= ~SETALARM_PIN;
    SETALARM_PORT -> DIR  |=  SETALARM_PIN;
    SETALARM_PORT -> REN  |=  SETALARM_PIN;
    SETALARM_PORT -> OUT  |=  SETALARM_PIN;*/
    SETALARM_PORT -> SEL0 &= ~SETALARM_PIN;
    SETALARM_PORT -> SEL1 &= ~SETALARM_PIN;
    SETALARM_PORT -> DIR  &= ~SETALARM_PIN;
    SETALARM_PORT -> REN  |=  SETALARM_PIN;
    SETALARM_PORT -> OUT  |=  SETALARM_PIN;
    SETALARM_PORT -> IES  |=  SETALARM_PIN;
    SETALARM_PORT -> IE   |=  SETALARM_PIN;
    SETALARM_PORT -> IFG  = 0;
}


void SysTick_Init(void)//sys tick initialization
{
    SysTick -> CTRL = 0;//disable systick during step
    SysTick -> LOAD = 0x00FFFFFF;//max reload value
    SysTick -> VAL = 0;//clears it
    SysTick -> CTRL = 0x00000005;//enables systick 3MHz no interrupts

}
void delay_micro(unsigned mic)//function for micro seconds using systick3
{
    SysTick -> LOAD = (mic*3) - 1;
    SysTick -> VAL = 0;
    while((SysTick->CTRL & BIT(16))==0);
}
void delay_ms(unsigned ms)//delay in milliseconds using systick
{
    SysTick->LOAD =(3000*ms)-1;
    SysTick->VAL = 0;
    while((SysTick->CTRL & BIT(16))==0);
}
void configRTC(void)
{
    RTC_C -> CTL0 = 0xA510;
    RTC_C -> CTL13 = 0x0000;
    RTC_C -> TIM0 = 00<<8 | 00;
    RTC_C -> TIM1 = 00<<8 | 1;
}