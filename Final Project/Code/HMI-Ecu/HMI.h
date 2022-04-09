/*
 * HMI.h
 *
 *  Created on: Oct 2, 2020
 *      Author: Mohamed Wagdy
 */

#ifndef HMI_H_
#define HMI_H_

/*******************************************Preprocessor Macros **************************************/
#define UARTREADY 'R'
#define CORRECT '1'
#define FALSE '0'
#define PASSWORDDIGITS 4
#define FIRSTTIME '#'
#define SECONDTIME '*'
#define DONE ';'
#define CHANGE 'c'
#define OPEN 'o'
#define AGAIN 'A'
/****************************************Modules Header Files Include**********************************/
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include "uart.h"
/********************************************Functions Prototypes******************************************/
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : getUserPass
 * [Description] : This function interact with the user and take the password form keypad
 * [Args] :
 * [in] uint8*ptr :
 *                    pointer to unsigned 8 bit variable hold first address of array hold the password
 * [Returns] : No Returns
 *
 *
 */
void getUserPass(uint8* ptr);



/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : sendPassToMaster
 * [Description] : This function send the password to the master and return the byte received by UART
 * [Args] :
 * [in] uint8*pass :
 *                    pointer to unsigned 8 bit variable hold first address of array hold the password
 * [Returns] :
 *                    Return 1 if password correct
 *                    return 0 if password is not correct
 *
 *
 */
uint8 sendPassToMaster(uint8* pass);



/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : timer_handler
 * [Description] : This function handle the timer0 overflow interrupt (call Back Function)
 * [Args] :
 * [in]        No Input
 * [Returns] : No Returns
 *
 *
 */
void timer_handler(void);
#endif /* HMI_H_ */
