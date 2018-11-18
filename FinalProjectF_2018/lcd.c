/****************************************************************************
  Title:       EGR 226 Lab 6
  Filename:    Lab_6
  Author(s):   Mathew Yerian-French
  Date:        09/30/2018
  Instructor:  Professor Scott Zuidema
  Description:
  Note:
*****************************************************************************/

#include "msp.h"
#include "LCD.h"
#include "promain.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


void initLCD(void)
{
  comWrit(0x03);//REST 3 times
  delay_ms(10);
  comWrit(0x03);
  delay_ms(10);
  comWrit(0x03);
  delay_ms(10);

  comWrit(0x02);//4bit mode
  delay_micro(100);
  comWrit(0x02);
  comWrit(0x06);//2 lin 5x7 format
  delay_micro(100);
  comWrit(0x0F);//display on cursor blinking
  delay_micro(100);
  comWrit(0x01);//clears display
  delay_micro(100);
  comWrit(0x06);//incremnt cursor
  delay_micro(100);
}
void PulsEnablePin(void)
{
    PULSE_PORT -> OUT &= ~PULSE_PIN;
    delay_micro(10);
    PULSE_PORT -> OUT |= PULSE_PIN;
    delay_micro(10);
    PULSE_PORT -> OUT &= ~PULSE_PIN;
    delay_micro(10);
}

void pushNib(uint8_t nib)
{
    DATA_PORT -> OUT &= ~0xF0; //clears P4.4 -P4.7
    DATA_PORT -> OUT |= (nib & 0x0F)<<4;
    PulsEnablePin();
}

void pushByt(uint8_t byte)
{
    uint8_t nib;

    nib = ( byte & 0xF0)>>4;
    pushNib(nib);

    nib = (byte & 0x0F);
    pushNib(nib);
    delay_micro(100);
}

void comWrit(uint8_t com)
{
    RS_PORT -> OUT &= ~RS_PIN;
    pushByt(com);
}

void dataWrit(uint8_t data)
{
    RS_PORT -> OUT |= RS_PIN;
    pushByt(data);
}



