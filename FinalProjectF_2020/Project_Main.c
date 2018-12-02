/****************************************************************************
  Title:       EGR 226 Lab 7
  Filename:    Lab_7_Lab_Part_
  Author(s):   Mathew Yerian-French & Malcolm Macdonald
  Date:        11/11/2018
  Instructor:  Professor Scott Zuidema
  Description:
  Note:         12g
*****************************************************************************/
#include "msp.h"
#include "LCD.h"
#include "serial.h"
#include "temp.h"
#include "promain.h"
#include "speaker.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char TempS[TEMPSTR], timeDisp[TIMESTR] = {' ',' ',':',' ',' ',':',' ',' ',},
     SetTime[8] = {'S','E','T',' ','M','O','D','E',},
     Blink[2] = {' ',' ',},
     On[3] = {' ','O','N',},
     Off[3] = {'O','F','F',},
     readA[10] = {'R','E','A','D','A','L','A','R','M','\0'},
     readT[9] = {'R','E','A','D','T','I','M','E','\0'},
     terminal[100];//to print invalid or valid

int AmPmFlag = 0,
    RTCFlag = 0,
    degreeFlag = 1,
    setTimeFlag = 0,
    setAlarmFlag = 0,
    alarmFlag = 0,
    blinkFlag = 0,
    alarmGoFlag = 0,
    snoozFlag = 0,
    MinSecFlag = 0,
    validFlag = 0,
    rAvalidFlag = 0,
    rTvalidFlag = 0,
    sAvalidFlag = 0,
    sTvalidFlag = 0,
    counter = 0,
    i = 0;              //flags to let the main know something happend

float bright, MaxBright = 7499.00;

enum states
{
    ALARM_GOING_OFF, ALARM_HOUR, ALARM_MIN, //ALARM_SEC,

    NORMAL_CLOCK, TIME_SET_HOUR, TIME_SET_MIN, TIME_SET_SEC,

}state;

struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
}now;

struct
{
    int8_t sec;
    int8_t min;
    int8_t hour;
}set;

struct
{
    //int8_t sec;
    int8_t min;
    int8_t hour;
}alarm;

struct
{
    //int8_t sec;
    int8_t min;
    int8_t hour;
}snooz;

