/***********************************************************************************************************************
* File Name    : trunk_task.c
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
#include "trunk_task.h"
#include "wpc_application.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct TRUNK_STATUS_FLAG Trunk_Status;
unsigned int Trunk_State_Change_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
//unsigned int *Trunk_State_Flag_address;
volatile unsigned char *trunk_state_flag_address;
/***********************************************************************************************************************
* Function Name: Clear_Trunk_State_Flag
* Description  : This function is Clear_Trunk_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_Trunk_State_Flag(unsigned long const *address)
{
	trunk_state_flag_address = address;
	*trunk_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: Trunk_En_Detection
* Description  : This function is Trunk_En_Detection function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Trunk_En_Detection(void)
{
	if(Trunk_Status.Trunk_Open_Flag==TRUE)
	{
		if(TRUNK_EN==CLOSE)
		{
			if(Trunk_State_Change_Time!=CLEAR)
			{
				Trunk_State_Change_Time--;	
			}
			else
			{
				Trunk_State_Change_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
				Trunk_Status.Trunk_Open_Flag = FALSE;
			}
		}
		else
		{
			if(Trunk_State_Change_Time!=DOOR_TRUNK_DEBOUNCE_TIME_200ms+50)
			{
				Trunk_State_Change_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
			}
		}
	}
	else
	{
		if(TRUNK_EN==OPEN)
		{
			if(Trunk_State_Change_Time!=CLEAR)
			{
				Trunk_State_Change_Time--;	
			}
			else
			{
				Trunk_State_Change_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
				Trunk_Status.Trunk_Open_Flag = TRUE;
			}
		}
		else
		{
			if(Trunk_State_Change_Time!=DOOR_TRUNK_DEBOUNCE_TIME_200ms+50)
			{
				Trunk_State_Change_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
			}
		}
	}
}
