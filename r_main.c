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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/F13 V2.03.04.01 [17 May 2019]
* Device(s)    : R5F10BAG
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2020/4/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
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
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
struct WCHC01_SYSTEM_FLAG WCHC01_SYS;
volatile unsigned char *Flag_Group_Address;
volatile unsigned int *WPC_System_Address;
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
	#ifdef _P9261_PROGRAMMING_MODE_
	#else
		WPC_Function_Status.IDT_Program_Debug_Mode_Flag = FALSE;
	#endif
	//P9261_LED1 = 0;
	TEST_TP2 = 0;
	TEST_TP3 = 0;
	TEST_TP4 = 0;
	TEST_TP5 = 0;
	TEST_TP6 = 0;
    while (1U)
    {	
		if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==TRUE)
		{
			if(CHARGE_EN==LEVEL_LOW)	{CHARGE_ENABLE;}
			IDT_Monitor_Mode_LED();
		}
		else
		{
			if(WCHC01_SYS.INITIAL_END_FLAG==TRUE)
			{
				WPC_Processer_Task();
				Abnormal_Alarm_Output_Event();
				//Error_LED_Alarm_Event();
        		//Error_Buzzer_Alarm_Event();
				//PhoneForger_Buzzer_Alarm_Event();
			}
			else
			{
				MCU_IO_Status_Init();
			}
		}
		
		
		
		/*if(WPC_Function_Status.IDT_Program_Debug_Mode_Flag==TRUE)
		{
			if(CHARGE_EN==LEVEL_LOW)	{CHARGE_ENABLE;}
			IDT_Monitor_Mode_LED();
		}
		else
		{	
			IDT_WPC_Processer_Task();
			Error_LED_Alarm_Event();
        	Error_Buzzer_Alarm_Event();
			PhoneForger_Buzzer_Alarm_Event();
		}*/
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	R_TAU0_Channel0_Start();
	R_TAU0_Channel1_Start();
	R_TAU1_Channel0_Start();
	R_TAU1_Channel1_Start();
	WPC_System_Initial();
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: Clear_WPC_Function_State_Flag
* Description  : This function is Clear_WPC_Function_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_Flag_Group(unsigned long const *address)
{
	Flag_Group_Address = address;
	*Flag_Group_Address = CLEAR;
}
unsigned char Mapping_WPC_System_Address(unsigned long *address)
{
	WPC_System_Address = address;
}
/* End user code. Do not edit comment generated here */
