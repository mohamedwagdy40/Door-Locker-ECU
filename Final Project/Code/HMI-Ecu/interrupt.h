 /******************************************************************************
 *
 * Module: Interrupt
 *
 * File Name: interrupt.h
 *
 * Description: Header file for the interrupt driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_


#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/**********************************************user defined variables*********************************************************/
typedef enum{
	INT_1,INT_0,INT_2
}INTNumber;

typedef enum{
	falling=2,rising=3
}Int_edge;

typedef struct{
	INTNumber number;
	Int_edge edge;
}INT_configType;

/*********************************************prototypes for Interrupt 0*****************************************************************/
void Interrupt_init(INT_configType* INT_configTypePtr);
void INT0_setEdge(Int_edge edge);
void INT1_setEdge(Int_edge edge);
void INT2_setEdge(Int_edge edge);
void INT0_setCallBack(void(*a_ptr)(void));
void INT1_setCallBack(void(*a_ptr)(void));
void INT2_setCallBack(void(*a_ptr)(void));
void INT_deinit(INTNumber number);


#endif /* INTERRUPT_H_ */
