/***********************************************************************************************************************
* File Name    : led_task.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BAC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_adc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_userdefine.h"
#include "led_task.h"
#include "power_sw_task.h"
#include "wpc_application.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct LED_STATUS_FLAG LED_Status;
unsigned int led_time = CLEAR;
unsigned int LED_On_Time = CLEAR;
unsigned int LED_Off_Time = CLEAR;
unsigned char LED_Flash_Duty = CLEAR;
unsigned int *Led_State_Flag_address;
volatile unsigned char *led_state_flag_address;

unsigned char led_case = 0;
unsigned char next_led_case = 0;
/***********************************************************************************************************************
* Function Name: Clear_Led_State_Flag
* Description  : This function is Clear_Led_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Init_Led_State_Flag(unsigned long const *address,unsigned int flag_set)
{
	led_state_flag_address = address;
	*led_state_flag_address = CLEAR;
	*led_state_flag_address |= flag_set;//0x03;
	//LED_Status.LED_Working_End_Flag = TRUE; //20200303//
	//LED_Status.Alarm_LED_Active_End_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: IDT_Monitor_Mode_LED
* Description  : This function is Alarm_LED_Disable function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_Monitor_Mode_LED(void)
{
	switch(led_case)
	{
		case 0:
				ERROR_LED_ON;
				POWER_SW_RED_LED_ON;
				led_time = ON_250ms;
				led_case = 4;
				next_led_case = 1;
		break;
		case 1:
				CHARGE_LED_ON;//ERROR_LED_ON;//		
				POWER_SW_GREEN_LED_ON;
				led_time = ON_250ms;
				led_case = 4;
				next_led_case = 2;
		break;
		case 2:
				ERROR_LED_ON;
				POWER_SW_RED_LED_ON;
				led_time = ON_250ms;
				led_case = 4;
				next_led_case = 3;
		break;
		case 3:
				CHARGE_LED_ON;	//ERROR_LED_ON;//	
				POWER_SW_GREEN_LED_ON;
				led_time = ON_250ms;
				led_case = 4;
				next_led_case = 0;
		break;
		case 4:
				if(led_time==CLEAR)
				{
					led_case = next_led_case;	
				}
		break;
		default:
		break;
	}
}
/***********************************************************************************************************************
* Function Name: Alarm_LED_Disable
* Description  : This function is Alarm_LED_Disable function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Alarm_LED_Disable(void)
{
	CHARGE_STATE_LED_OFF;
	LED_On_Time = CLEAR;
	LED_Off_Time = CLEAR;
	LED_Flash_Duty = CLEAR;
	Init_Led_State_Flag(&LED_Status,0x03);
}
/***********************************************************************************************************************
* Function Name: Error_LED_Update
* Description  : This function is Error_LED_Update function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_LED_Update(void)
{
	LED_On_Time = ON_600ms;
	LED_Off_Time = OFF_400ms;
}
/***********************************************************************************************************************
* Function Name: Error_LED_Flash_Out
* Description  : This function is Error_LED_Flash_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_LED_Flash_Out(void)
{
	Error_LED_Update();
	LED_Flash_Duty = DUTY_CYCLE_4;
	LED_Status.Error_LED_Trigger_Flag = FALSE;
	LED_Status.Alarm_LED_Active_End_Flag = FALSE;
	LED_Status.LED_Working_End_Flag = FALSE; //20200303//
	LED_Status.Alarm_LED_En_Flag = TRUE;
	LED_Status.Alarm_LED_Active_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: Rx_OverTemperature_LED_Flash_Out
* Description  : This function is Rx_OverTemperature_LED_Flash_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Rx_OverTemperature_LED_Flash_Out(void)
{
	Error_LED_Update();
	LED_Status.Rx_OverTemp_LED_Trigger_Flag = FALSE;
	LED_Status.Warring_LED_Active_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: Error_LED_Flash_Task
* Description  : This function is Error_LED_Flash_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_LED_Flash_Task(void)
{
	if(LED_Flash_Duty!=CLEAR)
	{
		if(LED_Status.Alarm_LED_En_Flag==TRUE)
		{
			//TEST_TP6 = 1;
			ERROR_LED_ON;
			LED_On_Time--;
			if(LED_On_Time==CLEAR)
			{
				LED_Status.Alarm_LED_En_Flag = FALSE;
			}
		}
		else
		{
			//TEST_TP6 = 0;
			ERROR_LED_OFF;
			LED_Off_Time--;
			if(LED_Off_Time==CLEAR)
			{
				LED_Status.Alarm_LED_En_Flag = TRUE;
				Error_LED_Update();
				LED_Flash_Duty--;
			}
		}
	}
	else
	{
		CHARGE_STATE_LED_OFF;
		Init_Led_State_Flag(&LED_Status,0x03);
	}
}
/***********************************************************************************************************************
* Function Name: Rx_OverTemperature_LED_Alarm_Event
* Description  : This function is Rx_OverTemperature_LED_Alarm_Event function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Rx_OverTemperature_LED_Alarm_Task(void)
{
	if(LED_Status.Alarm_LED_En_Flag==TRUE)
	{
		CHARGE_LED_ON;
		LED_On_Time--;
		if(LED_On_Time==CLEAR)
		{
			LED_Status.Alarm_LED_En_Flag = FALSE;
		}
	}
	else
	{
		CHARGE_LED_OFF;
		LED_Off_Time--;
		if(LED_Off_Time==CLEAR)
		{
			LED_Status.Alarm_LED_En_Flag = TRUE;
			Error_LED_Update();
		}
	}
}
/***********************************************************************************************************************
* Function Name: Error_LED_Alarm_Event
* Description  : This function is Error_LED_Alarm_Event function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_LED_Alarm_Event(void)
{
	/*if(LED_Status.Error_LED_Trigger_Flag==TRUE)
	{
		if(LED_Status.Alarm_LED_Active_End_Flag==TRUE)
		{
			Error_LED_Flash_Out();
		}
	}
	else if(LED_Status.Rx_OverTemp_LED_Trigger_Flag==TRUE)
	{
		Rx_OverTemperature_LED_Flash_Out();
	}*/
}





