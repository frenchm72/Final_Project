#include "msp.h"
#include "motor.h"
#include "promain.h"
#include <stdio.h>
#include <stdlib.h>

void initMOTOpins(void)
{
    MOTOR_PORT -> SEL0 &= ~MOTOR_PIN;
    MOTOR_PORT -> SEL1 &= ~MOTOR_PIN;
    MOTOR_PORT -> DIR  |=  MOTOR_PIN;

    SERVO_PORT->SEL0 &= ~SERVO_PIN;                  //setting up LEDs
    SERVO_PORT->SEL1 &= ~SERVO_PIN;
    SERVO_PORT->DIR |=   SERVO_PIN;
}
void initTimerAmoto(void)
{
    TIMER_A0 -> CCR[0] = TA0_PERIOD;//speed
    TIMER_A0 -> CCR[MOTOR_INST] = 0;
    TIMER_A0 -> CCTL[MOTOR_INST] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0 -> CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_0
            | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;

    TIMER_A2 -> CCR[0] = SERVO_PERIOD;//Position
        TIMER_A2 -> CCR[SERVO_INST] = 0;
        TIMER_A2 -> CCTL[SERVO_INST] = TIMER_A_CCTLN_OUTMOD_7;
        TIMER_A2 -> CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_0
                | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;
}
void releTimer(void)
{
    TIMER_A0 -> CTL = 0x00;
}
void initMotoPWM(void)
{
    MOTOR_PORT -> SEL0 |=  MOTOR_PIN;// t0 make the pwm signal
    MOTOR_PORT -> SEL1 &= ~MOTOR_PIN;
    MOTOR_PORT -> DIR  |=  MOTOR_PIN;
}

void setMotoPWM(uint16_t speed)
{
    TIMER_A0 -> CCR[MOTOR_INST] = speed;
}
void onServoPWM(void)
{
    SERVO_PORT->SEL0 |= SERVO_PIN;
    SERVO_PORT->SEL1 &= ~SERVO_PIN;
    SERVO_PORT->DIR |=   SERVO_PIN;
}
void offServoPWM(void)
{
    SERVO_PORT->SEL0 &= ~SERVO_PIN;
    SERVO_PORT->SEL1 &= ~SERVO_PIN;
}
void setServoOpenPWM(void)
{
    TIMER_A2 -> CCR[SERVO_INST] = OPEN;
}
void setServoClosePWM(void)
{
    TIMER_A2 -> CCR[SERVO_INST] = CLOSE;
}