void main(void)
{
    float temp;
    char string[BUFFER_SIZE];
    alarm.min = 0;
    alarm.hour = 1;

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	__disable_irq();

    //INPUT_BUFFER[0]= '\0';  // Sets the global buffer to initial condition of empty
   // setupSerial();//sets up serial communication at 9600 8-Bit no parity
	SysTick_Init();                                 //all initializations
	initPins();
	ADC14init();
	configRTC();

	NVIC -> ISER[0] |= 1 << ADC14_IRQn;            //adc temp interrupt
    NVIC -> ISER[1] |= 1 <<((PORT1_IRQn)&31); //enable port 1 interrupts
    NVIC_EnableIRQ(RTC_C_IRQn);
	
	__enable_irq();                       	//Enable all interrupts for MSP432

    delay_ms(100);//so we dont have problems with LCD
    initLCD();
    delay_ms(100);//so we dont have problems with LCD

    comWrit(0x0C);  comWrit(0xC0);  delay_ms(1);                                           //turns off cursor //starts cursor at top left corner/ the first address //delay for the command
    state = NORMAL_CLOCK;
    beep();
    comWrit(0x01);//***********************************************************big thing right here and the overall program ill explain later*******************************************************************
    delay_ms(100);

	while (1)
	{
//	    readInput(string); // Read the input
	    checkStr(string);   //checks if the string is valid
        temp = getTemp(degreeFlag); //this is to get the tempature and if the degree flag is 1 it'll be in farhrenheit and if its zero it'll be in celsius
        convertTemp(temp);//this is how you would convert the temp to a string called TempS so we can them print to the screen
        LCDbrightUpdate();
if(validFlag)
{
    if(sAvalidFlag)
    {
        convertSerialAlarm(string);
        sAvalidFlag = 0;
    }
    if(sTvalidFlag)
    {
        convertSerialTime(string);
        sTvalidFlag = 0;
    }
    if(rAvalidFlag)
    {
        serialAlarm();
        writeOutput(timeDisp);
       // convertTime();//converts the time to a string
        rAvalidFlag = 0;
    }
    if(rTvalidFlag)
    {
        serialTime();
        writeOutput(timeDisp);
       // convertTime();//converts the time to a string
        rTvalidFlag = 0;
    }
    validFlag = 0;
}
	if(RTCFlag)//alow it to update once every second we can change this later if need be
	{
	    switch(state)
	    {
	    case NORMAL_CLOCK:
	        convertTime();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(alarmFlag == 0)
            {
            comWrit(0x8D);
            delay_ms(1);
            for(i=0;i<3;i++)
            {
                dataWrit(Off[i]);
            }
            }
            if(alarmFlag == 1)
                        {
                        comWrit(0x8D);
                        delay_ms(1);
                        for(i=0;i<3;i++)
                        {
                            dataWrit(On[i]);
                        }
                        }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            break;

	    case TIME_SET_HOUR:
	        convertTimeSet();//converts the time to a string
	        comWrit(0x80);
	        delay_ms(1);
	        displayTime();
	        if(blinkFlag == 1)
	        {
	        comWrit(0x80);
	        delay_ms(1);
	        for(i=0;i<2;i++)
	        {
	            dataWrit(Blink[i]);
	        }
	        blinkFlag = 0;
	        }
	        else
	        {
	            blinkFlag = 1;
	        }
	        if(alarmFlag == 0)
	        {
	            comWrit(0x8D);
	            delay_ms(1);
	            for(i=0;i<3;i++)
	            {
	                dataWrit(Off[i]);
	            }
	        }
	        if(alarmFlag == 1)
	        {
	            comWrit(0x8D);
	            delay_ms(1);
	            for(i=0;i<3;i++)
	            {
	                dataWrit(On[i]);
	            }
	        }
	        comWrit(0xCA);
	        delay_ms(1);
	        displayTemp();
	        break;

        case TIME_SET_MIN:
            convertTimeSet();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(blinkFlag == 1)
            {
            comWrit(0x83);
            delay_ms(1);
            for(i=0;i<2;i++)
            {
                dataWrit(Blink[i]);
            }
            blinkFlag = 0;
            }
            else
            {
                blinkFlag = 1;
            }
            if(alarmFlag == 0)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(Off[i]);
                }
            }
            if(alarmFlag == 1)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(On[i]);
                }
            }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            break;

        case TIME_SET_SEC:
            convertTimeSet();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(blinkFlag == 1)
            {
            comWrit(0x86);
            delay_ms(1);
            for(i=0;i<2;i++)
            {
                dataWrit(Blink[i]);
            }
            blinkFlag = 0;
            }
            else
            {
                blinkFlag = 1;
            }
            if(alarmFlag == 0)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(Off[i]);
                }
            }
            if(alarmFlag == 1)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(On[i]);
                }
            }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            break;

        case ALARM_HOUR:
            convertAlarmSet();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(blinkFlag == 1)
            {
                comWrit(0x80);
                delay_ms(1);
                for(i=0;i<2;i++)
                {
                    dataWrit(Blink[i]);
                }
                blinkFlag = 0;
            }
            else
            {
                blinkFlag = 1;
            }
            if(alarmFlag == 0)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(Off[i]);
                }
            }
            if(alarmFlag == 1)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(On[i]);
                }
            }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            break;

        case ALARM_MIN:
            convertAlarmSet();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(blinkFlag == 1)
            {
                comWrit(0x83);
                delay_ms(1);
                for(i=0;i<2;i++)
                {
                    dataWrit(Blink[i]);
                }
                blinkFlag = 0;
            }
            else
            {
                blinkFlag = 1;
            }
            if(alarmFlag == 0)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(Off[i]);
                }
            }
            if(alarmFlag == 1)
            {
                comWrit(0x8D);
                delay_ms(1);
                for(i=0;i<3;i++)
                {
                    dataWrit(On[i]);
                }
            }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            break;
        /*case ALARM_SEC:
            break;*/
        case ALARM_GOING_OFF:
            beep();
            if(blinkFlag)
            {
                comWrit(0x01);
                delay_ms(10);
                blinkFlag = 0;
                break;
            }
            if(blinkFlag==0)
            {
            convertTime();//converts the time to a string
            comWrit(0x80);
            delay_ms(1);
            displayTime();
            if(alarmFlag == 0)
            {
            comWrit(0x8D);
            delay_ms(1);
            for(i=0;i<3;i++)
            {
                dataWrit(Off[i]);
            }
            }
            if(alarmFlag == 1)
                        {
                        comWrit(0x8D);
                        delay_ms(1);
                        for(i=0;i<3;i++)
                        {
                            dataWrit(On[i]);
                        }
                        }
            comWrit(0xCA);
            delay_ms(1);
            displayTemp();
            blinkFlag = 1;
            break;
            }
            break;

        default:
            state = NORMAL_CLOCK;
            break;
	    }//end of switch
	    if((alarmFlag == 1) && ((alarm.min - now.min)<=5) && (alarm.hour == now.hour) && ((alarm.min - now.min)>0))
	    {
	        counter++;
	        if(counter==3)
	        {
	            counter = 0;
	            bright = bright + (MaxBright * .005);
	            TIMER_A0 -> CCR[WAKE_INST] = bright;
	            // <--- set this equal to duty cycle add 5% brigntness to led for 5 mins before alarm for every 3 seconds
	        }
	    }
	    if((alarm.hour == now.hour) && (alarm.min == now.min) && (alarmFlag == 1) && (0 == now.sec))
	    {
	        state = ALARM_GOING_OFF;
	        alarmGoFlag = 1;
	    }
	    if((snooz.hour == now.hour) && (snooz.min == now.min) && (snoozFlag == 1) && (0 == now.sec) && (alarmFlag == 1))
	    {
            state = ALARM_GOING_OFF;
	        snoozFlag = 0;
	    }
	    if(MinSecFlag)
	        {
	            RTC_C -> TIM0 = (now.sec)>>8;
	        }
	    RTCFlag = 0;//do not move
	        }//End of if second statement
	}//end of while loop

}//end of main
void displayTime(void)
{
    for(i=0;i<TIMESTR;i++)
    {
        dataWrit(timeDisp[i]);
    }
}
void displayTemp(void)
{
    for(i=0;i<TEMPSTR;i++)
    {
        dataWrit(TempS[i]);
    }
}

