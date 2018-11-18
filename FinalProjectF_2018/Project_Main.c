/****************************************************************************
  Title:       EGR 226 Lab 7
  Filename:    Lab_7_Lab_Part_
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

void convert_Time (void);

int Time=0;

uint32_t adc_input;
float V_REF = 3.3, V_in;
int hour , min , sec;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	SysTick_Init();
	initPins();
	ADC14init();
	initLCD();
	
	TIMER32_2->CONTROL = 0b111000110;			//Sets timer 2 for Enabled, 
						//Periodic
						//With Interrupt
						//No Prescaler, 32 bit mode
						//Wrapping mode
      NVIC_EnableIRQ( T32_INT2_IRQn );          //Enable Timer32_2 interrupt.
      NVIC -> ISER[0] |= 1 << ADC14_IRQn;//adc temp
   __enable_interrupt ( );                        	//Enable all interrupts for MSP432

	while (1)
	{
	}

}

void T32_INT2_IRQHandler ( )                         	 //Interrupt Handler for Timer 2. 
{
    TIMER32_2->INTCLR = 1;                          	//Clear interrupt flag so it does not interrupt again immediately 
	Time++;
    TIMER32_2->LOAD = 3000000 ;                 //Set to a countdown of 1 second on 3 MHz clock
	
}

void convert_Time (void)
{
	//24 hours will include 86,400 incrementations of the variable "Time"
	//12 hours will include 43,200 incrementations of the clock.
	//These numbers will be used to distinguish A.M. vs P.M.
	
	if (Time < 43200) //A.M.
	{
		hour = (Time/3600);
		min = (Time - (hour*3600) )/ 60;
		sec = Time - (hour*3600) - (min*60);
		
	if ( hour == 0)
        {
            hour = 12;
        }
	}
		else if (Time >= 43200)  //P.M.
	{
		hour = (Time/3600);
		min = (Time - (hour*3600) )/ 60;
		sec = Time - (hour*3600) - (min*60);

		hour = hour - 12;
	}
}

/*void convertVal(void)//converts the number to a string
{
    int v, t;
    v = V_in * 1000;
    t = TEMP * 100;

    V_IN[0] = (v/1000)+48;
    V_IN[1] = '.';
    V_IN[2] = ((v/100)%10)+48;
    V_IN[3] = ((v/10)%10)+48;
    V_IN[4] = (v%10)+48;

    TemP[0] = (t/1000)+48;
    TemP[1] = ((t/100)%10)+48;
    TemP[2] = '.';
    TemP[3] = ((t/10)%10)+48;
    TemP[4] = (t%10)+48;
}*/
