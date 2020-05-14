/***********************************************************************************************************************
* File Name    : wpc_application.c
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

//----------------------------------------------------//
struct WPC_SYSTEM_MODE_FLAG	WPC_System;

unsigned char System_Init_Time = 200;
volatile unsigned char WPC_MODE_CASE = WPC_SHUTDOWN_MODE;
volatile unsigned char WPC_NEXT_MODE_CASE = WPC_SHUTDOWN_MODE;

//----------------------------------------------------//
IDT_WPC_STATE_TASK	IDT_WPC_TASK_NUM;
struct IDT_WPC_FLAG IDT_WPC_State;
struct WPC_FUNCTION_STATUS_FLAG WPC_Function_Status;
//struct WPC_OTHER_ALARM_STATUS_FLAG	Other_Alarm;


unsigned int I2C_DATA_CYCLE_COUNT = CLEAR;

unsigned int Read_I2C_Data_Duty_Time = CLEAR;
unsigned char Read_Next_Register_Delay_Time = CLEAR;

unsigned char FOD_Alarm_Count = CLEAR;
unsigned char PLOSS_FOD_Alarm_Count = CLEAR;

//unsigned char p9261_led_state = CLEAR;
//unsigned int caculator_time = CLEAR;
unsigned int UVOL_Wait_Time = CLEAR;
unsigned char Last_Charge_Load_On_Tx;
//unsigned char Coil_Current_Count = CLEAR;
unsigned int Temperature_Value = RESET_TEMPERATURE_VALUE;
//unsigned int Door_Close_Waitting_Time;
unsigned int EUT_Waitting_Time = CLEAR;//DELAY_1S;
unsigned int ACC_Off_Reset_Time = DELAY_3_5S;
unsigned int Over_Temperature_Reset_Time = CLEAR;
volatile unsigned char IDT_WPC_TASK;
volatile unsigned char IDT_WPC_NEXT_TASK;
//unsigned int *WPC_Function_State_Flag_address;
volatile unsigned int *wpc_function_state_flag_address;
volatile unsigned int *idt_wpc_state_flag_address;



unsigned char function_test_flag;
unsigned char Standby_Count = CLEAR;
//unsigned long p9261_message_case = 0x00000000;
//unsigned char read_p9261_message(unsigned long const *address);
//=====================================================================//
/*unsigned char read_p9261_message(unsigned long const *address)
{
	p9261_message_case = *address;
}*/
/***********************************************************************************************************************
* Function Name: Clear_WPC_Function_State_Flag
* Description  : This function is Clear_WPC_Function_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_WPC_Function_State_Flag(unsigned long const *address)
{
	wpc_function_state_flag_address = address;
	*wpc_function_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: Clear_IDT_WPC_State_Flag
* Description  : This function is Clear_IDT_WPC_State_Flag function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
unsigned char Clear_IDT_WPC_State_Flag(unsigned long const *address)
{
	idt_wpc_state_flag_address = address;
	*idt_wpc_state_flag_address = CLEAR;
}
/***********************************************************************************************************************
* Function Name: WPC_Sleep_Reset
* Description  : This function is WPC_Sleep_Reset function. 
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Sleep_Reset(void)
{
	I2C_Variable_Init();
	
	//Clear_Buzzer_State_Flag(&Buzzer_State);
	//Clear_Led_State_Flag(&LED_Status);
	Init_Buzzer_State_Flag(&Buzzer_State,0x0007);
	Init_Led_State_Flag(&LED_Status,0x03);
	Clear_WPC_Function_State_Flag(&WPC_Function_Status);
	Clear_IDT_WPC_State_Flag(&IDT_WPC_State);
	Clear_P9261_Tx_State_Flag(&P9261_TxState_Message);
	Clear_P9261_Tx_Status_Flag(&P9261_TxStatus_Message);
	Clear_P9261_Combined_State_Flag(&P9261_CombinedMessage);
	Clear_Abnormal_Event_Flag(&Abnormal_Event);
	/*
	Clear_WPC_Function_State_Flag(&WPC_Function_Status);
	Clear_WPC_Function_State_Flag(&IDT_WPC_State);
	Clear_P9261_IIC_State_Flag(&P9261_IIC_State);
	Clear_P9261_RegMessage_State_Flag(&P9261_TxState_Message);
	Clear_P9261_RegMessage_State_Flag(&P9261_TxStatus_Message);
	Clear_P9261_Combined_State_Flag(&P9261_CombinedMessage);
	*/
	IDT_WPC_TASK = IDT_WPC_STARTUP_TASK;
	WPC_Function_Status.P9261_State_Update_End_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: IO_Interface_Init
