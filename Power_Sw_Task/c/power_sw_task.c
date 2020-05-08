/***********************************************************************************************************************
* File Name    : power_sw_task.c
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
#include "power_sw_task.h"
#include "led_task.h"
#include "wpc_application.h"
#include "acc_task.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct POWER_SW_FLAG	Power_Sw;
unsigned int Power_Sw_Hold_Time;
//unsigned int *Power_Sw_State_Flag_address;
volatile unsigned char *power_sw_state_flag_address;
/***********************************************************************************************************************
* Function Name: Clear_Power_Sw_State_Flag
* Description  : This function is Clear_Power_Sw_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_Power_Sw_State_Flag(unsigned long const *address)
{
	power_sw_state_flag_address = address;
	*power_sw_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: Power_Sw_Detection
* Description  : This function is Power_Sw_Detection function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Power_Sw_Detection(void)
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(POWER_SW!=Power_Sw.Last_Edge_State)
		{
			
			//#ifdef _WPC_PCBA_VER_1_2_
				if(POWER_SW==LEVEL_HIGH)		{Power_Sw.Negative_Edge_State = TRUE;}
				else if(POWER_SW==LEVEL_LOW)	{Power_Sw.Negative_Edge_State = FALSE;}
			/*#else
				if(POWER_SW==LEVEL_LOW)			{Power_Sw.Negative_Edge_State = TRUE;}
				else if(POWER_SW==LEVEL_HIGH)	{Power_Sw.Negative_Edge_State = FALSE;}
			#endif*/
		}
		else
		{
			if(Power_Sw.Negative_Edge_State==TRUE)
			{
				if(Power_Sw_Hold_Time<POWER_SW_HOLD_MAX_TIME)	//100ms
				{
					Power_Sw_Hold_Time++;
					if((Power_Sw_Hold_Time>=POWER_SW_PRESS_TIME)&&(Power_Sw_Hold_Time<POWER_SW_HOLD_MAX_TIME))
					{
						if(Power_Sw.Negative_Edge_Detect_End_Flag==FALSE)
						{
							Power_Sw.Negative_Edge_Detect_End_Flag = TRUE;	
						}
					}
				}
				else
				{
					Power_Sw.Low_Level_Hold_Flag = TRUE;
					//----------------------------------------------//
					if(Power_Sw_Hold_Time<HOLD_OVERTIME) // HOLD_OVERTIME = 60sec //
					{
						Power_Sw_Hold_Time++;
						//----- for Qi test user code ---------------------------------//
						#ifdef _P9261_PROGRAMMING_MODE_
							if(Power_Sw_Hold_Time>HOLD_5S_TIME)
							{
								WPC_Function_Status.IDT_Program_Debug_Mode_Flag = TRUE;	
							}
						#else
						#endif
						//----- for Qi test user code ---------------------------------//
					}
					else
					{
						if(Power_Sw.Press_Key_Overtime_Flag==FALSE)
						{
							Power_Sw.Press_Key_Overtime_Flag = TRUE;
						}
					}
					//----------------------------------------------//
				}
			}
			else
			{
				if((Power_Sw.Negative_Edge_Detect_End_Flag==TRUE)&&(Power_Sw.Low_Level_Hold_Flag==TRUE))
				{		
					if(Power_Sw.Press_Key_Overtime_Flag==FALSE)
					{
						#ifdef _P9261_PROGRAMMING_MODE_
							if(Power_Sw_Hold_Time<HOLD_5S_TIME)
							{
								Power_Sw.Active_Flag = !Power_Sw.Active_Flag;
							}
							else
							{
								WPC_Function_Status.IDT_Program_Debug_Mode_Flag = TRUE;
							}
						#else
							Power_Sw.Active_Flag = !Power_Sw.Active_Flag;
						#endif
					}
					else
					{
						Power_Sw.Press_Key_Overtime_Flag = FALSE;
					}
				}
				Power_Sw.Negative_Edge_Detect_End_Flag = FALSE;
				Power_Sw.Low_Level_Hold_Flag = FALSE;
				Power_Sw_Hold_Time = CLEAR;
			}
		}
		Power_Sw.Last_Edge_State = POWER_SW;
	}
}
