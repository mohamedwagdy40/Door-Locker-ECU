 /******************************************************************************
 *
 * File Name: HMI.c
 *
 * Description: Source file for the HMI ECU main function
 *
 * Author: Mohamed Wagdy
 *
 *******************************************************************************/



#include "HMI.h"

/*********************************Global Variables*******************************/
/*
 * g_tick         -> global unsigned 8 bit variable holds the number of overflow for timer
 * g_timerCounter -> global unsigned 8 bit variable holds the number of seconds(every 12 tick equal 1 second)
 * g_timerFlag    -> global unsigned 8 bit variable used to hold the code flow until timer finished
 *
 */
uint8 g_tick = 0,g_timerCounter = 0,g_timerFlag = 0;

/********************************Functions Definitions*******************************/
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
void getUserPass(uint8* ptr)
{

	uint8 index=0;
	uint8 digit ;
	LCD_goToRowColumn(1,0);
	digit = KeyPad_getPressedKey();
	while(digit != '=')
	{
		ptr[index] = digit;//keypad response time
		_delay_ms(50);
		LCD_displayCharacter('*');
		index++;
		digit = KeyPad_getPressedKey();
	}
	//ptr[i] = '/';
	//ptr[i+1] = '#';
	LCD_clearScreen();

}

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
uint8 sendPassToMaster(uint8* pass)
{
	uint8 index=0;
	for(index = 0 ; index < PASSWORDDIGITS ; index++)
	{
		UART_SendByte(pass[index]);
	}
	return UART_ReceiveByte();
}
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : timer_handler
 * [Description] : This function handle the timer0 overflow interrupt (call Back Function)
 * [Args] :
 * [in]        No Input
 * [Returns] : No Returns
 *
 *
 */
