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

/***********************************************************************************************************************
Locate variables and functions
***********************************************************************************************************************/
unsigned char WPC_Sleep_Process(void);
/***********************************************************************************************************************
* Function Name: WPC_Sleep_Task
* Description  : This function is WPC_Sleep_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Sleep_Task(void)
{
	if(WPC_System.Sleep_Mode==TRUE)
	{
		WPC_System.Sleep_Mode = FALSE;
		CHARGE_DISABLE;
		//TEST_TP3 = 0;
		WPC_Sleep_Reset();
		KEYBOARD_PWR_SW_RED_LED_ON;	
		CHARGE_STATE_LED_OFF;
		WPC_MODE_CASE = WPC_SLEEP_MODE;
	}
	WPC_MODE_CASE = WPC_Sleep_Process();
}
/***********************************************************************************************************************
* Function Name: WPC_Sleep_Process
* Description  : This function is WPC_Sleep_Process function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char WPC_Sleep_Process(void)
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
				//*WPC_System_Address = EUT_SLEEP_MODE;	
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = FALSE;
				WPC_System.Sleep_Mode = FALSE;
				WPC_System.EUT_Sleep_Mode = TRUE;
				WPC_NEXT_MODE_CASE = WPC_EUT_SLEEP_MODE;
			}
		}
		else
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				WPC_NEXT_MODE_CASE = WPC_SLEEP_MODE;
			}
			else
			{
				//*WPC_System_Address = EUT_SLEEP_MODE;
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = FALSE;
				WPC_System.Sleep_Mode = FALSE;
				WPC_System.EUT_Sleep_Mode = TRUE;
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