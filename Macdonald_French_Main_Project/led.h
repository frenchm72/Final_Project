#ifndef __LED_H__
#define __LED_H__

#define TA0_PERIOD 7499 //30k tic corrasponds to 25ms period 400Hz

#define GREEN_PORT P2
#define GREEN_PIN BIT5
#define GREEN_INST 2

#define BLUE_PORT P2
#define BLUE_PIN BIT6
#define BLUE_INST 3

#define RED_PORT P2
#define RED_PIN BIT7
#define RED_INST 4

#define DOOR_C_PORT P5
#define DOOR_C_PIN BIT4

#define DOOR_O_PORT P5
#define DOOR_O_PIN BIT5

#define BUT_PORT P3
#define BUT_PIN BIT2

#define MAXBRIGHT 7499

void butPres(void);

void initLEDpins(void);
void initTimerALED(void);

void initGreenPWM(void);
void setGreenPWM(uint16_t bright);
void offGreenPWM(void);

void initBluePWM(void);
void setBluePWM(uint16_t bright);
void offBluePWM(void);

void initRedPWM(void);
void setRedPWM(uint16_t bright);
void offRedPWM(void);

#endif
