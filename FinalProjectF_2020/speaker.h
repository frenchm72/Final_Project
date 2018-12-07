#ifndef __SPEAKER_H__
#define __SPEAKER_H__

<<<<<<< HEAD
#define SPEAKER_PORT P5//MACROS FOR THE SPEAKER TO BEEP AND FOR THE PWM
#define SPEAKER_PIN BIT6
#define BEEP_INST 1

#define SMALL_PAUSE 100
#define BIG_PAUSE 500

#define BEEP_PORT P4//TO MAKE A BEEP NOISE PIN
#define BEEP_PIN BIT0

void beep(void);
=======
#define SPEAKER_PORT P5
#define SPEAKER_PIN BIT6
#define BEEP_INST 1
#define SMALL_PAUSE 100
#define BIG_PAUSE 500

#define BEEP_PORT P4
#define BEEP_PIN P0



void timer_A1_config(void);

>>>>>>> branch 'master' of https://github.com/frenchm72/Final_Project.git

#endif