void convertTime (void)
{

if(now.hour == 0)
{
    timeDisp[0] = '1';
    timeDisp[1] = '2';
    AmPmFlag = 0;
}
if((now.hour == 12))
{
    timeDisp[0] = ((now.hour/10)%10)+48;
    timeDisp[1] = (now.hour%10)+48;
    AmPmFlag = 1;
}
if((now.hour < 10) && (now.hour != 0))
{
   timeDisp[0] = ' ';
   timeDisp[1] = (now.hour+48);
   AmPmFlag = 0;
}
if((now.hour >= 10) && (now.hour < 12))
{
    timeDisp[0] = ((now.hour/10)%10)+48;
    timeDisp[1] = (now.hour%10)+48;
    AmPmFlag = 0;
}
if((now.hour > 21)&&(now.hour < 24))
{
    timeDisp[0] = (((now.hour-12)/10)%10)+48;
    timeDisp[1] = ((now.hour-12)%10)+48;
    AmPmFlag = 1;
}
if((now.hour > 12)&&(now.hour<22))
{
    timeDisp[0] = ' ';
    timeDisp[1] = ((now.hour-12)%10)+48;
    AmPmFlag = 1;
}
if(now.min < 10)
{
    timeDisp[3] = '0';
    timeDisp[4] = (now.min+48);
}
if(now.min >= 10)
{
    timeDisp[3] = ((now.min/10)%10)+48;
    timeDisp[4] = (now.min%10)+48;
}
if(now.sec < 10)
{
    timeDisp[6] = '0';
    timeDisp[7] = (now.sec+48);
}
if(now.sec >= 10)
{
    timeDisp[6] = ((now.sec/10)%10)+48;
    timeDisp[7] = (now.sec%10)+48;
}
if(AmPmFlag == 0)
{
    timeDisp[8] = ' ';
    timeDisp[9] = 'A';
    timeDisp[10] = 'M';
}
if(AmPmFlag == 1)
{
    timeDisp[8] = ' ';
    timeDisp[9] = 'P';
    timeDisp[10] = 'M';
}

}

void RTC_C_IRQHandler(void)
{
    now.sec = RTC_C -> TIM0>>0 & 0x00FF;
    now.min = RTC_C -> TIM0>>8 & 0x00FF;
    now.hour = RTC_C -> TIM1>>0 & 0x00FF;

    RTCFlag = 1;
    RTC_C -> CTL0 = 0xA510;
}

void convertTemp(float TEMP)//converts the number to a string
{
    int t;
    t = TEMP * 100;

    TempS[0] = (t/1000)+48;
    TempS[1] = ((t/100)%10)+48;
    TempS[2] = '.';
    TempS[3] = ((t/10)%10)+48;
    //TempS[4] = (t%10)+48;
    TempS[4] = 0b11011111;//for the degree symbol
    if(degreeFlag == 1)
    {
        TempS[5] = 'F';
    }
    else
    {
        TempS[5] = 'C';
    }
}