* Description  : This function is IO_Interface_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IO_Interface_Init(void)
{
	//CAN0_STB = LEVEL_HIGH;
	ACC_ON_RL78 = LEVEL_HIGH;	// Control Q33 shuts down : power on //
	CHARGE_DISABLE;
	BUZZER = OFF;				// Buzzer Disable //
	KEYBOARD_PWR_SW_LED_OFF;	// Keyboard pwr_sw led //
	POWER_SW_LED_OFF;			// Internal PCBA led //
	CHARGE_STATE_LED_OFF;
}
/***********************************************************************************************************************
* Function Name: Variable_Init
* Description  : This function is Variable_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Variable_Init(void)
{	
	Clear_Flag_Group(&WCHC01_SYS);
	Clear_Flag_Group(&WPC_System);
	
	Clear_ACC_State_Flag(&ACC_Status);
	Clear_Power_Sw_State_Flag(&Power_Sw);
	Clear_Door_State_Flag(&Door_Status);
	Clear_Trunk_State_Flag(&Trunk_Status);
	//Clear_Flag_Group(&Buzzer_State);
	//Clear_Flag_Group(&LED_Status);
	
	Clear_WPC_Function_State_Flag(&WPC_Function_Status);
	Clear_IDT_WPC_State_Flag(&IDT_WPC_State);
	Clear_P9261_IIC_State_Flag(&P9261_IIC_State);
	Clear_P9261_Tx_State_Flag(&P9261_TxState_Message);
	Clear_P9261_Tx_Status_Flag(&P9261_TxStatus_Message);
	Clear_P9261_Combined_State_Flag(&P9261_CombinedMessage);
	Clear_Abnormal_Event_Flag(&Abnormal_Event);
	
	Init_Buzzer_State_Flag(&Buzzer_State,0x0007);
	Init_Led_State_Flag(&LED_Status,0x03);
	Mapping_WPC_System_Address(&WPC_System);
	/*
	Clear_ACC_State_Flag(&ACC_Status);
	Clear_Power_Sw_State_Flag(&Power_Sw);
	Clear_Door_State_Flag(&Door_Status);
	Clear_Trunk_State_Flag(&Trunk_Status);
	Clear_Buzzer_State_Flag(&Buzzer_State);
	Clear_Led_State_Flag(&LED_Status);
	Clear_WPC_Function_State_Flag(&WPC_Function_Status);
	Clear_WPC_Function_State_Flag(&IDT_WPC_State);
	Clear_P9261_IIC_State_Flag(&P9261_IIC_State);
	Clear_P9261_RegMessage_State_Flag(&P9261_TxState_Message);
	Clear_P9261_RegMessage_State_Flag(&P9261_TxStatus_Message);
	Clear_P9261_Combined_State_Flag(&P9261_CombinedMessage);
	*/
}
/***********************************************************************************************************************
* Function Name: P9261_Reg_Msg_Init
* Description  : This function is P9261_Reg_Msg_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void P9261_Reg_Msg_Init(void)
{
	P9261_IIC_FLOW = 0xD0;
	P9261_Reg_State.COIL_NUMBER = RESET_VALUE;
	P9261_Reg_State.TX_STATE_MESSAGE = STATE_startup;
	P9261_Reg_State.TX_STATUS_MESSAGE = STATUS_ok;
	P9261_Reg_State.COMBINED_MESSAGE = COMBINED_standby;
	P9261_Reg_State.NTC_ADC_OUT = NTC_DEFAULT_TEMPERATURE;
	P9261_Reg_State.COIL_Q_MESSAGE = Q_DEFAULT;
	P9261_Reg_State.RPP = CLEAR;
	P9261_TX_STATE_MSG = P9261_Reg_State.TX_STATE_MESSAGE;
	P9261_TX_STATUS_MSG = P9261_Reg_State.TX_STATUS_MESSAGE;
	P9261_COMBINED_MSG = P9261_Reg_State.COMBINED_MESSAGE;
	COIL_1_Q_Message = 0xFFFF;
	COIL_2_Q_Message = 0xFFFF;
	COIL_3_Q_Message = 0xFFFF;
}
/***********************************************************************************************************************
* Function Name: I2C_Variable_Init
* Description  : This function is I2C_Variable_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void I2C_Variable_Init(void)
{	
	P9261_Reg_Msg_Init();
	Clear_P9261_IIC_State_Flag(&P9261_IIC_State);
	P9261_IIC_State.iic_send_end_flag = FALSE;
	P9261_IIC_State.iic_receiver_end_flag = FALSE;//TRUE;
	P9261_IIC_State.active_flag = FALSE;
}
/***********************************************************************************************************************
* Function Name: Check_Door_Trunk_Statue
* Description  : This function is Check_Door_Trunk_Statue function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_System_Initial(void)
{
	IO_Interface_Init();
	Variable_Init();
	I2C_Variable_Init();
	
	WCHC01_SYS.INITIAL_END_FLAG = FALSE;
	WPC_System.Shutdown_Mode = TRUE;
	WPC_MODE_CASE = WPC_SHUTDOWN_MODE;
	WPC_NEXT_MODE_CASE = WPC_MODE_CASE;
	IDT_WPC_TASK = IDT_WPC_STARTUP_TASK;
	WPC_Function_Status.Forget_Remove_End_Flag = TRUE;
	WPC_Function_Status.Ploss_Remove_End_Flag = TRUE;
	WPC_Function_Status.P9261_State_Update_End_Flag = TRUE;
}
/***********************************************************************************************************************
* Function Name: Phone_Placement_Detect
* Description  : This function implements wpc task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Phone_Placement_Detect(void)
{
	if((WPC_Function_Status.Charge_Load_In_Tx_Flag==TRUE)||(Last_Charge_Load_On_Tx==TRUE))
	{
		Buzzer_State.Forget_Buzzer_Tigger_Flag = TRUE;
	}
	else
	{
		Buzzer_State.Forget_Buzzer_Tigger_Flag = FALSE;
	}
	/*if((P9261_CombinedMessage.In_Charging==TRUE)||((Last_P9261_Tx_Status_Code==0x02)&&(P9261_CombinedMessage.In_Charge_complete_From_Rx==TRUE)))
	{
		Buzzer_State.Forget_Buzzer_Tigger_Flag = TRUE;
	}
	else
	{
		if((Last_P9261_Tx_State_Code==0x09)||(Last_P9261_Tx_Status_Code==0x02))
		{
			Buzzer_State.Forget_Buzzer_Tigger_Flag = TRUE;
		}
		else
		{
			Buzzer_State.Forget_Buzzer_Tigger_Flag = FALSE;
		}
	}*/
}
/***********************************************************************************************************************
* Function Name: IDT_WPC_Processer_Task
* Description  : This function implements wpc task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_WPC_Processer_Task(void)
{
	if(WPC_Function_Status.P9261_State_Update_End_Flag==FALSE)
	{
		OverTemperature_Detect();
		IDT_P9261_TX_Status_Processer();	// any abnormal status check function //
		ChargeLoan_In_Tx_Check();		//20200511//
		switch(IDT_WPC_TASK)
		{
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_STARTUP_TASK:														//= 0x00
														IDT_WPC_Startup_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_IDLE_TASK:															//= 0x01
														IDT_WPC_Idle_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_ANALOG_PING_TASK:													//= 0x02
														IDT_WPC_Analog_Ping_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_Q_MEASUREMENT_TASK:													//= 0x03
														IDT_WPC_Q_Measurement_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_DIGITAL_PING_TASK:													//= 0x04
														IDT_WPC_Digital_Ping_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_IDENTIFICATION_TASK:												//= 0x05
														IDT_WPC_Identification_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RX_REMOVED_DETECTION_TASK:											//= 0x06
														IDT_WPC_Rx_Removed_Detection_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_CONFIGURATION_TASK:													//= 0x07
														IDT_WPC_Configuration_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_POWER_TRANSFER_INIT_TASK:											//= 0x08
														IDT_WPC_Power_Transfer_Init_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WCP_POWER_TRANSFER_TASK:												//= 0x09
														IDT_WCP_Power_Transfer_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_ID_CONFIRMATION_TASK:												//= 0x0A
														IDT_WPC_Id_Confirmation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_REMOVE_POWER_TASK:													//= 0x0B
														IDT_WPC_Remove_Power_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RESTART_TASK:														//= 0x0C
														IDT_WPC_Restart_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_NEGOTIATION_TASK:													//= 0x0D
														IDT_WPC_Negotiation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_UNKNOW:																	//= 0x0E
														IDT_Unknow();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RE_NEGOTIATION_TASK:												//= 0x0F
														IDT_WPC_Re_Negotiation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_CALIBRATION_TASK:													//= 0x10
														IDT_WPC_Calibration_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			default:
			break;
		}
		P9261_TX_STATE_MSG = RESET_VALUE;
		P9261_TX_STATUS_MSG = RESET_VALUE;
		P9261_COMBINED_MSG = RESET_VALUE;
		WPC_Function_Status.P9261_State_Update_End_Flag = TRUE;
	}
	WPC_Abnormal_Alarm_Task();
	//TEST_TP3 = 0;
	/*
	if(P9261_IIC_State.iic_receiver_end_flag==TRUE)
	{	
		Read_I2C_Data_Duty_Time = I2C_DUTY_TIME;
		P9261_IIC_State.iic_receiver_end_flag = FALSE;
		Clear_P9261_RegMessage_State_Flag(&P9261_RegMessage); // clear read p9261 register message //
		Receive_P9261_Tx_State_Task(P9261_TX_STATE_MSG);
		Receive_P9261_Tx_Status_Task(P9261_TX_STATUS_MSG);
		Receive_P9261_Combined_Msg_Task(P9261_COMBINED_MSG);
		
		
		OpenFOD_Alarm_Active_Detect();
		
		
		
		
		#ifdef _USED_P9261_INTERNAL_TEMPERATURE_STATUS_
		
		#else
			OverTemperature_Detect();
		#endif
		switch(IDT_WPC_TASK)
		{
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_STARTUP_TASK:																		//= 0x00
														IDT_WPC_Startup_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_IDLE_TASK:																			//= 0x01
														IDT_WPC_Idle_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_ANALOG_PING_TASK:																	//= 0x02
														IDT_WPC_Analog_Ping_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_Q_MEASUREMENT_TASK:																	//= 0x03
														IDT_WPC_Q_Measurement_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_DIGITAL_PING_TASK:																	//= 0x04
														IDT_WPC_Digital_Ping_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_IDENTIFICATION_TASK:																//= 0x05
														IDT_WPC_Identification_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RX_REMOVED_DETECTION_TASK:															//= 0x06
														IDT_WPC_Rx_Removed_Detection_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_CONFIGURATION_TASK:																	//= 0x07
														IDT_WPC_Configuration_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_POWER_TRANSFER_INIT_TASK:															//= 0x08
														IDT_WPC_Power_Transfer_Init_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WCP_POWER_TRANSFER_TASK:																//= 0x09
														IDT_WCP_Power_Transfer_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_ID_CONFIRMATION_TASK:																//= 0x0A
														IDT_WPC_Id_Confirmation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_REMOVE_POWER_TASK:																	//= 0x0B
														IDT_WPC_Remove_Power_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RESTART_TASK:																		//= 0x0C
														IDT_WPC_Restart_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_NEGOTIATION_TASK:																	//= 0x0D
														IDT_WPC_Negotiation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_UNKNOW:																					//= 0x0E
														IDT_Unknow();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RE_NEGOTIATION_TASK:																//= 0x0F
														IDT_WPC_Re_Negotiation_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_CALIBRATION_TASK:																	//= 0x10
														IDT_WPC_Calibration_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_FOD_ALARM_TASK:																		//= 0x11
														IDT_FOD_Alarm_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_POWER_SW_OFF_TASK:																	//= 0x12
														IDT_Power_Sw_Off_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_CELLPHONE_LEFT_ALERT_TASK:															//= 0x13
														IDT_Cellphone_Left_Alert_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_ACC_OFF_TASK:																		//= 0x14
														IDT_ACC_Off_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_EUT_SLEEP_TASK:																		//= 0x15
														IDT_EUT_Sleep_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_OVERTEMPERATURE_TASK:																//= 0x16
														IDT_Temperature_Alarm_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RX_JIGGLE_ALARM_TASK:																//= 0x17
														IDT_RX_Jiggle_Alarm_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_OTHER_EOF_PWR_RX_ALARM_TASK:														//= 0x18
														IDT_Other_Eof_Pwr_Rx_Alarm_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_TX_UNDER_VOLTAGE_LIMIT_TASK:														//= 0x19
														IDT_TX_Under_Voltage_Limit_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RX_FAIL_EPT_CODE_TASK:																//= 0x1A
														IDT_RX_Fail_Ept_Code_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_OVERCURRENT_TASK:																	//= 0x1B
														IDT_Current_Alarm_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_RECEIVE_POWER_PACKET_TIMEOUT_TASK:													//= 0x1C
														IDT_Receive_Power_Packet_Timeout_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			case	IDT_WPC_WIRELESS_CHARGER_OFF_TASK:															//= 0x1D
														IDT_Wireless_Charger_Off_Task();
			break;
			//---------------------------------------------------------------------------------------------------------//
			default:
			break;
		}
		P9261_TX_STATE_MSG = RESET_VALUE;
		P9261_TX_STATUS_MSG = RESET_VALUE;
		P9261_COMBINED_MSG = RESET_VALUE;
	}
	else
	{
		if(Read_I2C_Data_Duty_Time==CLEAR)
		{
			if(P9261_IIC_State.read_reg_start_flag==FALSE)
			{
				I2C_DATA_CYCLE_COUNT++;	// I2C Communication system time counter : per 1 cycle count is 0.21 sec //
				I2C_Commumication_Start();
			}
			else
			{
				if(P9261_IIC_State.next_reg_flag==TRUE)
				{
					if(Read_Next_Register_Delay_Time==CLEAR)
					{
						if(P9261_IIC_State.iic_receiver_end_flag==FALSE)
						{
							P9261_IIC_State.next_reg_flag = FALSE;
							R_IIC00_Master_Send(P9261_ADDRESS,&P9261_Address,2);// i2c - write //
						}
					}
				}
			}
		}
	}
	*/
}
/***********************************************************************************************************************
* Function Name: OverTemperature_Detect
* Description  : This function is OverTemperature_Detect function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OverTemperature_Detect(void)
{
	if(WPC_Function_Status.Temperature_Detect_Start_Flag==TRUE)
	{
		if((P9261_NTC_ADC_OUT>NTC_OVER_TEMPERATURE)&&(P9261_NTC_ADC_OUT<=NTC_DOWN_TEMPERATURE))
		{
			WPC_Function_Status.Over_Temperature_Alarm_Status = TRUE;
		}
		else
		{
			if(P9261_CombinedMessage.In_Charging==FALSE)
			{
				if(WPC_Function_Status.Temperature_Detect_Start_Flag==TRUE)
				{
					WPC_Function_Status.Temperature_Detect_Start_Flag = FALSE;
				}
			}
		}
	}
	else
	{
		if(P9261_CombinedMessage.In_Charging==TRUE)
		{
			WPC_Function_Status.Temperature_Detect_Start_Flag = TRUE;
		}
	}
}
/***********************************************************************************************************************
* Function Name: P9261_Restart_Init
* Description  : This function is P9261_Restart_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void P9261_Restart_Init(void)
{
	if(P9261_COMBINED_MSG==0x09)	{Standby_Count++;}
	else							{Standby_Count = CLEAR;}
	
	if(Standby_Count==2)
	{
		Standby_Count = CLEAR;
		WPC_Function_Status.P9261_ReStart_End_Flag = TRUE;
	}
	/*if(((COIL_1_Q_Message>CLEAR)&&(COIL_1_Q_Message<1000))||((COIL_2_Q_Message>CLEAR)&&(COIL_2_Q_Message<1000))||((COIL_3_Q_Message>CLEAR)&&(COIL_3_Q_Message<1000)))
	{
		WPC_Function_Status.P9261_ReStart_End_Flag = TRUE;
	}*/
}
/***********************************************************************************************************************
* Function Name: OpenFOD_Alarm_Active_Detect
* Description  : This function is OpenFOD_Alarm_Active_Detect function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void OpenFOD_Alarm_Active_Detect(void)
{
	/*if(WPC_Function_Status.OpenFOD_Detect_Time_Flag==TRUE)
	{
		if(I2C_DATA_CYCLE_COUNT>=CNT_X_0_2T)
		{
			I2C_DATA_CYCLE_COUNT = CLEAR;
			OpenFOD_Alarm_Algorithm();
			if(FOD_Alarm_Count>=FOD_COUNT)
			{
				P9261_RegMessage.Open_Fod_Alarm_Status = TRUE;
				OpenFOD_Detect_Restart();
			}
		}
	}*/
}
/***********************************************************************************************************************
* Function Name: ChargeLoan_In_Tx_Check
* Description  : This function is ChargeLoan_In_Tx_Check function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ChargeLoan_In_Tx_Check(void)
{
	if((P9261_Reg_State.TX_STATE_MESSAGE==0x09)&&(P9261_Reg_State.COMBINED_MESSAGE==0x08))
	{
		WPC_Function_Status.Charge_Load_In_Tx_Flag = TRUE;
	}
	else if((P9261_Reg_State.TX_STATUS_MESSAGE==0x08)&&(P9261_Reg_State.COMBINED_MESSAGE==0x08))
	{
		WPC_Function_Status.Charge_Load_In_Tx_Flag = TRUE;
	}
	else if((P9261_Reg_State.TX_STATUS_MESSAGE==0x02)&&(P9261_Reg_State.COMBINED_MESSAGE==0x05))
	{
		WPC_Function_Status.Charge_Load_In_Tx_Flag = TRUE;
	}
	else
	{
		WPC_Function_Status.Charge_Load_In_Tx_Flag = FALSE;
	}
}
/***********************************************************************************************************************
* Function Name: IDT_Unknow
* Description  : This function is IDT_Unknow function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void IDT_Unknow(void)
{
	function_test_flag = 1;
}
/***********************************************************************************************************************
* Function Name: WPC_Processer_Task
* Description  : This function is WPC_Processer_Task function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Processer_Task(void)
{
	Read_P9261_Register_Message();
	switch(WPC_MODE_CASE)
	{
		//---------------------------------------------------------//
		case	WPC_SHUTDOWN_MODE:	// = 0x00 //
		//---------------------------------------------------------//
				WPC_Shutdown_Task();
		break;
		//---------------------------------------------------------//
		case	WPC_NORMAL_MODE:	// = 0x01 //
		//---------------------------------------------------------//
				WPC_Normal_Task();
		break;
		//---------------------------------------------------------//
		case	WPC_SLEEP_MODE:		// = 0x02 //
		//---------------------------------------------------------//
				WPC_Sleep_Task();
		break;
		//---------------------------------------------------------//
		case	WPC_EUT_SLEEP_MODE:	// = 0x03 //
		//---------------------------------------------------------//
				WPC_EUT_Sleep_Task();
		break;
		//---------------------------------------------------------//
		default:
		break;
	}
}
/***********************************************************************************************************************
* Function Name: WPC_Mode_Select
* Description  : This function is WPC_Mode_Select function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void WPC_Mode_Select(void)
{
	if(ACC_Status.Active_On_Flag==TRUE)
	{
		if(Power_Sw.Active_Flag==TRUE)
		{
			if((Door_Status.Door_Open_Flag==FALSE)&&(Trunk_Status.Trunk_Open_Flag==FALSE))
			{
				//*WPC_System_Address = NORMAL_MODE;
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = TRUE;
				WPC_System.Sleep_Mode = FALSE;
				WPC_System.EUT_Sleep_Mode = FALSE;
				WPC_MODE_CASE = WPC_NORMAL_MODE;
			}
			else
			{
				//*WPC_System_Address = EUT_SLEEP_MODE;
				WPC_System.Shutdown_Mode = FALSE;
				WPC_System.Normal_Mode = FALSE;
				WPC_System.Sleep_Mode = FALSE;
				WPC_System.EUT_Sleep_Mode = TRUE;
				WPC_MODE_CASE = WPC_EUT_SLEEP_MODE;
			}
		}
		else
		{
			//*WPC_System_Address = SLEEP_MODE;
			WPC_System.Shutdown_Mode = FALSE;
			WPC_System.Normal_Mode = FALSE;
			WPC_System.Sleep_Mode = TRUE;
			WPC_System.EUT_Sleep_Mode = FALSE;
			WPC_MODE_CASE = WPC_SLEEP_MODE;
		}
	}
	else
	{
		//*WPC_System_Address = SHUTDOWN_MODE;
		WPC_System.Shutdown_Mode = TRUE;
		WPC_System.Normal_Mode = FALSE;
		WPC_System.Sleep_Mode = FALSE;
		WPC_System.EUT_Sleep_Mode = FALSE;
		ACC_Status.On_Change_To_Off_Flag = FALSE;
		WPC_MODE_CASE = WPC_SHUTDOWN_MODE;
	}
}
/***********************************************************************************************************************
* Function Name: MCU_IO_Status_Init
* Description  : This function is MCU_IO_Status_Init function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void MCU_IO_Status_Init(void)
{
	WPC_Mode_Select();
	if(System_Init_Time==CLEAR)
	{
		WCHC01_SYS.INITIAL_END_FLAG = TRUE;
	}
}


