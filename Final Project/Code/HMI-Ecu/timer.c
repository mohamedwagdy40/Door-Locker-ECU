 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/
#include "timer.h"

static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtrA) (void) = NULL_PTR ;
static volatile void (*g_Timer1_callBackPtrB) (void) = NULL_PTR ;
static volatile void (*g_Timer2_callBackPtr) (void) = NULL_PTR ;

/**********************************************INTERRUPT SERVICES ROUTINES******************************************/
ISR(TIMER0_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}

}
ISR(TIMER0_COMP_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
	}

}


ISR(TIMER1_OVF_vect)
{
	if(g_Timer1_callBackPtrA != NULL_PTR)
	{
		(*g_Timer1_callBackPtrA)();
	}

}
ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1_callBackPtrA != NULL_PTR)
	{
		(*g_Timer1_callBackPtrA)();
	}

}
ISR(TIMER1_COMPB_vect)
{
	if(g_Timer1_callBackPtrB != NULL_PTR)
	{
		(*g_Timer1_callBackPtrB)();
	}

}
ISR(TIMER2_COMP_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}

}
ISR(TIMER2_OVF_vect)
{
	if(g_Timer2_callBackPtr != NULL_PTR)
	{
		(*g_Timer2_callBackPtr)();
	}

}


/**************************************************************FUNCTIONS DEFENTIONS*****************************************/
void Timer_init(Timer_configType* ptrTimer_configType){
	TCCR0 = (1<<FOC0);                                          //Non PWM Mode

	if(ptrTimer_configType->timer == Timer0)
	{
		/*SET MODE FOR TIMER 0*/
		if(ptrTimer_configType->mode == CTC)
			SET_BIT(TCCR0,WGM00);

		//TCCR0 |= (TCCR0 & (0xF8))|( ptrTimer_configType->clock & (0x07)) ;       //pre-scaler insert in LS 3 bits in TCCR0 register

		OCR0 = ptrTimer_configType->compareValue;                              //set compare value in OCR0 timer 0 compare register

		SET_BIT(TIMSK,TOIE0);                                                  //enable timer0 interrupt

		TCNT0 = ptrTimer_configType-> initTime ;
	}

	else if(ptrTimer_configType->timer == Timer1)
	{
		TCCR1A = (1 << FOC1A) | (1 << FOC1B) ;

		TCCR1B = (TCCR1B & 0xF7) | ( (ptrTimer_configType ->mode) << 3 ) ;

		if(ptrTimer_configType->mode == CTCA)
		{
			TCCR1A = (TCCR1A & 0x3F) | ( (ptrTimer_configType ->out) << 6 ) ;
			OCR1A = ptrTimer_configType->compareAValue ;

			if(ptrTimer_configType->out != disconnected)
			{
				SET_BIT(DDRD,PD5);
			}
		}
		else if (ptrTimer_configType->mode == CTCB)
		{
			TCCR1A = (TCCR1A & 0x3F) | ( (ptrTimer_configType ->out) << 4 ) ;
			OCR1B = ptrTimer_configType->compareBValue ;

			if(ptrTimer_configType-> out != disconnected)
			{
				SET_BIT(DDRD,PD4);
			}
		}

		//TCCR1B = (TCCR1B & 0xF8) | ( (ptrTimer_configType ->clock)) ;

		TIMSK = (TIMSK & 0xC3) | (1 << ptrTimer_configType->mode) ;

		TCNT1 = ptrTimer_configType-> initTime ;
	}

	else if(ptrTimer_configType->timer == Timer2)
	{
		/*SET MODE FOR TIMER 0*/
		if(ptrTimer_configType->mode == CTC)
			SET_BIT(TCCR2,WGM00);

		//TCCR2 |= (TCCR2 & (0xF8))|( ptrTimer_configType->clock & (0x07)) ;       //pre-scaler insert in LS 3 bits in TCC register

		OCR2 = ptrTimer_configType->compareValue;                              //set compare value in OCR2 timer 0 compare register

		SET_BIT(TIMSK,TOIE2);                                                  //enable timer2 interrupt

		TCNT2 = ptrTimer_configType-> initTime ;
	}
}
void Timer_start(Timer_configType* ptrTimer_configType )
{
	if(ptrTimer_configType->timer == Timer0)
	{
		TCCR0 |= (TCCR0 & (0xF8))|( (ptrTimer_configType->clock) & (0x07) ) ;       //pre-scaler insert in LS 3 bits in TCCR0 register
	}
	else if(ptrTimer_configType->timer == Timer1)
	{
		TCCR1B = (TCCR1B & 0xF8) | ( (ptrTimer_configType ->clock) & (0x07) ) ;    //pre-scaler insert in LS 3 bits in TCCR1B register
	}
	else if(ptrTimer_configType->timer == Timer2)
	{
		TCCR2 |= (TCCR2 & (0xF8))|( (ptrTimer_configType->clock) & (0x07) ) ;       //pre-scaler insert in LS 3 bits in TCCR2 register
	}
}
void Timer_stop(TimerNumber timer)
{
	if(timer == Timer0)
	{
		/*clear Pre-scaler for timer 0,and clear timer register value*/
		CLEAR_BIT(TCCR0,0);
		CLEAR_BIT(TCCR0,1);
		CLEAR_BIT(TCCR0,2);
		Timer_clearTimerValue(timer);
	}
	else if(timer == Timer1)
	{
		/*clear Pre-scaler for timer 1,and clear timer register value*/
		CLEAR_BIT(TCCR1B,0);
		CLEAR_BIT(TCCR1B,1);
		CLEAR_BIT(TCCR1B,2);
		Timer_clearTimerValue(timer);
	}
	else if(timer == Timer2)
	{
		/*clear Pre-scaler for timer 2,and clear timer register value*/
		CLEAR_BIT(TCCR2,0);
		CLEAR_BIT(TCCR2,1);
		CLEAR_BIT(TCCR2,2);
		Timer_clearTimerValue(timer);
	}
}
void Timer_clearTimerValue(TimerNumber timer){
	if(timer == Timer0)
		TCNT0 = 0;
	else if(timer == Timer1)
			TCNT1= 0 ;
	else if(timer == Timer2)
			TCNT2= 0 ;
}

