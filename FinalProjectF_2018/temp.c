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
#include "temp.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


    void ADC14init(void)
    {
        ADC_PORT -> SEL0 |= ADC_PIN;//set adc conversion
        ADC_PORT -> SEL1 |= ADC_PIN;
        ADC14 -> CTL0 &= ~ADC14_CTL0_ENC;//enable conersion
        ADC14 -> CTL0 |= ADC14_CTL0_PDIV__32 |//setpdiv to 32
                         ADC14_CTL0_SHP |//hold off sampling timer
                         ADC14_CTL0_SSEL__SMCLK |//sub master clock
                         ADC14_CTL0_SHT1__32 |//hol regisert for 32 clock
                         ADC14_CTL0_ON;//turns on
        ADC14 -> CTL1 |= ADC14_CTL1_RES__14BIT;//10 but resolution
        ADC14 -> MCTL[ADC_INST] = 0;//defult config for adc
        ADC14 -> IER0;//interrupt on
        ADC14 -> CTL0 |= ADC14_CTL0_ENC;//enable conversion
    }

