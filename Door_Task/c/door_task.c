/***********************************************************************************************************************
* File Name    : door_task.c
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
#include "door_task.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct DOOR_STATUS_FLAG Door_Status;
unsigned char Door_Open_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
unsigned char Door_Close_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
//unsigned int *Door_State_Flag_address;
volatile unsigned char *door_state_flag_address;
/***********************************************************************************************************************
* Function Name: Clear_Door_State_Flag
* Description  : This function is Clear_Door_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_Door_State_Flag(unsigned long const *address)
{
	door_state_flag_address = address;
	*door_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: Door_En_Detection
* Description  : This function is Door_En_Detection function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Door_En_Detection(void)
{
	if(Door_Status.Last_Door_State_Chk_Flag==FALSE)
	{
		if(DOOR_EN==LEVEL_LOW)	// detect door close keep time //
		{
			Door_Open_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
			if(Door_Close_Keep_Time!=CLEAR)
			{
				Door_Close_Keep_Time--;
			}
			else
			{
				Door_Close_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
				Door_Status.Last_Door_State_Chk_Flag = TRUE;
				Door_Status.Door_Open_Flag = FALSE;
			}
		}
		else	// detect door open keep time //
		{
			Door_Close_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
			if(Door_Open_Keep_Time!=CLEAR)
			{
				Door_Open_Keep_Time--;
			}
			else
			{
				Door_Open_Keep_Time = DOOR_TRUNK_DEBOUNCE_TIME_200ms+50;
				Door_Status.Last_Door_State_Chk_Flag = TRUE;
				Door_Status.Door_Open_Flag = TRUE;
			}
		}
	}
	else
	{
		if(Door_Status.Door_Open_Flag==TRUE)	// Door open //
		{
			if(DOOR_EN==LEVEL_LOW)
			{
				Door_Status.Door_Open_Flag = FALSE;	
				Door_Status.Last_Door_State_Chk_Flag = FALSE;
			}
		}
		else									// Door close //
		{
			if(DOOR_EN==LEVEL_HIGH)
			{
				Door_Status.Door_Open_Flag = FALSE;	
				Door_Status.Last_Door_State_Chk_Flag = FALSE;
			}
		}
	}
}