void timer_handler(void)
{

	g_tick++;
	//after 12 timer overflow(interrupt) equal 1 Second in real time
	if(g_tick == 12)
	{
		g_tick = 0;
		g_timerCounter++;
		LCD_goToRowColumn(1,0);
		//show the time to the user in HMI Screen
		LCD_intgerToString(10-g_timerCounter);
		//after 10 seconds
		if(g_timerCounter == 10)
		{
			//reset the counter
			g_timerCounter = 0;
			//stop the timer(no clock connected)
			Timer_stop(Timer0);
			//clear HMI screen
			LCD_clearScreen();
			//rise flag for resume HMI ECU code flow
			g_timerFlag = 1;
		}
	}

}
/**************************************Main Function***************************************/
int main()
{
	/*
	 *pass[10]  -> array of 10 unsigned 8 bits numbers hold the first entered password by user
	 *pass2[10] ->array of 10 unsigned 8 bits numbers hold the second entered password by user
	 *Timer     -> variable indicate this is the first time Application runs or not
	 *index     -> loop index for array check or array saving
	 *flag      -> used to hold the flow until opening the door process done
	 *counter   ->unsigned 8 bit variable hold Thief counter
	 *
	 */
	uint8 pass[10],pass2[10],Time,index,flag=0,counter=0;
	/*Structure holds the configuration for UART module
	 *
	 * UART Asynchronous Mode
	 * Eight bit Mode transfer
	 * parityBitMode is disabled
	 * 9600 kilobyte per second bit rate
	 */
	UART_configType uart_config = {Asynchronous,Eight,Disabled,9600};
	//function to initialize the LCD
	LCD_init();
	//function to initialize the UART and pass the address of Structure holds the configuration for UART module
	UART_init(&uart_config);
	/*Structure holds the configuration for Timer  module
	 *
	 * use Timer 0
	 * prescaler equal CPU frequency over 265
	 * Normal mode (overflow mode)
	 * output pin (OCO) disabled
	 * compare value = 0
	 * initial value = 0
	 * compare A value = 0
	 * Compare B value = 0
	 */
	Timer_configType timer0_config = {Timer0,FCPU256,Normal,disconnected,0,0,0,0};
	//function to initialize the Timer and pass the address of Structure holds the configuration for Timer module
	Timer_init(&timer0_config);
	/*function set the call back function
	 * every interrupt the CPU will execute (timer_handler) function
	 */
	Timer_setCallBack(Timer0,Normal,timer_handler);
	//enable I-bit in Status register(global interrupt enable)
	SREG |= (1<<7);
	//check from the master is this first time this Application runs or not
	Time = UART_ReceiveByte();
	//if master told HMI ECU that us the first time ever app runs
	if(Time == FIRSTTIME)
	{
		//ask the user to enter the new password
		do
		{
			UART_SendByte(AGAIN);
			flag = 0;
			LCD_displayString("Enter New Pass");
			getUserPass(pass);

			//send the pass to the master
			sendPassToMaster(pass);
			//confirm the pass
			LCD_displayString("Re Enter Pass");
			getUserPass(pass2);
			//if confirm password failed
			for(index = 0 ; index < PASSWORDDIGITS ; index++)
			{
				if(!(pass[index] == pass2[index]))
					flag =1;
			}
			//re ask the user again for the new password
		}while(flag);
		//if new password entered correctly send to master it is done
		UART_SendByte(DONE);
		//and send the pass to master to save it
		sendPassToMaster(pass);
		/*
		 * if master runs for the first time then send this is the first time
		 * and save the password
		 * master will send this is second time
		 * and slave must response to the UART request or Control ECU(master) will stuck and wait for the response
		 */
		Time = UART_ReceiveByte();

	}
		//super loop(infinity loop)
		while(1){

			//if thief counter reached 3 make a noise by buzzer and wait for 10 second
			if(counter == 3)
			{
				//reset the counter to count three wrong password entry again
				counter = 0;
				LCD_clearScreen();
				LCD_displayString("You are a thief");
				LCD_goToRowColumn(1,0);
				LCD_displayString("I will call 911");
				//use delay not timer because we need to stuck the HMI
				_delay_ms(1000);
			}
			//clear LCD screen
			LCD_clearScreen();
			/*Main Screen
			 * ask user to enter + for change the password
			 * or - for open the door the close it
			 */
			LCD_displayString("+ : Change pass");
			LCD_goToRowColumn(1,0);
			LCD_displayString("- : Open door");
			/*get the user interact with keypad
			 *if user enter -
			 */
			if(KeyPad_getPressedKey() == '-')
			{
				//send to the master to open the door but the master will wait until password entered correctly
				UART_SendByte(OPEN);
				LCD_clearScreen();
				//ask the user to enter the password
				LCD_displayString("Enter Pass");
				getUserPass(pass);
				//if master response that entered password is correct
				if(sendPassToMaster(pass) == CORRECT)
				{
					//reset the password array for the next time
					for(index = 0 ; index < PASSWORDDIGITS ; index++)
					{
						pass[index] = 0;
					}
					//reset the thief counter
					counter = 0;
					LCD_displayString("Opening");
					//start timer 0 for 10 seconds and show this counter on LCD
					Timer_start(&timer0_config);
					//wait until timer finish
					while(!g_timerFlag);
					//reset the flag for next timer
					g_timerFlag = 0;
					//re process same sequence but now is closing the door
					LCD_displayString("Closing");
					Timer_start(&timer0_config);
					while(!g_timerFlag);
					g_timerFlag = 0;
				}
				//if password entered wrong increment thief counter
				else
				{
					counter++;
				}

			}
			//if user entered +
			else if(KeyPad_getPressedKey() == '+')
			{
				//send to master change command by UART
				UART_SendByte(CHANGE);
				LCD_clearScreen();
				//ask the user to enter the old password
				LCD_displayString("Enter old Pass");
				getUserPass(pass);
				//if password is correct
				if(sendPassToMaster(pass) == CORRECT)
				{
					//send change again to make master change the password saved in external EEPROM
					UART_SendByte(CHANGE);
					//enter the new pass
					LCD_displayString("Enter new Pass");
					getUserPass(pass);
					//send the new pass to master
					sendPassToMaster(pass);
					//reset the password array for the next time
					for(index = 0 ; index < PASSWORDDIGITS ; index++)
					{
						pass[index] = 0;
					}
					//reset the thief counter
					counter = 0;
					//tell the user that password changed
					LCD_displayString("CHANGED");
					//wait 100 millisecond to make CHANGED word seen able
					_delay_ms(100);
				}
			}
		}
}
