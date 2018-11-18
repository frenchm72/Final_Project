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

int hour , min , sec, Time=0, degreeFlag = 0;
char TempS[7];

void main(void)
{
    float temp; //theres probaly an warning here but we havent used temp yet so dont worry about it

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	SysTick_Init();                                 //all initializations
	initPins();
	ADC14init();
	initLCD();
	
	TIMER32_2->CONTROL = 0b111000110;			//Sets timer 2 for Enabled, Periodic, With Interrupt, No Prescaler, 32 bit mode, Wrapping mode
      NVIC_EnableIRQ( T32_INT2_IRQn );          //Enable Timer32_2 interrupt.
      NVIC -> ISER[0] |= 1 << ADC14_IRQn;       //adc temp interrupt
   __enable_interrupt ( );                        	//Enable all interrupts for MSP432

   temp = getTemp(degreeFlag); //this is to get the tempature and if the degree flag is 1 it'll be in farhrenheit and if its zero it'll be in celsius
   convertTemp(temp);//this is how you would convert the temp to a string called TempS so we can them print to the screen

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

void convert_Time (void)//************************************************************still needs fixed to restart at 24 hours
{
	//24 hours will include 86,400 incrementations of the variable "Time"
	//12 hours will include 43,200 incrementations of the clock.
	//These numbers will be used to distinguish A.M. vs. P.M.
	
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

void convertTemp(float TEMP)//converts the number to a string
{
    int t;
    t = TEMP * 100;

    TempS[0] = (t/1000)+48;
    TempS[1] = ((t/100)%10)+48;
    TempS[2] = '.';
    TempS[3] = ((t/10)%10)+48;
    TempS[4] = (t%10)+48;
    if(degreeFlag == 1)
    {
        TempS[5] = 'F';
    }
    else
    {
        TempS[5] = 'C';
    }
}
