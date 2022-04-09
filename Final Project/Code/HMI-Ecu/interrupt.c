 /******************************************************************************
 *
 * Module: Interrupt
 *
 * File Name: interrupt.c
 *
 * Description: Source file for the Interrupt driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/


#include "interrupt.h"

static volatile void (*g_INT0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_INT1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_INT2_callBackPtr)(void) = NULL_PTR;

ISR(INT0_vect)
{
	if(g_INT0_callBackPtr != NULL_PTR)
	{
		(*g_INT0_callBackPtr)();
	}

}
ISR(INT1_vect)
{
	if(g_INT1_callBackPtr != NULL_PTR)
	{
		(*g_INT1_callBackPtr)();
	}

}
ISR(INT2_vect)
{
	if(g_INT2_callBackPtr != NULL_PTR)
	{
		(*g_INT2_callBackPtr)();
	}

}

void Interrupt_init(INT_configType* INT_configTypePtr){
	GICR = (GICR & (0x2F))|( (INT_configTypePtr->number << 6) & (0xE0)) ;      /*insert the  value in MS 3 bits in GICR register*/
	if(INT_configTypePtr->number == INT_0)
	{
		CLEAR_BIT(DDRD,PD2);                                                 /*set PD2 as input pin*/
		MCUCR = (MCUCR & (0xFC))|(INT_configTypePtr->edge  & (0x03)) ;       /*edge insert in LS 2 bits in MCUCR register*/
	}
	else if(INT_configTypePtr->number == INT_1)
	{
		CLEAR_BIT(DDRD,PD3);                                                        /*set PD3 as input pin*/
		MCUCR = (MCUCR & (0xF3))|(((INT_configTypePtr->edge)<<2)  & (0x0C)) ;       /*edge insert in bit2,3 in MCUCR register*/
	}
	else if(INT_configTypePtr->number == INT_2)
	{
		CLEAR_BIT(DDRB,PB2);                                                  /*set PB2 as input pin*/
		MCUCSR = (MCUCSR & (0xFB))|(INT_configTypePtr->edge  & (0x40)) ;       /*edge insert in  bit 6 in MCUCSR register*/
	}

}

void INT0_setEdge(Int_edge edge){
 MCUCR = (MCUCR & (0xFC))|(edge  & (0x03)) ;       /*edge insert in LS 2 bits in MCUCR register*/

}

void INT1_setEdge(Int_edge edge){
	MCUCR |= (MCUCR & (0xFC))|( (edge<<2)  & (0x03)) ;       /*edge insert in bit2,3 in MCUCR register*/
}


void INT2_setEdge(Int_edge edge){
	MCUCSR |= (MCUCSR & (0xBF))|( (edge-2)<<6  & (0x40)) ;       /*edge insert in bit6 in MCUCSR register*/
}


void INT0_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_INT0_callBackPtr = a_ptr;
}
void INT1_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_INT1_callBackPtr = a_ptr;
}
void INT2_setCallBack(void(*a_ptr)(void)){
	/* Save the address of the Call back function in a global variable */
	g_INT2_callBackPtr = a_ptr;
}

void INT_deinit(INTNumber number)
{
	if(number == INT_0)
	{
		CLEAR_BIT(GICR,INT0);
		GICR &= ~ ((1<<ISC00) | (1<<ISC01));
	}
	else if(number == INT_1)
	{
		CLEAR_BIT(GICR,INT1);
		GICR &= ~ ((1<<ISC10) | (1<<ISC11));
	}
	else if(number == INT_2)
	{
		CLEAR_BIT(GICR,INT2);
		CLEAR_BIT(MCUCSR,ISC2);
	}
}
