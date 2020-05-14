/***********************************************************************************************************************
* File Name    : wpc_sleep_mode.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BAC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2020/2/27
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
#include "wpc_application.h"
#include "power_sw_task.h"
#include "buzzer_task.h"
#include "led_task.h"
#include "acc_task.h"
#include "door_task.h"
#include "trunk_task.h"
#include "p9261_read_task.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
unsigned char WPC_EUT_Sleep_Process(void);
unsigned char Power_Sw_Status_Process(void);
/***********************************************************************************************************************
Locate variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: WPC_EUT_Sleep_Task
* Description  : This function is WPC_EUT_Sleep_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_EUT_Sleep_Task(void)
{
	if(WPC_System.EUT_Sleep_Mode==TRUE)
	{
		WPC_System.EUT_Sleep_Mode = FALSE;
		Last_Charge_Load_On_Tx = WPC_Function_Status.Charge_Load_In_Tx_Flag;
		CHARGE_DISABLE;
		WPC_Sleep_Reset();
		CHARGE_STATE_LED_OFF;
		WPC_MODE_CASE = WPC_EUT_SLEEP_MODE;
	}
	Power_Sw_Status_Process();
	WPC_MODE_CASE = WPC_EUT_Sleep_Process();
}
/***********************************************************************************************************************
* Function Name: WPC_EUT_Sleep_Process
* Description  : This function is WPC_EUT_Sleep_Process function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char WPC_EUT_Sleep_Process(void)
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				//*WPC_System_Address = NORMAL_MODE;
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = TRUE;
				WPC_System.Sleep_Mode = FALSE;
				WPC_System.EUT_Sleep_Mode = FALSE;
				WPC_NEXT_MODE_CASE = WPC_NORMAL_MODE;
			}
			else
			{		
				WPC_NEXT_MODE_CASE = WPC_EUT_SLEEP_MODE;
			}
		}
		else
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				//*WPC_System_Address = SLEEP_MODE;	
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = FALSE;
				WPC_System.Sleep_Mode = TRUE;
				WPC_System.EUT_Sleep_Mode = FALSE;
				WPC_NEXT_MODE_CASE = WPC_SLEEP_MODE;
			}
			else
			{
				WPC_NEXT_MODE_CASE = WPC_EUT_SLEEP_MODE;
			}
		}
	}
	else
	{
		//*WPC_System_Address = SHUTDOWN_MODE;
		WPC_System.Shutdown_Mode = TRUE;
		WPC_System.Normal_Mode = FALSE;
		WPC_System.Sleep_Mode = FALSE;
		WPC_System.EUT_Sleep_Mode = FALSE;
		WPC_NEXT_MODE_CASE = WPC_SHUTDOWN_MODE;
	}
	return WPC_NEXT_MODE_CASE;
}
/***********************************************************************************************************************
* Function Name: Power_Sw_Status_Process
* Description  : This function is Power_Sw_Status_Process function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Power_Sw_Status_Process(void)
{
	if(Power_Sw.Active_Flag==TRUE)
	{
		if((POWER_OFF_IDN == 1)&&(POWER_SW_LED1 == 0))
		{
			KEYBOARD_PWR_SW_GREEN_LED_ON;
		}
	}
	else
	{
		if((POWER_OFF_IDN == 0)&&(POWER_SW_LED1 == 1))
		{
			KEYBOARD_PWR_SW_RED_LED_ON;
		}
	}
}
