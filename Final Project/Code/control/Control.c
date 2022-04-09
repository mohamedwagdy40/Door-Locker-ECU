 /******************************************************************************
 *
 * File Name: Control.c
 *
 * Description: Source file for the Control ECU main function
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/

#include "control.h"



/**********************************Functions Defections*************************************************/
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
uint8 checkPass(uint8* pass)
{
	uint8 val,index=0;
	for(index = 0 ; index < PASSWORDDIGITS ; index++)                 //compare the passed pass with saved pass in External EEPROM
	{
		EEPROM_readByte(0x0200+index, &val);
		_delay_ms(10);                              //delay 10 millisecond between every 2 write byte instruction in External EEPROM(EEPROM data sheet)
		if(val != pass[index])                      //if one digit not correct return 0
		{
			return 0;
		}
	}
	return 1;                                       //if matched return 1
}

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
uint8 receivePass(uint8 *pass)
{
	uint8 index=0;                                   //index variable to loop for array
	for(index = 0 ; index < PASSWORDDIGITS ; index++)                 //receive the password in an array
	{
		pass[index] = UART_ReceiveByte();
	}
	if(checkPass(pass))                             //if pass is correct
	{
		UART_SendByte(CORRECT);                    //send to the slave correct macro by UART
		return 1;                                 //and return 1
	}
	else                                         //if pass is not correct
	{
		UART_SendByte(FALSE);                   //send to the slave false macro by UART
		return 0;                               //and return 0
	}

}
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : openDoor
 * [Description] : This function open the door for 10 seconds then close the door for ten seconds
 * [Args] : No Arguments
 * [in]
 * [Returns] : No Returns
 *
 *
 */
void openDoor(void)
{
	//run the DC motor clockWise for 10 seconds
	SET_BIT(PORTD,PD4);
	CLEAR_BIT(PORTD,PD5);
	_delay_ms(1100);
	//run the DC motor Anti clockWise for 10 seconds
	SET_BIT(PORTD,PD5);
	CLEAR_BIT(PORTD,PD4);
	_delay_ms(1100);
	//turn off the DC motor
	CLEAR_BIT(PORTD,PD5);
}
/**************************************Main Function***************************************/
int main()
{
	uint8 pass[10],pass2[10],index,flag=0,value,command,counter=0;
	/*Structure holds the configuration for UART Module*/
	UART_configType uart_config = {Asynchronous,Eight,Disabled,9600};
	/*initialization for UART Module*/
	UART_init(&uart_config);
	/*initialization for EEPROM Module*/
	EEPROM_init();
	/* make PD4,PD5 as output pins for motor*/
	SET_BIT(DDRD,PD4);
	SET_BIT(DDRD,PD5);
	/* make PD6 as output pin for buzzer*/
	SET_BIT(DDRD,PD6);
	/*if you need to reset the application and set the pass as first time ever then -> un comment next 2 lines*/
	//EEPROM_writeByte(0x0100,0);
	//_delay_ms(10);

	//read byte from address 0x0100 in External EEPROM
	EEPROM_readByte(0x0100,&value);
	//And if this byte holds zero then this is the first time ever this Application runs and ask the user for the password
	if(value == 0)
	{
		flag = 0;
		//Send to the slave FIRSTTIME Macro to indicate the first time
		UART_SendByte(FIRSTTIME);
		//if password confirmation dosen't match ask the user to enter the password again until 2 passwords matched
		while(UART_ReceiveByte() == AGAIN)
		//get the password from slave and save it in external EEPROM for the first time
		receivePass(pass);
		//confirm the password
		receivePass(pass2);
		//check the 2 passes is matched
		for(index = 0 ; index < PASSWORDDIGITS ; index++)
		{
			if(!(pass[index] == pass2[index]))
				flag =1;
		}
		//if matched save it in EEPROM
		if(!flag)
		{
			for(index = 0 ; index < PASSWORDDIGITS ; index++)
			{
				EEPROM_writeByte(0x0200+index,pass2[index]);
				//delay 10 millisecond between every 2 write byte instruction in External EEPROM(EEPROM data sheet)
				_delay_ms(10);
			}
		}

	}
	//write 1 in address 0x0100 in External EEPROM indicating that is not the first time this app runs
	EEPROM_writeByte(0x0100,1);
	//delay 10 millisecond between every 2 write byte instruction in External EEPROM(EEPROM data sheet)
	_delay_ms(10);
	//read byte from address 0x0100 in External EEPROM
	EEPROM_readByte(0x0100,&value);
	//if this value equal 1 then this not the first time
	if(value == 1)
	{
		//send to the salve SECONDTIME Macro by UART to indicate that is not the first time to go to main screen
		UART_SendByte(SECONDTIME);
		//then go in super loop
		while(1)
		{
			//if password entered 3 times wrong then turn on the buzzer and tell salve to show a thief massage
			if(counter == 3)
			{
				counter = 0;
				SET_BIT(PORTD,PD6);
				// turn on buzzer for 10 seconds
				_delay_ms(1000);
				//turn off the buzzer
				CLEAR_BIT(PORTD,PD6);
			}
			//Receive the command from the slave if open the door or change the password
			command = UART_ReceiveByte();
			//if command open
			if(command == OPEN)
			{
				//check the pass entered
				if(receivePass(pass2))
				{
					//reset the thief counter
					counter = 0;
					//open the door
					openDoor();
				}
				else
					//if entered password wrong increment the thief counter
					counter++;
			}
			//if command change
			else if(command == CHANGE)
			{
				//Receive the pass from the slave
				receivePass(pass2);
				//Receive the command again
				command = UART_ReceiveByte();
				//if master Resends the change command again then the pass is correct
				if(command == CHANGE)
				{
					//reset the thief counter
					counter = 0;
					//Receive the new password
					receivePass(pass2);
					//save the new password in External EEPROM
					for(index = 0 ; index < PASSWORDDIGITS ; index++)
					{
						EEPROM_writeByte(0x0200+index,pass2[index]);
						//delay 10 millisecond between every 2 write byte instruction in External EEPROM(EEPROM data sheet)
						_delay_ms(10);
					}
				}
			}
		}
	}
}