void BUTTON_IN(void)                                                                                                                //button port interrupt
{
    ONOFFUP_PORT -> IFG & ONOFFUP_PIN;
    SDOWN_PORT -> IFG & SDOWN_PIN;
    SET_PORT -> IFG & SET_PIN;
    SETALARM_PORT -> IFG & SETALARM_PIN;
    MINSEC_PORT -> IFG & MINSEC_PIN;
    BUTTON_PORT -> IFG & BUTTON_PIN;

    if(!(MINSEC_PORT -> IN & MINSEC_PIN) && (MinSecFlag == 0))
    {
        delay_ms(DEBOUN);                                                                                                              //delay for debounce
        while(!(MINSEC_PORT -> IN & MINSEC_PIN)){}
        MinSecFlag = 1;
    }
    if(!(MINSEC_PORT -> IN & MINSEC_PIN) && (MinSecFlag == 1))
        {
            delay_ms(DEBOUN);                                                                                                              //delay for debounce
            while(!(MINSEC_PORT -> IN & MINSEC_PIN)){}
            MinSecFlag = 0;
        }

   if((setTimeFlag == 1) && (setAlarmFlag == 0))
        {
       if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == TIME_SET_HOUR))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
           if(set.hour >= 0)
           set.hour--;
           if(set.hour < 0)
           {
             set.hour = 23;
           }

       }
       if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == TIME_SET_HOUR))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
           if(set.hour<24)
           set.hour++;
           if(set.hour == 24)
                      {
                          set.hour = 0;
                      }
       }
       if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == TIME_SET_MIN))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
           if(set.min >= 0)
           set.min--;
           if(set.min < 0)
           {
               set.hour--;
               if(set.hour <= 0)
               {
                   set.hour = 23;
               }
               set.min = 59;
           }
       }
       if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == TIME_SET_MIN))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
           if(set.min <= 60)
           set.min++;
           if(set.min >= 60)
           {
               set.hour++;
               if(set.hour == 24)
               {
                   set.hour = 0;
               }
               set.min = 0;
           }
       }
       if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == TIME_SET_SEC))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
           if(set.sec >= 0)
                      set.sec--;
                      if(set.sec < 0)
                      {
                          set.min--;
                          if(set.min < 0)
                          {
                              set.min = 59;
                              set.hour--;
                              if(set.hour <= 0)
                              {
                                  set.hour = 23;
                              }
                          }
                          set.sec = 59;
                      }
       }
       if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == TIME_SET_SEC))
       {
           delay_ms(DEBOUN);                                                                                                              //delay for debounce
           while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
           if(set.sec <= 60)
                      set.sec++;
                      if(set.sec >= 60)
                      {
                          set.min++;
                          if(set.min >= 60)
                          {
                              set.min = 0;
                              set.hour++;
                              if(set.hour == 24)
                              {
                                  set.hour = 0;
                              }
                          }
                          set.sec = 0;
                      }
       }

        if(!(SET_PORT -> IN & SET_PIN) && (state == TIME_SET_HOUR))
                {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(SET_PORT -> IN & SET_PIN)){}                                                                                  //if button is being held
                    state = TIME_SET_MIN;
                }
        if(!(SET_PORT -> IN & SET_PIN) && (state == TIME_SET_MIN))
                {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(SET_PORT -> IN & SET_PIN)){}                                                                                  //if button is being held
                    state = TIME_SET_SEC;
                }
        if(!(SET_PORT -> IN & SET_PIN) && (state == TIME_SET_SEC))
                {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(SET_PORT -> IN & SET_PIN)){}                                                                                  //if button is being held
                    state = NORMAL_CLOCK;
                    setRTC();
                    setTimeFlag = 0;
                    now.hour = set.hour;
                    now.min = set.min;
                    now.sec = set.sec;
                }
        }

    if((setTimeFlag == 0) && (setAlarmFlag == 0))
    {
    if(!(SET_PORT -> IN & SET_PIN))
            {
            delay_ms(DEBOUN);                                                                                                              //delay for debounce
            while(!(SET_PORT -> IN & SET_PIN)){}                                                                                  //if button is being held
                setTimeFlag = 1;
                state = TIME_SET_HOUR;
                set.hour = now.hour;
                set.min = now.min;
                set.sec = now.sec;
            }
    if(!(SETALARM_PORT -> IN & SETALARM_PIN))
                {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(SETALARM_PORT -> IN & SETALARM_PIN)){}                                                                                  //if button is being held
                    setAlarmFlag = 1;
                    state = ALARM_HOUR;
                }
    if((alarmFlag == 0) && (!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)))
    {
        delay_ms(DEBOUN);                                                                                                              //delay for debounce
        while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
        alarmFlag = 1;
        //bright = MAXBRIGHT * .05;
    }
    if((alarmFlag == 1) && (!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)))
        {
            delay_ms(DEBOUN);                                                                                                              //delay for debounce
            while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
            alarmFlag = 0;
            counter = 0;
            alarmGoFlag = 0;
            state = NORMAL_CLOCK;
            bright = 0;//*********************************************************************set the led to 0 brightness
            snoozFlag = 0;
            TIMER_A0 -> CCR[WAKE_INST] = bright;
        }
    }


    if((setAlarmFlag == 1))
           {
          if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == ALARM_HOUR))
          {
              delay_ms(DEBOUN);                                                                                                              //delay for debounce
              while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
              if(alarm.hour >= 0)
                  alarm.hour--;
              if(alarm.hour < 0)
              {
                  alarm.hour = 23;
              }

          }
          if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == ALARM_HOUR))
          {
              delay_ms(25);                                                                                                              //delay for debounce
              while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
              if(alarm.hour<24)
                  alarm.hour++;
              if(alarm.hour == 24)
                         {
                  alarm.hour = 0;
                         }
          }
          if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == ALARM_MIN))
          {
              delay_ms(25);                                                                                                              //delay for debounce
              while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
              if(alarm.min >= 0)
                  alarm.min--;
              if(alarm.min < 0)
              {
                  alarm.hour--;
                  if(alarm.hour < 0)
                  {
                      alarm.hour = 23;
                  }
                  alarm.min = 59;
              }
          }
          if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == ALARM_MIN))
          {
              delay_ms(DEBOUN);                                                                                                              //delay for debounce
              while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
              if(alarm.min < 60)
                  alarm.min++;
              if(alarm.min == 60)
              {
                  alarm.hour++;
                  if(alarm.hour == 24)
                  {
                      alarm.hour = 0;
                  }
                  alarm.min = 0;
              }
          }
        /*  if(!(SDOWN_PORT -> IN & SDOWN_PIN) && (state == ALARM_SEC))
          {
              delay_ms(DEBOUN);                                                                                                              //delay for debounce
              while(!(SDOWN_PORT -> IN & SDOWN_PIN)){}                                                                                  //if button is being held
              if(alarm.sec >= 0)
                  alarm.sec--;
                         if(alarm.sec < 0)
                         {
                             alarm.min--;
                             if(alarm.min < 0)
                             {
                                 alarm.min = 59;
                                 alarm.hour--;
                                 if(alarm.hour <= 0)
                                 {
                                     alarm.hour = 24;
                                 }
                             }
                             alarm.sec = 59;
                         }
          }
          if(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN) && (state == ALARM_SEC))
          {
              delay_ms(DEBOUN);                                                                                                              //delay for debounce
              while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
              if(alarm.sec <= 60)
                  alarm.sec++;
                         if(alarm.sec >= 60)
                         {
                             alarm.min++;
                             if(alarm.min >= 60)
                             {
                                 alarm.min = 0;
                                 alarm.hour++;
                                 if(alarm.hour == 25)
                                 {
                                     alarm.hour = 1;
                                 }
                             }
                             alarm.sec = 0;
                         }
          }*/

           if(!(SETALARM_PORT -> IN & SETALARM_PIN) && (state == ALARM_HOUR))
                   {
                   delay_ms(DEBOUN);                                                                                                              //delay for debounce
                   while(!(SETALARM_PORT -> IN & SETALARM_PIN)){}                                                                                  //if button is being held
                       state = ALARM_MIN;
                   }
           if(!(SETALARM_PORT -> IN & SETALARM_PIN) && (state == ALARM_MIN))
                   {
                   delay_ms(DEBOUN);                                                                                                              //delay for debounce
                   while(!(SETALARM_PORT -> IN & SETALARM_PIN)){}                                                                                  //if button is being held
                       //state = ALARM_SEC;
                   state = NORMAL_CLOCK;
                   setAlarmFlag = 0;
                   convertTime();
                   timeDisp[5] = ':';
                   }
          /* if(!(SETALARM_PORT -> IN & SETALARM_PIN) && (state == ALARM_SEC))
                   {
                   delay_ms(DEBOUN);                                                                                                              //delay for debounce
                   while(!(SETALARM_PORT -> IN & SETALARM_PIN)){}                                                                                  //if button is being held
                       state = NORMAL_CLOCK;
                       setAlarmFlag = 0;
                       convertTime();
                       timeDisp[5] = ':';
                   }*/
           }
    if((alarmGoFlag == 1) && (!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)))
            {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(ONOFFUP_PORT -> IN & ONOFFUP_PIN)){}                                                                                  //if button is being held
                alarmFlag = 0;
                counter = 0;
                alarmGoFlag = 0;
                bright = 0;//*********************************************************************set the led to 0 brightness
                snoozFlag = 0;
                state = NORMAL_CLOCK;
                TIMER_A0 -> CCR[WAKE_INST] = bright;
            }
    if((alarmFlag == 1) && !(SDOWN_PORT -> IN & SDOWN_PIN) && (snoozFlag == 0))
    {
        snooz.min = now.min + 5;
        snooz.hour = now.hour;
        state = NORMAL_CLOCK;
        snoozFlag = 1;
    }
    if(!(BUTTON_PORT -> IN & BUTTON_PIN) && (degreeFlag == 0))
        {
            delay_ms(DEBOUN);                                                                                                              //delay for debounce
            while(!(BUTTON_PORT -> IN & BUTTON_PIN)){}
            degreeFlag = 1;
        }
        if(!(BUTTON_PORT -> IN & BUTTON_PIN) && (degreeFlag == 1))
            {
                delay_ms(DEBOUN);                                                                                                              //delay for debounce
                while(!(BUTTON_PORT -> IN & BUTTON_PIN)){}
                degreeFlag = 0;
            }
   SET_PORT -> IFG &= ~SET_PIN;//turns flag to 0
   SDOWN_PORT -> IFG &= ~SDOWN_PIN;
   ONOFFUP_PORT -> IFG &= ~ONOFFUP_PIN;
   SETALARM_PORT -> IFG &= ~SETALARM_PIN;//turns flag to 0
   MINSEC_PORT -> IFG &= ~MINSEC_PIN;//turns flag to 0
   BUTTON_PORT -> IFG &= ~BUTTON_PIN;//turns flag to 0
}
void setRTC(void)
{
    RTC_C -> TIM0 = set.min<<8 | set.sec;
    RTC_C -> TIM1 = 00<<8 | set.hour;
}
void convertTimeSet (void)
{

    if(set.hour == 0)
    {
        timeDisp[0] = '1';
        timeDisp[1] = '2';
        AmPmFlag = 0;
    }
    if((set.hour == 12))
    {
        timeDisp[0] = ((set.hour/10)%10)+48;
        timeDisp[1] = (set.hour%10)+48;
        AmPmFlag = 1;
    }
    if((set.hour < 10) && (set.hour != 0))
    {
       timeDisp[0] = ' ';
       timeDisp[1] = (set.hour+48);
       AmPmFlag = 0;
    }
    if((set.hour >= 10) && (set.hour < 12))
    {
        timeDisp[0] = ((set.hour/10)%10)+48;
        timeDisp[1] = (set.hour%10)+48;
        AmPmFlag = 0;
    }
    if((set.hour > 21)&&(set.hour < 24))
    {
        timeDisp[0] = (((set.hour-12)/10)%10)+48;
        timeDisp[1] = ((set.hour-12)%10)+48;
        AmPmFlag = 1;
    }
    if((set.hour > 12)&&(set.hour<22))
    {
        timeDisp[0] = ' ';
        timeDisp[1] = ((set.hour-12)%10)+48;
        AmPmFlag = 1;
    }
    if(set.min < 10)
    {
        timeDisp[3] = '0';
        timeDisp[4] = (set.min+48);
    }
    if(set.min >= 10)
    {
        timeDisp[3] = ((set.min/10)%10)+48;
        timeDisp[4] = (set.min%10)+48;
    }
    if(set.sec < 10)
    {
        timeDisp[6] = '0';
        timeDisp[7] = (set.sec+48);
    }
    if(set.sec >= 10)
    {
        timeDisp[6] = ((set.sec/10)%10)+48;
        timeDisp[7] = (set.sec%10)+48;
    }
    if(AmPmFlag == 0)
    {
        timeDisp[8] = ' ';
        timeDisp[9] = 'A';
        timeDisp[10] = 'M';
    }
    if(AmPmFlag == 1)
    {
        timeDisp[8] = ' ';
        timeDisp[9] = 'P';
        timeDisp[10] = 'M';
    }
}

