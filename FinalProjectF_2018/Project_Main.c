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

void SysTick_Init_interrupt (void);                       //Initialize SysTick with interrupt
void ADC14_init (void);
int _delay_ms(uint16_t ms);
void TEMP_Pin_Init (void);
void convert_Time (void);

static volatile uint16_t result;
float ADC, V_REF, TEMPC, TEMPF;

char TempC[5],TemPF[5];

int Time=0;


void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	ADC14_init ( );                                   //Initialize 14bit ADC
  	Pin_Init( );                                      //intitialize pins
    	SysTick_Init_interrupt ( );                       //Initialize SysTick with interrupt
   	                               
	
	TIMER32_2->CONTROL = 0b111000110;			//Sets timer 2 for Enabled, 
						//Periodic
						//With Interrupt
						//No Prescaler, 32 bit mode
						//Wrapping mode
      NVIC_EnableIRQ( T32_INT2_IRQn );          //Enable Timer32_2 interrupt.  
   __enable_interrupt ( );                        	//Enable all interrupts for MSP432

  
	while (1)
	{
	}

}
void SysTick_Init_interrupt (void) {             //initialization of systic timer
    SysTick -> CTRL = 0;                         // disable SysTick During step
    SysTick -> LOAD = 1500000;                   // load value configured for 0.5 second interrupts
    SysTick -> VAL = 0;                          // any write to current clears it
    SysTick -> CTRL = 0x00000007;                // enable SysTick, 3MHz,   Interrupts
  }
void TEMP_Pin_Init (void) {
        P5->SEL0          |= BIT5;  // configure pin 5.5 for A0 input
        P5->SEL1          |= BIT5;
}
void ADC14_init (void) {
    ADC14 -> CTL0  &=~    ADC14_CTL0_ENC;     // disable ADC converter during initialization
      ADC14->CTL0      |=      0x04200210;      // S/H pulse mode, SMCLK, 16 sample clocks
      ADC14->CTL1       =      0x00000030;      // 14 bit resolution
      ADC14->CTL1      |=      0x00000000;      // convert for mem0 register
      ADC14->MCTL[0] =     0x00000000;      // ADC14INCHx = 0 for mem[0]
      ADC14->CTL0 |=       ADC14_CTL0_ENC;  // enable ADC14ENC, Starts the ADC after confg.

 }
int _delay_ms(uint16_t ms)
{
    SysTick->LOAD =(3000 * ms)-1;
    SysTick->VAL = 0;
    while((SysTick -> CTRL & BIT(16))==0){};
    return ms;
}
void SysTick_Handler (void) {

    ADC14->CTL0 |=  ADC14_CTL0_SC;          //start conversation or write ADC14->CTL0 |= 1
    while ( ! (ADC14->IFGR0 & BIT0 )  );    //wait for conversation to complete

    result = ADC14->MEM[0];             // get the value from the ADC
    ADC = (result * 3.3) / 16384;       //Calculates input voltage
    TEMPC = (1000*ADC)/ 10 ;            //conversion to Celcius
    TEMPF = (TEMPC*9/5)+32;             //conversion to Fahrenheit
}
}
void convertVal(void)//converts the number to a string
{
	int t_F, t_C;
	t_F = TEMPF * 100;
	t_C = TEMPC * 100;

 
	TemPF[0] = (t_F/1000)+48;
	TemPF[1] = ((t_F/100)%10)+48;
	TemPF[2] = '.';
	TemPF[3] = ((t_F/10)%10)+48;
	TemPF[4] = (t_F%10)+48;
	
		TemPC[0] = (t_C/1000)+48;
	TemPC[1] = ((t_C/100)%10)+48;
	TemPC[2] = '.';
	TemPC[3] = ((t_C/10)%10)+48;
	TemPC[4] = (t_C%10)+48;
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

