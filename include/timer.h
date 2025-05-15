/**
 * timer.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/


#ifndef TIMER_H
#define TIMER_H

void timerInit(int valueMilliSec);
void timerDestroy();
void timerUpdateTimer(int valueMilliSec);
int timerTimeOver();
void timerPrint();
void timerDelay(int segundos); 

#endif