void convertAlarmSet(void)
{

    if(alarm.hour == 0)
    {
        timeDisp[0] = '1';
        timeDisp[1] = '2';
        AmPmFlag = 0;
    }
    if((alarm.hour == 12))
    {
        timeDisp[0] = ((alarm.hour/10)%10)+48;
        timeDisp[1] = (alarm.hour%10)+48;
        AmPmFlag = 1;
    }
    if((alarm.hour < 10) && (alarm.hour != 0))
    {
       timeDisp[0] = ' ';
       timeDisp[1] = (alarm.hour+48);
       AmPmFlag = 0;
    }
    if((alarm.hour >= 10) && (alarm.hour < 12))
    {
        timeDisp[0] = ((alarm.hour/10)%10)+48;
        timeDisp[1] = (alarm.hour%10)+48;
        AmPmFlag = 0;
    }
    if((alarm.hour > 21)&&(alarm.hour < 24))
    {
        timeDisp[0] = (((alarm.hour-12)/10)%10)+48;
        timeDisp[1] = ((alarm.hour-12)%10)+48;
        AmPmFlag = 1;
    }
    if((alarm.hour > 12)&&(alarm.hour<22))
    {
        timeDisp[0] = ' ';
        timeDisp[1] = ((alarm.hour-12)%10)+48;
        AmPmFlag = 1;
    }
    if(alarm.min < 10)
    {
        timeDisp[3] = '0';
        timeDisp[4] = (alarm.min+48);
    }
    if(alarm.min >= 10)
    {
        timeDisp[3] = ((alarm.min/10)%10)+48;
        timeDisp[4] = (alarm.min%10)+48;
    }
 /*   if(set.sec < 10)
    {
        timeDisp[6] = '0';
        timeDisp[7] = (set.sec+48);
    }
    if(set.sec >= 10)
    {
        timeDisp[6] = ((set.sec/10)%10)+48;
        timeDisp[7] = (set.sec%10)+48;
    }*/

    timeDisp[5] = ' ';
    timeDisp[6] = ' ';
    timeDisp[7] = ' ';

    if(AmPmFlag == 0)
    {
        timeDisp[8] = ' ';
        timeDisp[9] = 'A';
        timeDisp[10] = 'M';
    }
    if(AmPmFlag == 1)
    {
        timeDisp[8] = ' ';
        timeDisp[9] = 'P';
        timeDisp[10] = 'M';
    }

}
void checkStr(char *string)
{
    if(string[0] != '\0')//if the first char is a null it wont do anything
           {
               if((strlen(string)==16)&&(string[0] == 'S')&&(string[1] == 'E')&&(string[2] == 'T')&&(string[3] == 'T')&&(string[4] == 'I')&&(string[5] == 'M')&&(string[6] == 'E')&&(string[7] == ' ')&&(string[10] == ':')&&(string[13] == ':'))//if it is 16 chars long for SETTIME HH:MM:SS
               {
                   if( ((string[8] >= '0')&&(string[8] <= '2')) && ((string[9] >= '0')&&(string[9] <= '9')) && ((string[11] >= '0')&&(string[11] <= '5')) && ((string[12] >= '0')&&(string[12] <= '9')) && ((string[14] >= '0')&&(string[14] <= '5')) && ((string[15] >= '0')&&(string[15] <= '9')))
                   {
                       if((string[8] == '2') && ((string[9] >= '0')&&(string[9] <= '3')))
                       {
                   sprintf(terminal, "Valid %s\n", string);
                   writeOutput(terminal);
                   validFlag = 1;
                   sTvalidFlag = 1;
                       }
                       else if (!(string[8] == '2'))
                       {
                           sprintf(terminal, "Valid %s\n", string);
                           writeOutput(terminal);
                           validFlag = 1;
                           sTvalidFlag = 1;
                       }
                   }
               }
               if((strlen(string)==14)&&(string[0] == 'S')&&(string[1] == 'E')&&(string[2] == 'T')&&(string[3] == 'A')&&(string[4] == 'L')&&(string[5] == 'A')&&(string[6] == 'R')&&(string[7] == 'M')&&(string[8] == ' ')&&(string[11] == ':'))//if it 14 Chars Long for SETALARM HH:MM
               {
                   if( ((string[9] >= '0')&&(string[9] <= '2')) && ((string[10] >= '0')&&(string[10] <= '9')) && ((string[12] >= '0')&&(string[12] <= '5')) && ((string[13] >= '0')&&(string[13] <= '9')))
                   {
                       if((string[9] == '2') && ((string[10] >= '0')&&(string[10] <= '3')))
                       {
                   sprintf(terminal, "Valid %s\n", string);
                   writeOutput(terminal);
                   validFlag = 1;
                   sAvalidFlag = 1;
                       }
                       else if (!(string[9] == '2'))
                       {
                           sprintf(terminal, "Valid %s\n", string);
                           writeOutput(terminal);
                           validFlag = 1;
                           sAvalidFlag = 1;
                       }
                   }
               }
               if((strlen(string)==8)&&!(strcmp(string,readT)))//if it 8 Chars Long for READTIME
               {
                   sprintf(terminal, "%s-->", string);
                   writeOutput(terminal);
                   validFlag = 1;
                   rTvalidFlag = 1;
               }
               if((strlen(string)==9)&&!(strcmp(string,readA)))//if it 9 Chars Long for READALARM
               {
                   sprintf(terminal, "%s-->", string);
                   writeOutput(terminal);
                   validFlag = 1;
                   rAvalidFlag = 1;
               }
               else if(validFlag == 0)//anything other than the rest
                              {
                               sprintf(terminal, "Invalid %s\n", string);
                               writeOutput(terminal);
                               validFlag = 0;
                               }
           }//null if
}//function
void convertSerialAlarm(char *string)
{
    /* string[9] >= '0')&&(string[9] <= '9'
     string[10] >= '0')&&(string[10] <= '9'
     string[12] >= '0')&&(string[12] <= '9'
     string[13] >= '0')&&(string[13] <= '9'*/
    alarm.hour = (string[10]-48) + ((string[9]-48)*10);
    alarm.min = (string[13]-48) + ((string[12]-48)*10);
}
void convertSerialTime(char *string)
{
   /* string[8] >= '0')&&(string[8] <= '9'
    string[9] >= '0')&&(string[9] <= '9'
    string[11] >= '0')&&(string[11] <= '9'
    string[12] >= '0')&&(string[12] <= '9'
    string[14] >= '0')&&(string[14] <= '9'
    string[15] >= '0')&&(string[15] <= '9'*/

    set.hour = (string[9]-48) + ((string[8]-48)*10);
    set.min = (string[12]-48) + ((string[11]-48)*10);
    set.sec = (string[15]-48) + ((string[14]-48)*10);

    RTC_C -> TIM0 = set.min<<8 | set.sec;
    RTC_C -> TIM1 = 00<<8 | set.hour;
}
void serialTime(void)
{
    if(now.hour < 10)
    {
        timeDisp[0] = '0';
        timeDisp[1] = (now.hour%10)+48;
    }
    if(now.hour >= 10)
    {
            timeDisp[0] = ((now.hour/10)%10)+48;
            timeDisp[1] = (now.hour%10)+48;
    }
        if(now.min < 10)
        {
            timeDisp[3] = '0';
            timeDisp[4] = (now.min+48);
        }
        if(now.min >= 10)
        {
            timeDisp[3] = ((now.min/10)%10)+48;
            timeDisp[4] = (now.min%10)+48;
        }
        if(now.sec < 10)
        {
            timeDisp[6] = '0';
            timeDisp[7] = (now.sec+48);
        }
        if(now.sec >= 10)
        {
            timeDisp[6] = ((now.sec/10)%10)+48;
            timeDisp[7] = (now.sec%10)+48;
        }
        timeDisp[5] = ':';
        timeDisp[8] = ' ';
        timeDisp[9] = ' ';
        timeDisp[10] = ' ';

}
void serialAlarm(void)
{
    if(alarm.hour < 10)
    {
        timeDisp[0] = '0';
        timeDisp[1] = (alarm.hour%10)+48;
    }
    if(alarm.hour >= 10)
    {
            timeDisp[0] = ((alarm.hour/10)%10)+48;
            timeDisp[1] = (alarm.hour%10)+48;
    }
        if(alarm.min < 10)
        {
            timeDisp[3] = '0';
            timeDisp[4] = (alarm.min+48);
        }
        if(alarm.min >= 10)
        {
            timeDisp[3] = ((alarm.min/10)%10)+48;
            timeDisp[4] = (alarm.min%10)+48;
        }

        timeDisp[5] = ' ';
        timeDisp[6] = ' ';
        timeDisp[7] = ' ';
        timeDisp[8] = ' ';
        timeDisp[9] = ' ';
        timeDisp[10] = ' ';

}
