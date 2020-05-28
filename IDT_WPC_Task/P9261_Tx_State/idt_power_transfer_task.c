/***********************************************************************************************************************
* File Name    : idt_power_transfer_task.c
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file is wpc task function.
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
Locate variables and functions
***********************************************************************************************************************/
unsigned char Charge_Go_Flag = CLEAR;
unsigned char IDT_WCP_Power_Transfer_Process(void);
void Rx_Status_Detecting(void);
void Current_Detect(void);
void Q_FOD_Detect(void);
/***********************************************************************************************************************
* Function Name: IDT_WPC_Power_Transfer_Task
* Description  : This function is IDT_WPC_Power_Transfer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_WCP_Power_Transfer_Task(void)
{
	if(IDT_WPC_State.Power_Transfer_Task_Flag==TRUE)
	{
		IDT_WPC_State.Power_Transfer_Task_Flag = FALSE;
		//----------------------------------------------//
		/*Reset_Buzzer_LED_State();
		Charge_Starting_Buzzer_Out();
		CHARGE_LED_ON;
		*/
		//Last_P9261_Tx_State_Code = P9261_Reg_State.TX_STATE_MESSAGE;
		//TEST_TP2 = 1;
		//----------------------------------------------//
		IDT_WPC_TASK = IDT_WCP_POWER_TRANSFER_TASK;
	}
	IDT_WPC_TASK = IDT_WCP_Power_Transfer_Process();
}
/***********************************************************************************************************************
* Function Name: IDT_WPC_Power_Transfer_Process
* Description  : This function is IDT_WPC_Power_Transfer_Process function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char IDT_WCP_Power_Transfer_Process(void)			// --- In Tx state 09 --- //
{
	IDT_WPC_NEXT_TASK = IDT_WCP_POWER_TRANSFER_TASK;
	//IDT_P9261_TX_Status_Processer();
	if(P9261_TxState_Message.Startup_State == TRUE)
	{
		P9261_TxState_Message.Startup_State = FALSE;
		IDT_WPC_State.Startup_Task_Flag = TRUE;
		IDT_WPC_TASK = IDT_WPC_STARTUP_TASK;
	}
	else if(P9261_TxState_Message.Idle_State == TRUE)
	{
		P9261_TxState_Message.Idle_State = FALSE;
		IDT_WPC_State.Idle_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_IDLE_TASK;
	}
	else if(P9261_TxState_Message.Analog_Ping_State == TRUE)
	{
		P9261_TxState_Message.Analog_Ping_State = FALSE;
		IDT_WPC_State.Analog_Ping_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_ANALOG_PING_TASK;
	}
	else if(P9261_TxState_Message.Q_Measurement_State == TRUE)
	{
		P9261_TxState_Message.Q_Measurement_State = FALSE;
		IDT_WPC_State.Q_Measurement_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_Q_MEASUREMENT_TASK;
	}
	else if(P9261_TxState_Message.Digital_Ping_State == TRUE)
	{
		P9261_TxState_Message.Digital_Ping_State = FALSE;
		IDT_WPC_State.Digital_Ping_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_DIGITAL_PING_TASK;
	}
	else if(P9261_TxState_Message.Identification_State == TRUE)
	{
		P9261_TxState_Message.Identification_State = FALSE;
		IDT_WPC_State.Identification_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_IDENTIFICATION_TASK;
	}
	else if(P9261_TxState_Message.Rx_Removed_Detection_State == TRUE)
	{
		P9261_TxState_Message.Rx_Removed_Detection_State = FALSE;
		IDT_WPC_State.Rx_Removed_Detection_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_RX_REMOVED_DETECTION_TASK;
	}
	else if(P9261_TxState_Message.Configuration_State == TRUE)
	{
		P9261_TxState_Message.Configuration_State = FALSE;
		IDT_WPC_State.Configuration_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_CONFIGURATION_TASK;
	}
	else if(P9261_TxState_Message.Power_Transfer_Init_State == TRUE)
	{
		P9261_TxState_Message.Power_Transfer_Init_State = FALSE;
		IDT_WPC_State.Power_Transfer_Init_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_POWER_TRANSFER_INIT_TASK;
	}
	else if(P9261_TxState_Message.Id_Confirmation_State == TRUE)
	{
		P9261_TxState_Message.Id_Confirmation_State = FALSE;
		IDT_WPC_State.Id_Confirmation_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_ID_CONFIRMATION_TASK;
	}
	else if(P9261_TxState_Message.Remove_Power_State == TRUE)
	{
		P9261_TxState_Message.Remove_Power_State = FALSE;
		IDT_WPC_State.Remove_Power_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_REMOVE_POWER_TASK;
	}
	else if(P9261_TxState_Message.Restart_State == TRUE)
	{
		P9261_TxState_Message.Restart_State = FALSE;
		IDT_WPC_State.Restart_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_RESTART_TASK;
	}
	else if(P9261_TxState_Message.Negotiation_State == TRUE)
	{
		P9261_TxState_Message.Negotiation_State = FALSE;
		IDT_WPC_State.Negotiation_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_NEGOTIATION_TASK;
	}
	else if(P9261_TxState_Message.Re_Negotiation_State == TRUE)
	{
		P9261_TxState_Message.Re_Negotiation_State = FALSE;
		IDT_WPC_State.Re_Negotiation_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_RE_NEGOTIATION_TASK;
	}
	else if(P9261_TxState_Message.Calibration_State == TRUE)
	{
		P9261_TxState_Message.Calibration_State = FALSE;
		IDT_WPC_State.Calibration_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_CALIBRATION_TASK;
	}else{}	
	Rx_Status_Detecting();
	return IDT_WPC_NEXT_TASK;
	/*
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				if(P9261_RegMessage.Over_Temperature_Alarm_Status==TRUE)
				{
					if(Buzzer_State.Charge_Start_Flag==FALSE)
					{
						P9261_RegMessage.Over_Temperature_Alarm_Status = FALSE;
						IDT_WPC_State.Over_Temperature_Alarm_Task_Flag = TRUE;
						IDT_WPC_NEXT_TASK = IDT_WPC_OVERTEMPERATURE_TASK;
					}
				}
				else if(P9261_RegMessage.Open_Fod_Alarm_Status==TRUE)		// Tx Open FOD alarm status 25(0x19) //	
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Open_Fod_Alarm_Status = FALSE;
					IDT_WPC_State.Fod_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_FOD_ALARM_TASK;
				}
				else if(P9261_RegMessage.Power_Loss_Fod_Alarm_Status==TRUE)	// Tx Power loss FOD alarm status 24(0x18) //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Power_Loss_Fod_Alarm_Status = FALSE;
					IDT_WPC_State.Fod_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_FOD_ALARM_TASK;
				}
				else if(P9261_RegMessage.Other_End_Of_Power_Rx_Alarm_Status==TRUE) // Tx Status Other end-of-power rx alarm 09 //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Other_End_Of_Power_Rx_Alarm_Status = FALSE;
					Other_Alarm.Other_Eof_Pwr_Rx_Alarm_Status_Flag = TRUE; // 20191017 modify //
					IDT_WPC_State.Other_Eof_Pwr_Rx_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_OTHER_EOF_PWR_RX_ALARM_TASK;
				}
				else if(P9261_RegMessage.Rx_Jiggle_Alarm_Status==TRUE)	// Rx Jigle alarm status 27(0x1b) //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Rx_Jiggle_Alarm_Status = FALSE;
					Other_Alarm.Rx_Jiggle_Status_Flag = TRUE;	// 20191016 modify //
					IDT_WPC_State.Rx_Jiggle_Alarm_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_JIGGLE_ALARM_TASK;
				}
				else if(P9261_RegMessage.Rx_Over_Current_Ept_Code_Status==TRUE)	// Rx over_current alarm status 07(0x07) //
				{
					P9261_RegMessage.Rx_Over_Current_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Over_Current_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_OVERCURRENT_TASK;
				}
				else if(P9261_RegMessage.Received_Power_Packet_Timeout_Status==TRUE)	// Rx receiver power packet timeout alarm status 12(0x0c) //
				{
					P9261_RegMessage.Received_Power_Packet_Timeout_Status = FALSE;
					Other_Alarm.Receiver_Timeout_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Received_Power_Packet_Timeout_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RECEIVE_POWER_PACKET_TIMEOUT_TASK;
				}
				else if(P9261_RegMessage.No_Response_Rx_Ept_Code_Status==TRUE)	// Rx No_Response_Rx_Ept_Code_Status 4(0x04) //
				{
					P9261_RegMessage.No_Response_Rx_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Fail_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_FAIL_EPT_CODE_TASK;
				}
				else if(P9261_RegMessage.Internal_Fault_Rx_Ept_Code_Status==TRUE)	// Rx Internal_Fault_Rx_Ept_Code_Status 5(0x05) //
				{
					P9261_RegMessage.Internal_Fault_Rx_Ept_Code_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20191114 modify //
					IDT_WPC_State.Rx_Fail_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_FAIL_EPT_CODE_TASK;
				}
				else if(P9261_RegMessage.Rx_Over_Temperature_Alarm_Status==TRUE)	// Rx Over Temperature EPT Code 0x06 //
				{
					CHARGE_STATE_LED_OFF;
					P9261_RegMessage.Rx_Over_Temperature_Alarm_Status = FALSE;
					Other_Alarm.Rx_Ept_Code_Flag = TRUE;	// 20200213 modify //
					Other_Alarm.Rx_Over_Temperature_Flag = TRUE;  //20200218 modify //
					IDT_WPC_State.Rx_Fail_Ept_Code_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RX_FAIL_EPT_CODE_TASK;
				}
				
				else if(P9261_RegMessage.Tx_Under_Voltage_Limit_Status==TRUE)	// Tx Status Tx_Under_Voltage_Limit 0x14 //
				{
					P9261_RegMessage.Tx_Under_Voltage_Limit_Status = FALSE;
					IDT_WPC_State.Tx_Under_Voltage_Limit_Task_Flag = TRUE;	// reset p9261 //
					IDT_WPC_NEXT_TASK = IDT_WPC_TX_UNDER_VOLTAGE_LIMIT_TASK;
				}
				else if(P9261_RegMessage.Re_Negotiation_Status==TRUE)
				{
					CHARGE_STATE_LED_OFF;
					P9261_RegMessage.Re_Negotiation_Status = FALSE;
					IDT_WPC_State.Re_Negotiation_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_RE_NEGOTIATION_TASK;
					
				}
				else if(P9261_RegMessage.Charge_Complete_From_Rx_Status==TRUE)	// Tx Status charge complete from rx 02 //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Charge_Complete_From_Rx_Status = FALSE;
					IDT_WPC_State.Idle_Task_Flag = TRUE;
					Charge_Starting_Buzzer_Out();
					IDT_WPC_NEXT_TASK = IDT_WPC_IDLE_TASK;
				}
				else if(P9261_RegMessage.Full_Chargeed_Status==TRUE)			// Tx Status full charged from tx 08 //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Full_Chargeed_Status = FALSE;
					IDT_WPC_State.Idle_Task_Flag = TRUE;
					Charge_Starting_Buzzer_Out();
					IDT_WPC_NEXT_TASK = IDT_WPC_IDLE_TASK;
				}
				else if(P9261_RegMessage.Analog_Ping_Status==TRUE)		// Tx State 02 //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Analog_Ping_Status = FALSE;
					IDT_WPC_State.Analog_Ping_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_ANALOG_PING_TASK;
				}
				else if(P9261_RegMessage.Q_Measurement_Status==TRUE)	// Tx State 03 //
				{
					CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
					P9261_RegMessage.Q_Measurement_Status = FALSE;
					IDT_WPC_State.Q_Measurement_Task_Flag = TRUE;
					IDT_WPC_NEXT_TASK = IDT_WPC_Q_MEASUREMENT_TASK;
				}
				else
				{
					IDT_WPC_NEXT_TASK = IDT_WCP_POWER_TRANSFER_TASK;
				}
			}
			else
			{
				IDT_WPC_State.EUT_Sleep_Task_Flag = TRUE;
				CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
				
				IDT_WPC_NEXT_TASK = IDT_WPC_EUT_SLEEP_TASK;
			}
		}
		else
		{
			IDT_WPC_State.Power_Sw_Off_Task_Flag = TRUE;
			CHARGE_STATE_LED_OFF;						// charger led off //20191022 modify//
			
			IDT_WPC_NEXT_TASK = IDT_WPC_POWER_SW_OFF_TASK;
		}
	}
	else
	{
		Phone_Placement_Detect();	// phone forget detect //
		
		WPC_Function_Status.ACC_Off_Dly3_5T_Flag = TRUE;
		IDT_WPC_State.Cellphone_Left_Alert_Task_Flag = TRUE;
		IDT_WPC_NEXT_TASK = IDT_WPC_CELLPHONE_LEFT_ALERT_TASK;
	}
	return IDT_WPC_NEXT_TASK;
	*/
}
/***********************************************************************************************************************
* Function Name: Current_Detect
* Description  : This function is Current_Detect function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Q_FOD_Detect(void)
{
	/*if((COIL_1_Q_Message<120)||(COIL_2_Q_Message<120)||(COIL_3_Q_Message<120))
	{
		P9261_RegMessage.Open_Fod_Alarm_Status = TRUE;
	}*/
}
/***********************************************************************************************************************
* Function Name: Current_Detect
* Description  : This function is Current_Detect function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Current_Detect(void)
{
	/*
	if(WPC_Function_Status.On_Chargeing_State_Flag==FALSE)
	{
		if(Last_COIL_Current>1000)
		{
			WPC_Function_Status.On_Chargeing_State_Flag = TRUE;
		}
		Last_COIL_Current = COIL_Current;
	}
	else
	{
		if(COIL_Current<400)
		{
			if(Coil_Current_Count>=150)
			{
				WPC_Function_Status.On_Chargeing_State_Flag = FALSE;
				Coil_Current_Count = CLEAR;
				CHARGE_STATE_LED_OFF;
				Last_COIL_Current = COIL_Current;
			}
		}
		else
		{
			if(Coil_Current_Count!=CLEAR)
			{
				Coil_Current_Count = CLEAR;	
			}
		}
	}
	*/
}
/***********************************************************************************************************************
* Function Name: Rx_Status_Detecting
* Description  : This function is Rx_Status_Detecting function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Rx_Status_Detecting(void)
{
	if(IDT_WPC_NEXT_TASK!=IDT_WCP_POWER_TRANSFER_TASK)
	{
		WPC_Function_Status.Charge_Buzzer_Trigger_End_Flag = FALSE;
		WPC_Function_Status.Exception_Handing_Flag = FALSE;
		if(IDT_WPC_NEXT_TASK!=IDT_WPC_IDLE_TASK)
		{
			CHARGE_STATE_LED_OFF;
		}
	}
	else
	{
		if(WPC_Function_Status.Exception_Handing_Flag==FALSE)
		{
			if(Abnormal_Event.Rx_Charge_Complete_Alarm_Flag==TRUE)
			{
				Abnormal_Event.Rx_Charge_Complete_Alarm_Flag = FALSE;
				WPC_Function_Status.Exception_Handing_Flag = TRUE;
				CHARGE_STATE_LED_OFF;
				Charge_Starting_Buzzer_Out();
			}
			else if(Abnormal_Event.Rx_Full_Charged_Alarm_Flag==TRUE)
			{
				Abnormal_Event.Rx_Full_Charged_Alarm_Flag = FALSE;
				WPC_Function_Status.Exception_Handing_Flag = TRUE;
				CHARGE_STATE_LED_OFF;
				Charge_Starting_Buzzer_Out();
			}
		}
	}
}
