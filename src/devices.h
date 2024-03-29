#ifndef _DEVICES_
#define _DEVICES_
#include<stdint.h>

void fileOut(uint8_t data);
uint8_t fileIn(void);
uint8_t fileState(void);
void fileCtl(uint8_t data);
int stopTimer();
void setTimer(uint8_t data);
uint8_t getTimer(void);
void termInit();
void termQuit();
void termOutputChar(uint8_t data);
uint8_t termReadChar(void);

#endif /*_DEVICES_*/
