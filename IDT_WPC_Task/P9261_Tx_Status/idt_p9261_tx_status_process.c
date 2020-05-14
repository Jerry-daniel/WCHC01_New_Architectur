/***********************************************************************************************************************
* File Name    : idt_p9261_tx_status_process.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file is wpc tx_state process function.
* Creation Date: 2019/9/4
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
/* Start user code for include. Do not edit comment generated here */
#include "wpc_application.h"
#include "acc_task.h"
#include "buzzer_task.h"
#include "led_task.h"
#include "power_sw_task.h"
#include "door_task.h"
#include "trunk_task.h"
#include "p9261_read_task.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"


/***********************************************************************************************************************
variables define
***********************************************************************************************************************/
#ifdef _OPENFOD_LONG_TIME_
	#define FOD_COUNT			3//5
#else
	#define FOD_COUNT			6//3
#endif

#define PLOSS_FOD_COUNT			6
/***********************************************************************************************************************
Locate variables and functions
***********************************************************************************************************************/
void OpenFOD_Alarm_Processer(void);
void PlossFOD_Alarm_Processer(void);
void Tx_Extern_OverTemperature_Alarm_Processer(void);
unsigned char PlossFOD_Count = CLEAR;
/***********************************************************************************************************************
* Function Name: IDT_P9261_TX_Status_Processer
* Description  : This function is IDT_P9261_TX_Status_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_P9261_TX_Status_Processer(void)
{
	if(P9261_Reg_State.TX_STATUS_MESSAGE!=STATUS_ok)
	{
		if(P9261_TxStatus_Message.Q_FOD_Alarm_Status == TRUE)
		{
			P9261_TxStatus_Message.Q_FOD_Alarm_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Charge_Complete_Rx_EPT_Code_Status == TRUE)/////////////////////////
		{
			P9261_TxStatus_Message.Charge_Complete_Rx_EPT_Code_Status = FALSE;
			Abnormal_Event.Rx_Charge_Complete_Alarm_Flag = TRUE;
		}
		/*else if(P9261_TxStatus_Message.No_Response_Rx_EPT_Code_Status == TRUE)
		{
			P9261_TxStatus_Message.No_Response_Rx_EPT_Code_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Internal_Fault_Rx_EPT_Code_Status == TRUE)
		{
			P9261_TxStatus_Message.Internal_Fault_Rx_EPT_Code_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Rx_OverTemperature_EPT_Code_Status == TRUE)
		{
			P9261_TxStatus_Message.Rx_OverTemperature_EPT_Code_Status = FALSE;
			//if(LED_Status.Warring_LED_Active_Flag==FALSE)
			//{
			//	Abnormal_Event.Rx_OverTemp_Alarm_Flag = TRUE;
			//}
		}
		else if(P9261_TxStatus_Message.Rx_OverCurrent_EPT_Code_Status == TRUE)
		{
			P9261_TxStatus_Message.Rx_OverCurrent_EPT_Code_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}*/
		else if(P9261_TxStatus_Message.Full_Chargeed_Status == TRUE)///////////////////////
		{
			P9261_TxStatus_Message.Full_Chargeed_Status = FALSE;
			Abnormal_Event.Rx_Full_Charged_Alarm_Flag = TRUE;
		}
		//----------------------------------------------------------------------------//
		/*
		else if(P9261_TxStatus_Message.Other_End_Of_Power_Rx_EPT_Code_Status == TRUE)
		{
			P9261_TxStatus_Message.Other_End_Of_Power_Rx_EPT_Code_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Negotiation_Failed_Status == TRUE)
		{
			P9261_TxStatus_Message.Negotiation_Failed_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Control_Error_Packet_Timeout_Status == TRUE)
		{
			P9261_TxStatus_Message.Control_Error_Packet_Timeout_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Received_Power_Packet_Timeout_Status == TRUE)
		{
			P9261_TxStatus_Message.Received_Power_Packet_Timeout_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Tnext_Timing_Violation_Status == TRUE)
		{
			P9261_TxStatus_Message.Tnext_Timing_Violation_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Packet_Defect_During_ID_Config_Status == TRUE)
		{
			P9261_TxStatus_Message.Packet_Defect_During_ID_Config_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Illegal_Packet_During_Power_Tx_Status == TRUE)
		{
			P9261_TxStatus_Message.Illegal_Packet_During_Power_Tx_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		
		else if(P9261_TxStatus_Message.Tx_External_OverTemp_Limit_Status == TRUE)
		{
			P9261_TxStatus_Message.Tx_External_OverTemp_Limit_Status = FALSE;
			//Abnormal_Event.Tx_Extern_OverTemp_Alarm_Flag = TRUE;
			Tx_Extern_OverTemperature_Alarm_Processer();
		}
	
		else if(P9261_TxStatus_Message.Tx_OverVoltage_Limit_Status == TRUE)
		{
			P9261_TxStatus_Message.Tx_OverVoltage_Limit_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Tx_Die_Temp_OverLimit_Status == TRUE)
		{
			P9261_TxStatus_Message.Tx_Die_Temp_OverLimit_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Illegal_Packet_During_ID_Config_Status == TRUE)
		{
			P9261_TxStatus_Message.Illegal_Packet_During_ID_Config_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Tx_Under_Voltage_Limit_Status == TRUE)
		{
			P9261_TxStatus_Message.Tx_Under_Voltage_Limit_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Wireless_Charger_Off_Status == TRUE)
		{
			P9261_TxStatus_Message.Wireless_Charger_Off_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.PEPS_Interrupted_Status == TRUE)
		{
			P9261_TxStatus_Message.PEPS_Interrupted_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		else if(P9261_TxStatus_Message.Standby_Status == TRUE)
		{
			P9261_TxStatus_Message.Standby_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		*/
		else if(P9261_TxStatus_Message.Power_Loss_Fod_Alarm_Status == TRUE)
		{
			P9261_TxStatus_Message.Power_Loss_Fod_Alarm_Status = FALSE;
			WPC_Function_Status.Ploss_Remove_End_Flag = FALSE;
			//WPC_Function_Status.First_Ploss_FOD_Flag = TRUE;
			PlossFOD_Alarm_Processer();
			/*PlossFOD_Count++;
			if(PlossFOD_Count>=4)
			{
				PlossFOD_Count = CLEAR;
				Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
				TEST_TP4 = 1;
			}*/
		}
		else if(P9261_TxStatus_Message.Open_FOD_Alarm_Status == TRUE)
		{
			P9261_TxStatus_Message.Open_FOD_Alarm_Status = FALSE;
			WPC_Function_Status.Forget_Remove_End_Flag = FALSE;
			//WPC_Function_Status.First_Open_FOD_Flag = TRUE;
			OpenFOD_Alarm_Processer();	
		}
		else if(P9261_TxStatus_Message.Tx_OverCurrent_Alarm_Status == TRUE)
		{
			P9261_TxStatus_Message.Tx_OverCurrent_Alarm_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}
		/*else if(P9261_TxStatus_Message.Rx_Jiggle_Alarm_Status == TRUE)
		{
			P9261_TxStatus_Message.Rx_Jiggle_Alarm_Status = FALSE;
			Abnormal_Event.P9261_Error_Alarm_Flag = TRUE;
		}*/
		else{}
	}
	else
	{
		Clear_Abnormal_Event_Flag(&Abnormal_Event);	
		//Last_P9261_Tx_Status_Code = P9261_Reg_State.TX_STATUS_MESSAGE;
		
		/*if(WPC_Function_Status.First_Open_FOD_Flag==TRUE)
		{
			WPC_Function_Status.First_Open_FOD_Flag = FALSE;
		}*/
		
	}
	if(WPC_Function_Status.Over_Temperature_Alarm_Status == TRUE)
	{
		//WPC_Function_Status.Over_Temperature_Alarm_Status = FALSE;	
		Tx_Extern_OverTemperature_Alarm_Processer();
	}
}
/***********************************************************************************************************************
* Function Name: OpenFOD_Alarm_Processer
* Description  : This function is OpenFOD_Alarm_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void PlossFOD_Alarm_Processer(void)
{
	PLOSS_FOD_Alarm_Count++;
	if(PLOSS_FOD_Alarm_Count>=PLOSS_FOD_COUNT)
	{
		WPC_Function_Status.Hold_Ploss_FOD_Flag = TRUE;
		PLOSS_FOD_Alarm_Count = CLEAR;
		//TEST_TP3 = 0;
	}
}

/***********************************************************************************************************************
* Function Name: OpenFOD_Alarm_Processer
* Description  : This function is OpenFOD_Alarm_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OpenFOD_Alarm_Processer(void)
{
	if(WPC_Function_Status.OpenFOD_Detect_Time_Flag==FALSE)
	{
		WPC_Function_Status.OpenFOD_Detect_Time_Flag = TRUE;
		OpenFOD_Alarm_Algorithm();	
		FOD_Alarm_Count++;
		//I2C_DATA_CYCLE_COUNT = CLEAR;
	}
	else
	{
		OpenFOD_Alarm_Algorithm();
		if(FOD_Alarm_Count>=FOD_COUNT)
		{
			WPC_Function_Status.Hold_Open_FOD_Flag = TRUE;	
			OpenFOD_Detect_Restart();
		}
	}
}
/***********************************************************************************************************************
* Function Name: OpenFOD_Alarm_Algorithm
* Description  : This function is OpenFOD_Alarm_Algorithm function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OpenFOD_Alarm_Algorithm(void)
{
	if((COIL_1_Q_Message<120)||(COIL_2_Q_Message<120)||(COIL_3_Q_Message<120))
	{
		COIL_DETECT = RESET_COIL;
		if(COIL_1_Q_Message<120)		{COIL_DETECT = COIL_1;}
		else if(COIL_2_Q_Message<120)	{COIL_DETECT = COIL_2;}
		else if(COIL_3_Q_Message<120)	{COIL_DETECT = COIL_3;}
		if(LAST_COIL!=RESET_COIL)
		{
			if(COIL_DETECT!=LAST_COIL)
			{
				FOD_Alarm_Count = CLEAR;	
			}
			FOD_Alarm_Count++;
		}
		LAST_COIL = COIL_DETECT;
	}
	else
	{
		OpenFOD_Detect_Restart();
	}
}
/***********************************************************************************************************************
* Function Name: OpenFOD_Detect_Restart
* Description  : This function is OpenFOD_Detect_Restart function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OpenFOD_Detect_Restart(void)
{
	WPC_Function_Status.OpenFOD_Detect_Time_Flag = FALSE;
	FOD_Alarm_Count = CLEAR;
	COIL_DETECT = RESET_COIL;
	LAST_COIL = COIL_DETECT;
}
/***********************************************************************************************************************
* Function Name: Tx_Extern_OverTemperature_Alarm_Processer
* Description  : This function is Tx_Extern_OverTemperature_Alarm_Processer function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Tx_Extern_OverTemperature_Alarm_Processer(void)
{
	CHARGE_DISABLE;
	P9261_IIC_State.active_flag = FALSE;
	Over_Temperature_Reset_Time = DELAY_3S;
	Abnormal_Event.Tx_Extern_OverTemp_Alarm_Flag = TRUE;
	WPC_Function_Status.Over_Temperature_Alarm_Status = FALSE;
}














