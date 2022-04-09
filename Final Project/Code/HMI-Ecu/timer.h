 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the timer driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"


/**********************************************user defined variables*********************************************************/
typedef enum{
	NOCLOCK,NOPRESCALING,FCPU8,FCPU64,FCPU256,FCPU1024,EXTERNALCLOCKFALINGEDGE,EXTERNALCLOCKRISINGEDGE
}Clock;

typedef enum{
	Normal,CTC,CTCA,CTCB
}Mode;

typedef enum {
	disconnected , TOGGLE , CLEAR , SET
}Output;

typedef enum{
	Timer0,Timer1,Timer2
}TimerNumber;

typedef struct{
	TimerNumber timer;
	Clock clock;
	Mode mode;
	Output out;
	uint8 compareValue;
	uint16 initTime;
	uint16 compareAValue;
	uint16 compareBValue;
}Timer_configType;



/*********************************************prototypes for timer 0*****************************************************************/
void Timer_init(Timer_configType* ptrTimer_configType);
void Timer_start(Timer_configType* ptrTimer_configType );
void Timer_stop(TimerNumber timer);
void Timer_clearTimerValue(TimerNumber timer);
uint16 Timer_getTimerValue(TimerNumber timer);
void Timer_setCallBack(TimerNumber timer,Mode mode,void(*a_ptr)(void));
void Timer_deinit(TimerNumber timer);
#endif /* TIMER_H_ */
