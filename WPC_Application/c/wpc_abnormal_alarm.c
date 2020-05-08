/***********************************************************************************************************************
* File Name    : wpc_abnormal_alarm.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BAC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2020/3/3
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
struct WPC_ABNORMAL_ALARM_STATUS_FLAG Abnormal_Event;
void Forget_Remove_Check(void);
void Error_Alarm_Out_Processer(void);
void WPC_OverTemperature_Alarm_Out_Processer(void);
void Rx_OverTemperature_Alarm_Out_Processer(void);
volatile unsigned char *abnormal_event_flag_address;


/***********************************************************************************************************************
* Function Name: Clear_Abnormal_Event_Flag
* Description  : This function is Clear_Abnormal_Event_Flag function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_Abnormal_Event_Flag(unsigned long const *address)
{
	abnormal_event_flag_address = address;
	*abnormal_event_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: Abnormal_Alarm_Output_Event
* Description  : This function is Abnormal_Alarm_Output_Event function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Abnormal_Alarm_Output_Event(void)
{
	//---------- LED -----------------------------------//
	if(LED_Status.Error_LED_Trigger_Flag==TRUE)
	{
		if(LED_Status.Alarm_LED_Active_End_Flag==TRUE)
		{
			Error_LED_Flash_Out();
		}
	}
	else if(LED_Status.Rx_OverTemp_LED_Trigger_Flag==TRUE)
	{
		Rx_OverTemperature_LED_Flash_Out();
	}
	//---------- Error BUZZER -----------------------------------//
	if(Buzzer_State.Error_Buzzer_Trigger_Flag==TRUE)
	{
		if(Buzzer_State.Active_End_Flag==TRUE)
		{
			if((Buzzer_State.Cycle_Delay_Start_Flag==FALSE)&&(Buzzer_State.Cycle_Delay_End_Flag==TRUE))
			{
				Error_State_Buzzer_Out();
			}
		}
	}
	//---------- Phone Forget BUZZER -----------------------------------//
	if(Buzzer_State.Forget_Buzzer_Tigger_Flag==TRUE)
	{
		if(Buzzer_State.Active_End_Flag==TRUE)
		{
			PhoneForget_Buzzer_Out();
		}
	}
}
/***********************************************************************************************************************
* Function Name: Forget_Remove_Check
* Description  : This function is Forget_Remove_Check function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Forget_Remove_Check(void)
{
	if(WPC_Function_Status.Forget_Remove_End_Flag==FALSE)
	{
		if(P9261_CombinedMessage.In_Open_FOD == FALSE)
		{
			Buzzer_Disable();
			Alarm_LED_Disable();
			OpenFOD_Detect_Restart();
			WPC_Function_Status.Hold_Open_FOD_Flag = FALSE;		
			WPC_Function_Status.Forget_Remove_End_Flag = TRUE;
		}
		else
		{
			if(WPC_Function_Status.Hold_Open_FOD_Flag==TRUE)
			{
				//WPC_Function_Status.Hold_Open_FOD_Flag = FALSE;
				//if(WPC_Function_Status.Sync_Capture_Open_FOD_Flag==TRUE)
				//{
					/////Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
					/////TEST_TP4 = 1;
				//}
				if((COIL_1_Q_Message<120)||(COIL_2_Q_Message<120)||(COIL_3_Q_Message<120))
				{
					Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
					TEST_TP4 = 1;
				}
				else
				{
					Buzzer_Disable();
					Alarm_LED_Disable();
					Abnormal_Event.P9261_Error_Alarm_Flag = FALSE;
				}
			}
		}
	}
}
/***********************************************************************************************************************
* Function Name: IDT_FOD_Alarm_Task
* Description  : This function is IDT_FOD_Alarm_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Reset_Buzzer_LED_State(void)
{
	Buzzer_Disable();
	Alarm_LED_Disable();
}
/***********************************************************************************************************************
* Function Name: WPC_Abnormal_Alarm_Task
* Description  : This function is WPC_Abnormal_Alarm_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Abnormal_Alarm_Task(void)
{
	if(Abnormal_Event.P9261_Error_Alarm_Flag==TRUE)
	{
		Error_Alarm_Out_Processer();
	}
	else if(Abnormal_Event.Tx_Extern_OverTemp_Alarm_Flag==TRUE)
	{
		WPC_OverTemperature_Alarm_Out_Processer();
	}
	else if(Abnormal_Event.Rx_OverTemp_Alarm_Flag==TRUE)
	{
		Rx_OverTemperature_Alarm_Out_Processer();
	}
	Forget_Remove_Check();
}
/***********************************************************************************************************************
* Function Name: Error_Alarm_Out_Processer
* Description  : This function is Error_Alarm_Out_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Error_Alarm_Out_Processer(void)
{
	//Abnormal_Event.P9261_Error_Alarm_Flag = FALSE;
	if((Buzzer_State.Buzzer_Working_End_Flag==TRUE)&&(LED_Status.LED_Working_End_Flag==TRUE))
	{
		TEST_TP4 = 0;
		Abnormal_Event.P9261_Error_Alarm_Flag = FALSE;
		CHARGE_STATE_LED_OFF;
		LED_Status.Error_LED_Trigger_Flag = TRUE;
		Buzzer_State.Error_Buzzer_Trigger_Flag = TRUE;
	}
}
/***********************************************************************************************************************
* Function Name: WPC_OverTemperature_Alarm_Out_Processer
* Description  : This function is WPC_OverTemperature_Alarm_Out_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_OverTemperature_Alarm_Out_Processer(void)
{
	if((Buzzer_State.Buzzer_Working_End_Flag==TRUE)&&(LED_Status.LED_Working_End_Flag==TRUE))
	{
		if(Over_Temperature_Reset_Time!=CLEAR)
		{
			CHARGE_STATE_LED_OFF;
			LED_Status.Error_LED_Trigger_Flag = TRUE;
			Buzzer_State.Error_Buzzer_Trigger_Flag = TRUE;
		}
		else
		{
			WPC_Sleep_Reset();
			Reset_Buzzer_LED_State();
			//WPC_System.Normal_Mode = TRUE;
			WPC_System.Shutdown_Mode = FALSE;
			WPC_System.Normal_Mode = TRUE;
			WPC_System.Sleep_Mode = FALSE;
			WPC_System.EUT_Sleep_Mode = FALSE;
			WPC_MODE_CASE = WPC_NORMAL_MODE;
			Abnormal_Event.Tx_Extern_OverTemp_Alarm_Flag = FALSE;
		}
	}
}
/***********************************************************************************************************************
* Function Name: Rx_OverTemperature_Alarm_Out_Processer
* Description  : This function is Rx_OverTemperature_Alarm_Out_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Rx_OverTemperature_Alarm_Out_Processer(void)
{
	CHARGE_STATE_LED_OFF;
	Abnormal_Event.Rx_OverTemp_Alarm_Flag = FALSE;
	LED_Status.Rx_OverTemp_LED_Trigger_Flag = TRUE;
}
