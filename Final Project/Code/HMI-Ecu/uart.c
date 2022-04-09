 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART driver
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/


#include "uart.h"

void UART_init(UART_configType* UART_configTypePtr){
	UCSRA = (1<<U2X);                         //double speed mode
	UCSRB = (1<<RXEN) | (1<<TXEN) ;          //Receiver and Transmitter enable
	UCSRC = (1<<URSEL);                     //select bit for write and read from UCSRC
	if(UART_configTypePtr->type)
		SET_BIT(UCSRC,UMSEL);                //synchronous mode
	//parity bit mode(disabled,even or odd)
	UCSRC = (UCSRC & 0xCF) | ( ((UART_configTypePtr->parity)<<4) & 0x30 ) ;
	/*number of bits in one UART frame*/
	UCSRC = (UCSRC & 0xF9) | ( ((UART_configTypePtr->number)<<1) & 0x06 ) ;
	if(UART_configTypePtr->number == Nine)
		SET_BIT(UCSRB,UCSZ2);
	//CLEAR_BIT(UBRRH,URSEL);             //select bit for write and read from UBRRH
	UBRRL =( ( F_CPU/(8UL * UART_configTypePtr->buadRate) ) -1);
	UBRRH = ((( F_CPU/(8UL * UART_configTypePtr->buadRate) ) -1)>>8);
}


void UART_sendByte(uint8 data){
	UDR = data;                            //put data in UART data register
	while(BIT_IS_CLEAR(UCSRA,TXC));        //wait until transition complete
	SET_BIT(UCSRA,TXC);                    //clear the flag
}

void UART_SendByte(uint8 data)
{
	while(UART_receiveByte() != UARTREADY);
	UART_sendByte(UARTREADY);
	UART_sendByte(data);
}

uint8 UART_receiveByte(void){
	while(BIT_IS_CLEAR(UCSRA,RXC));      //wait until receive complete
	if(!( UCSRA & 0x1C ))                //check for errors
	return UDR;
	else
		return 0;
}

uint8 UART_ReceiveByte(void)
{
	//check if master is ready for UART connection
	UART_sendByte(UARTREADY);
	while(UART_receiveByte() != UARTREADY);
	return UART_receiveByte();
}
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_receiveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte();
	}
	Str[i] = '\0';
}
