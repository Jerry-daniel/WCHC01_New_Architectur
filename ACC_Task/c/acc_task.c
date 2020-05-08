/***********************************************************************************************************************
* File Name    : acc_task.c
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
#include "acc_task.h"
#include "power_sw_task.h"
#include "wpc_application.h"
//#include "led_task.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct ACC_STATUS_FLAG ACC_Status;
unsigned char Last_ACC_State;
unsigned char ACC_Change_Cnt = CLEAR;
unsigned int ACC_DeBounce_Time;
//extern unsigned int *ACC_State_Flag_address;
volatile unsigned char *acc_state_flag_address;
/***********************************************************************************************************************
* Function Name: Clear_ACC_State_Flag
* Description  : This function is Clear_ACC_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_ACC_State_Flag(unsigned long const *address)
{
	acc_state_flag_address = address;
	*acc_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: ACC_Detection
* Description  : This function is ACC_Detection function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ACC_Detection(void)
{
	if(ACC_Status.Active_On_Flag==FALSE)
	{
		if(ACC_IN==ACC_ON)
		{
			if(ACC_DeBounce_Time!=CLEAR)
			{
				ACC_DeBounce_Time--;
			}
			else
			{
				ACC_Status.Active_On_Flag = TRUE;	// ACC ON //	
				Power_Sw.Active_Flag = TRUE;
				ACC_DeBounce_Time = DEBOUNCE_TIME_200MS+50;
			}
		}
		else
		{
			if((ACC_DeBounce_Time==CLEAR)||(ACC_DeBounce_Time<DEBOUNCE_TIME_200MS+50))
			{
				ACC_DeBounce_Time = DEBOUNCE_TIME_200MS+50;
			}
		}
	}
	else
	{
		if(ACC_IN==ACC_OFF)
		{
			if(ACC_DeBounce_Time!=CLEAR)
			{
				ACC_DeBounce_Time--;
			}
			else
			{
				ACC_Status.Active_On_Flag = FALSE;	// ACC OFF //
				//Power_Sw.Active_Flag = FALSE;
				ACC_DeBounce_Time = DEBOUNCE_TIME_200MS+50;
			}
		}
		else
		{
			if((ACC_DeBounce_Time==CLEAR)||(ACC_DeBounce_Time<DEBOUNCE_TIME_200MS+50))
			{
				ACC_DeBounce_Time = DEBOUNCE_TIME_200MS+50;
			}
		}
	}
	ACC_State_Ccheck();
	Last_ACC_State = ACC_IN;
}
/***********************************************************************************************************************
* Function Name: ACC_State_Ccheck
* Description  : This function is ACC_State_Ccheck function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ACC_State_Ccheck(void)
{
	if((Last_ACC_State==ACC_ON)&&(ACC_IN==ACC_OFF))
	{
		ACC_Status.On_Change_To_Off_Flag = TRUE;
	}
}
