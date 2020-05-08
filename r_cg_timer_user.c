/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_timer_user.c
* Version      : CodeGenerator for RL78/F13 V2.03.04.01 [17 May 2019]
* Device(s)    : R5F10BAG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for TAU module.
* Creation Date: 2020/4/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
#include "wpc_application.h"
#include "p9261_read_task.h"
#include "acc_task.h"
#include "buzzer_task.h"
#include "led_task.h"
#include "power_sw_task.h"
#include "door_task.h"
#include "trunk_task.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_tau0_channel0_interrupt(vect=INTTM00)
#pragma interrupt r_tau0_channel1_interrupt(vect=INTTM01)
#pragma interrupt r_tau1_channel0_interrupt(vect=INTTM10)
#pragma interrupt r_tau1_channel1_interrupt(vect=INTTM11)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_tau0_channel0_interrupt
* Description  : This function is INTTM00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
//	#ifdef _P9261_PROGRAMMING_MODE_
	//P9261_LED1 = !P9261_LED1;
//	#else
		if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==FALSE)
		{
			ACC_Detection();
			Door_En_Detection();
			Trunk_En_Detection();
			Power_Sw_Detection();	
		}	
//	#endif
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau0_channel1_interrupt
* Description  : This function is INTTM01 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau0_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==FALSE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if(WPC_Function_Status.P9261_ReStart_End_Flag==FALSE)
			{
				P9261_Restart_Init();
			}
		}
		//if(UVOL_Wait_Time!=CLEAR)							{UVOL_Wait_Time--;}
		//if(WPC_Function_Status.EUT_Wait_Time_Flag==TRUE)	{if(EUT_Waitting_Time!=CLEAR){EUT_Waitting_Time--;}}
		if(WPC_Function_Status.ACC_Off_Dly3_5T_Flag==TRUE)	{if(ACC_Off_Reset_Time!=CLEAR){ACC_Off_Reset_Time--;}}
		if(Over_Temperature_Reset_Time!=CLEAR)				{Over_Temperature_Reset_Time--;}
		if(Read_I2C_Data_Duty_Time!=CLEAR)					{Read_I2C_Data_Duty_Time--;}
		if(Read_Next_Register_Delay_Time!=CLEAR)			{Read_Next_Register_Delay_Time--;}
		if(System_Init_Time!=CLEAR)							{System_Init_Time--;}
	}
	else
	{
		if(led_time!=CLEAR)	{led_time--;}
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau1_channel0_interrupt
* Description  : This function is INTTM10 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau1_channel0_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==FALSE)
	{
		if(Buzzer_State.Charge_Start_Flag==TRUE)			{Charge_Starting_Buzzer_Task();}
		if(Buzzer_State.Charge_Complete_Flag==TRUE)			{Charger_Complete_Buzzer_Task();}
		if(Buzzer_State.Error_Alarm_Active_Flag==TRUE)		{Error_Alarm_Buzzer_Task();}
		if(Buzzer_State.Forget_Alarm_Active_Flag==TRUE)		{PhoneForger_Buzzer_Task();}
		if(LED_Status.Alarm_LED_Active_Flag==TRUE)			{Error_LED_Flash_Task();}
		if(LED_Status.Warring_LED_Active_Flag==TRUE)		
		{
			if(P9261_Reg_State.TX_STATUS_MESSAGE==STATUS_over_temperature_rx_ept_code)
			{
				Rx_OverTemperature_LED_Alarm_Task();
			}
			else
			{
				CHARGE_STATE_LED_OFF;
				LED_Status.Warring_LED_Active_Flag = FALSE;
			}
		}
		Buzzer_Cycle_Delay();
	}
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_tau1_channel1_interrupt
* Description  : This function is INTTM11 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_tau1_channel1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
	if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==FALSE)
	{
		if(Buzzer_State.PWM_Go_Flag==TRUE)		{BUZZER = !BUZZER;}//{BUZZER = OFF;}//
	}
	else
	{
		if(BUZZER==ON)	{BUZZER = OFF;}
	}
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
