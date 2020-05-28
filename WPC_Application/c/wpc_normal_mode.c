/***********************************************************************************************************************
* File Name    : wpc_normal_mode.c
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
unsigned char WPC_Normal_Process(void);
/***********************************************************************************************************************
* Function Name: WPC_Normal_Task
* Description  : This function is WPC_Normal_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Normal_Task(void)
{
	if(WPC_System.Normal_Mode==TRUE)
	{
		WPC_System.Normal_Mode = FALSE;
		Last_Charge_Load_On_Tx = CLEAR;
		CHARGE_ENABLE;
		//TEST_TP3 = 1;
		P9261_IIC_State.active_flag = TRUE;
		KEYBOARD_PWR_SW_GREEN_LED_ON;
		WPC_MODE_CASE = WPC_NORMAL_MODE;
	}
	WPC_MODE_CASE = WPC_Normal_Process();
}
/***********************************************************************************************************************
* Function Name: WPC_Normal_Task
* Description  : This function is WPC_Normal_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char WPC_Normal_Process(void)
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				if(WPC_Function_Status.P9261_ReStart_End_Flag==TRUE)
				{
					//TEST_TP4 = 1;
					IDT_WPC_Processer_Task();	// IDT P9261 power charge processer //
				}
				WPC_NEXT_MODE_CASE = WPC_NORMAL_MODE;
			}
			else
			{
				//TEST_TP3 = 0;
				//TEST_TP4 = 0;
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
			//TEST_TP3 = 0;
			//TEST_TP4 = 0;
			//*WPC_System_Address = SLEEP_MODE;
			WPC_System.Shutdown_Mode = FALSE;
			WPC_System.Normal_Mode = FALSE;
			WPC_System.Sleep_Mode = TRUE;
			WPC_System.EUT_Sleep_Mode = FALSE;
			WPC_NEXT_MODE_CASE = WPC_SLEEP_MODE;
		}
	}
	else
	{
		//TEST_TP3 = 0;
		//TEST_TP4 = 0;
		//*WPC_System_Address = SHUTDOWN_MODE;
		WPC_System.Shutdown_Mode = TRUE;
		WPC_System.Normal_Mode = FALSE;
		WPC_System.Sleep_Mode = FALSE;
		WPC_System.EUT_Sleep_Mode = FALSE;
		WPC_NEXT_MODE_CASE = WPC_SHUTDOWN_MODE;
	}
	return WPC_NEXT_MODE_CASE;
}

