#ifndef __SERIAL_H__
#define __SERIAL_H__

//char terminal[100];//to print invalid or valid
//int validFlag = 0;//to set a flag if its invalid or valid

#define BUFFER_SIZE 100
char INPUT_BUFFER[BUFFER_SIZE];

void writeOutput(char *string); // write output characters to the serial port
void readInput(char* string); // read input characters from INPUT_BUFFER that are valid
void setupP2(); // Sets up P1.0 as an output to drive the on board LED
void setupSerial(); // Sets up serial for use and enables interrupts

#endif
