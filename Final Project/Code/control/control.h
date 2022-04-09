/*
 * control.h
 *
 *  Created on: Oct 4, 2020
 *      Author: Mohamed Wagdy
 */

#ifndef CONTROL_H_
#define CONTROL_H_

/****************************************** Preprocessor Macros **************************************/
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
#include "external_eeprom.h"
#include "i2c.h"
#include "uart.h"
#include "timer.h"
/********************************************Functions Prototypes******************************************/
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : checkPass
 * [Description] : This function compare the password passed in array(pass) with password saved in EEPROM
 * [Args] :
 * [in] uint8*pass :
 *                    pointer to unsigned 8 bit variable hold first address of array hold the password
 * [Returns] uint8:
 *                    return 1 if password correct and matched with saved password
 *                    return 0 if password false if password dosent match
 *
 *
 */
uint8 checkPass(uint8* pass);
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : receivePass
 * [Description] : This function receive password from slave by UART module
 * [Args] :
 * [in] uint8*pass :
 *                    pointer to unsigned 8 bit variable hold first address of array hold the password
 * [Returns] uint8:
 *                    return 1 if password correct and matched with saved password in EEPROM
 *                    return 0 if password false if password dosen't match
 *
 *
 */
uint8 receivePass(uint8 *pass);
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : openDoor
 * [Description] : This function open the door for 10 seconds then close the door for ten seconds
 * [Args] : No Arguments
 * [in]
 * [Returns] : No Returns
 *
 *
 */
void openDoor(void);

#endif /* CONTROL_H_ */
