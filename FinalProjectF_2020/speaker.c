<<<<<<< HEAD
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

void beep(void)//PULSES THE PIN TO MAKE A BEEP NOISE
{
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    //delay_ms(BIG_PAUSE);
=======
void beep(void){
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT |= BEEP_PIN;
    delay_ms(SMALL_PAUSE);
    BEEP_PORT->OUT &=~ BEEP_PIN;
    delay_ms(BIG_PAUSE);
>>>>>>> branch 'master' of https://github.com/frenchm72/Final_Project.git
}
