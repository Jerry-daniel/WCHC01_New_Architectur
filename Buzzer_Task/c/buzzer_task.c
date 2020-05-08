/***********************************************************************************************************************
* File Name    : buzzer_task.c
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
#include "buzzer_task.h"
#include "wpc_application.h"
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
struct BUZZER_STATUS_FLAG Buzzer_State;
unsigned int Buzzer_On_Time = CLEAR;
unsigned int Buzzer_Off_Time = CLEAR;
unsigned int Buzzer_Cycle_Delay_Time = DELAY_2S;
unsigned char Buzzer_Active_Cycle_Time = CLEAR;
unsigned int *Buzzer_State_Flag_address;
volatile unsigned int *buzzer_state_flag_address;
/***********************************************************************************************************************
* Function Name: Clear_Buzzer_State_Flag
* Description  : This function is Clear_Buzzer_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Init_Buzzer_State_Flag(unsigned long const *address,unsigned int flag_set)
{
	buzzer_state_flag_address = address;
	*buzzer_state_flag_address = CLEAR;
	*buzzer_state_flag_address |= flag_set;//0x07;
	*buzzer_state_flag_address &= 0xFFBF;
	//Buzzer_State.Active_End_Flag = TRUE;
	//Buzzer_State.Buzzer_Working_End_Flag = TRUE;//20200303//
	//Buzzer_State.Cycle_Delay_Start_Flag = FALSE;
	//Buzzer_State.Cycle_Delay_End_Flag = TRUE;
}

/***********************************************************************************************************************
* Function Name: Buzzer_Disable
* Description  : This function is Buzzer_Disable function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Buzzer_Disable(void)
{
	BUZZER = OFF;
	Buzzer_On_Time = CLEAR;
	Buzzer_Off_Time = CLEAR;
	Buzzer_Active_Cycle_Time = CLEAR;
	Init_Buzzer_State_Flag(&Buzzer_State,0x0007);
}
/***********************************************************************************************************************
* Function Name: Charge_Starting_Buzzer_Out
* Description  : This function is Charge_Starting_Buzzer_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Charge_Starting_Buzzer_Out(void)
{
	Buzzer_State.PWM_Go_Flag = TRUE;
	Buzzer_On_Time = ON_200ms;
	Buzzer_State.Charge_Start_Flag = TRUE;
	Buzzer_State.Buzzer_Working_End_Flag = FALSE; //20200304 modify //
}
/***********************************************************************************************************************
* Function Name: Charge_Starting_Buzzer_Task
* Description  : This function is Charge_Starting_Buzzer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Charge_Starting_Buzzer_Task(void)
{
	Buzzer_On_Time--;
	if(Buzzer_On_Time==CLEAR)
	{
		BUZZER = OFF;
		Buzzer_State.PWM_Go_Flag = FALSE;
		Buzzer_State.Charge_Start_Flag = FALSE;
		Buzzer_State.Buzzer_Working_End_Flag = TRUE; //20200304 modify //
	}
}
/***********************************************************************************************************************
* Function Name: Charger_Complete_Buzzer_Out
* Description  : This function is Charger_Complete_Buzzer_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Charger_Complete_Buzzer_Out(void)
{
	Buzzer_State.PWM_Go_Flag = TRUE;
	Buzzer_On_Time = ON_1000ms;
	Buzzer_State.Charge_Complete_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: Charger_Complete_Buzzer_Task
* Description  : This function is Charger_Complete_Buzzer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Charger_Complete_Buzzer_Task(void)
{
	Buzzer_On_Time--;
	if(Buzzer_On_Time==CLEAR)
	{
		BUZZER = OFF;
		Buzzer_State.PWM_Go_Flag = FALSE;
		Buzzer_State.Charge_Complete_Flag = FALSE;
	}
}
/***********************************************************************************************************************
* Function Name: Error_Buzzer_Update
* Description  : This function is Error_Buzzer_Update function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_Buzzer_Update(void)
{
	Buzzer_On_Time = ON_250ms;
	Buzzer_Off_Time = OFF_250ms;
}
/***********************************************************************************************************************
* Function Name: Error_State_Buzzer_Out
* Description  : This function is Error_State_Buzzer_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_State_Buzzer_Out(void)
{
	Error_Buzzer_Update();
	Buzzer_Active_Cycle_Time = DUTY_CYCLE_4;
	Buzzer_State.Error_Buzzer_Trigger_Flag = FALSE; // 20190907 modify//
	Buzzer_State.Active_End_Flag = FALSE;
	Buzzer_State.Buzzer_Working_End_Flag = FALSE;//20200303//
	Buzzer_State.Alarm_En_Flag = TRUE;
	Buzzer_State.Error_Alarm_Active_Flag = TRUE;	
}
/***********************************************************************************************************************
* Function Name: PhoneForger_Buzzer_Update
* Description  : This function is PhoneForger_Buzzer_Update function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void PhoneForger_Buzzer_Update(void)
{
	Buzzer_On_Time = ON_500ms;
	Buzzer_Off_Time = OFF_250ms;
}
/***********************************************************************************************************************
* Function Name: PhoneForget_Buzzer_Out
* Description  : This function is PhoneForget_Buzzer_Out function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void PhoneForget_Buzzer_Out(void)
{
	PhoneForger_Buzzer_Update();
	Buzzer_Active_Cycle_Time = DUTY_CYCLE_5;
	Buzzer_State.Forget_Buzzer_Tigger_Flag = FALSE; // 20190907 modify//
	Buzzer_State.Active_End_Flag = FALSE;
	Buzzer_State.Alarm_En_Flag = TRUE;
	Buzzer_State.Forget_Alarm_Active_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: Alarm_Buzzer_Task
* Description  : This function is Alarm_Buzzer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_Alarm_Buzzer_Task(void)
{
	if(Buzzer_State.Alarm_En_Flag==TRUE)
	{	
		TEST_TP5 = 1;
		Buzzer_State.PWM_Go_Flag = TRUE;
		Buzzer_On_Time--;
		if(Buzzer_On_Time==CLEAR)
		{
			Buzzer_State.Alarm_En_Flag = FALSE;
		}
	}
	else
	{
		TEST_TP5 = 0;
		Buzzer_State.PWM_Go_Flag = FALSE;
		BUZZER = OFF;
		Buzzer_Off_Time--;
		if(Buzzer_Off_Time==CLEAR)
		{
			Error_Buzzer_Update();
			Buzzer_State.Alarm_En_Flag = TRUE;
			Buzzer_Active_Cycle_Time--;
			if(Buzzer_Active_Cycle_Time==CLEAR)
			{
				Buzzer_Disable();
				Buzzer_State.Buzzer_Working_End_Flag = TRUE; //20191016 modify //
				//Buzzer_State.Active_End_Flag = TRUE;
				Buzzer_State.Cycle_Delay_Start_Flag = TRUE;
				Buzzer_State.Cycle_Delay_End_Flag = FALSE;
			}
		}
		
	}
}
/***********************************************************************************************************************
* Function Name: PhoneForger_Buzzer_Task
* Description  : This function is PhoneForger_Buzzer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void PhoneForger_Buzzer_Task(void)
{
	if(Buzzer_State.Alarm_En_Flag==TRUE)
	{		
		Buzzer_State.PWM_Go_Flag = TRUE;
		Buzzer_On_Time--;
		if(Buzzer_On_Time==CLEAR)
		{
			Buzzer_State.Alarm_En_Flag = FALSE;
		}
	}
	else
	{
		Buzzer_State.PWM_Go_Flag = FALSE;
		BUZZER = OFF;
		Buzzer_Off_Time--;
		if(Buzzer_Off_Time==CLEAR)
		{
			PhoneForger_Buzzer_Update();
			Buzzer_State.Alarm_En_Flag = TRUE;
			Buzzer_Active_Cycle_Time--;
			if(Buzzer_Active_Cycle_Time==CLEAR)
			{
				Buzzer_Disable();
				Buzzer_State.Active_End_Flag = TRUE;
			}
		}
	}
}
/***********************************************************************************************************************
* Function Name: Error_Buzzer_Alarm_Event
* Description  : This function is Error_Buzzer_Alarm_Event function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_Buzzer_Alarm_Event(void)
{
	/*if(Buzzer_State.Error_Buzzer_Trigger_Flag==TRUE)
	{
		if(Buzzer_State.Active_End_Flag==TRUE)
		{
			if((Buzzer_State.Cycle_Delay_Start_Flag==FALSE)&&(Buzzer_State.Cycle_Delay_End_Flag==TRUE))
			{
				Error_State_Buzzer_Out();
			}
		}
	}*/
}
/***********************************************************************************************************************
* Function Name: PhoneForger_Buzzer_Alarm_Event
* Description  : This function is PhoneForger_Buzzer_Alarm_Event function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void PhoneForger_Buzzer_Alarm_Event(void)
{
	/*if(Buzzer_State.Forget_Buzzer_Tigger_Flag==TRUE)
	{
		if(Buzzer_State.Active_End_Flag==TRUE)
		{
			PhoneForget_Buzzer_Out();
		}
	}*/
}
/***********************************************************************************************************************
* Function Name: Buzzer_Cycle_Delay
* Description  : This function is Buzzer_Cycle_Delay function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Buzzer_Cycle_Delay(void)
{
	if(Buzzer_State.Cycle_Delay_Start_Flag==TRUE)
	{
		if(Buzzer_Cycle_Delay_Time!=CLEAR)
		{
			Buzzer_Cycle_Delay_Time--;
		}
		else
		{
			Buzzer_Cycle_Delay_Time = DELAY_2S;
			Buzzer_State.Cycle_Delay_Start_Flag = FALSE;
			Buzzer_State.Cycle_Delay_End_Flag = TRUE;
		}
	}
}