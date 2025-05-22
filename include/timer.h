
#ifndef TIMER_H
#define TIMER_H

void timerInit(int valueMilliSec);
void timerDestroy();
void timerUpdateTimer(int valueMilliSec);
int timerTimeOver();
void timerPrint();
void timerDelay(int segundos); 

#endif
