 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the uart driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

//#define ERROR 0x00
#define UARTREADY 'R'

typedef enum{
	Five,Six,Seven,Eight,Nine=7
}NumOfBits;

typedef enum{
	Disabled,Reserved,Even,Odd
}parityBitMode;

typedef enum{
	Asynchronous,synchronous
}communicationType;



typedef struct{
	communicationType type;
	NumOfBits number;
	parityBitMode parity;
	uint16 buadRate;
}UART_configType;



void UART_init(UART_configType* UART_configTypePtr);
void UART_sendByte(uint8 data);
void UART_SendByte(uint8 data);
uint8 UART_receiveByte(void);
uint8 UART_ReceiveByte(void);
void UART_sendString(const uint8 *Str);
void UART_receiveString(uint8 *Str);


#endif /* UART_H_ */