uint16 Timer_getTimerValue(TimerNumber timer){
	if(timer == Timer0)
		return TCNT0;
	else if(timer == Timer1)
		return TCNT1;
	else
		return TCNT2;
}


void Timer_deinit(TimerNumber timer){
	if(timer == Timer0)
	{
		TCCR0 = 0;
		OCR0 = 0;
		CLEAR_BIT(TIMSK,TOIE0);
	}
	else if(timer == Timer1)
	{
		TCCR1A = 0;
		TCCR1B = 0;
		OCR1A = 0;
		OCR1B = 0;
		TCNT1 = 0;
		CLEAR_BIT(TIMSK,TOIE1);
		CLEAR_BIT(TIMSK,OCIE1B) ;
		CLEAR_BIT(TIMSK,OCIE1A) ;
	}
	else if(timer == Timer2)
	{
		TCCR2 = 0;
		OCR2 = 0;
		CLEAR_BIT(TIMSK,TOIE2);
	}
}

void Timer_setCallBack(TimerNumber timer,Mode mode,void(*a_ptr)(void))
{
	if(timer == Timer0)
	{
		g_Timer0_callBackPtr = a_ptr ;
	}
	else if (timer == Timer1 && (mode == CTCA || mode == Normal)  )
	{
		g_Timer1_callBackPtrA = a_ptr ;
	}
	else if (timer == Timer1 && mode ==CTCB)
	{
		g_Timer1_callBackPtrB = a_ptr ;
	}
	else if (timer == Timer2)
	{
		g_Timer2_callBackPtr = a_ptr ;
	}
}

