#ifndef __TEMP_H__
#define __TEMP_H__

#define ADC_PORT P5
#define ADC_PIN BIT5
#define ADC_INST 0

#define V_REF 3.3
float getTemp(int degree);
void convertTemp(float TEMP);
void ADC14init(void);

#endif
