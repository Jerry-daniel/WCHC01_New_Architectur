/***********************************************************************************************************************
* File Name    : wpc_application.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _WPC_APPLICATION_H
#define _WPC_APPLICATION_H
//===============================================================================//
//#define _MCU_VDD_3V3_
#define _P9261_PROGRAMMING_MODE_
//#define _WPC_PCBA_VER_1_2_
//#define _OPENFOD_LONG_TIME_
//#define _USED_P9261_INTERNAL_TEMPERATURE_STATUS_
//===============================================================================//
/***********************************************************************************************************************
* Description  : Normal define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
typedef enum
{
	WPC_SHUTDOWN_MODE	= 0x00,	// ACC off //
	WPC_NORMAL_MODE		= 0x01, // ACC ON and Power Sw On //
	WPC_SLEEP_MODE		= 0x02, // ACC ON and Power Sw Off //
	WPC_EUT_SLEEP_MODE	= 0x03,
	WPC_ABNORMAL_MODE	= 0x04
}WPC_SYSTEM_MODE;
/***********************************************************************************************************************
* Description  : IDT State define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
typedef enum
{
	IDT_WPC_STARTUP_TASK						= 0x00,
	IDT_WPC_IDLE_TASK							= 0x01,
	IDT_WPC_ANALOG_PING_TASK					= 0x02,
	IDT_WPC_Q_MEASUREMENT_TASK					= 0x03,
	IDT_WPC_DIGITAL_PING_TASK					= 0x04,
	IDT_WPC_IDENTIFICATION_TASK					= 0x05,
	IDT_WPC_RX_REMOVED_DETECTION_TASK			= 0x06,
	IDT_WPC_CONFIGURATION_TASK					= 0x07,
	IDT_WPC_POWER_TRANSFER_INIT_TASK			= 0x08,
	IDT_WCP_POWER_TRANSFER_TASK					= 0x09,
	IDT_WPC_ID_CONFIRMATION_TASK				= 0x0A,
	IDT_WPC_REMOVE_POWER_TASK					= 0x0B,
	IDT_WPC_RESTART_TASK						= 0x0C,
	IDT_WPC_NEGOTIATION_TASK					= 0x0D,
	IDT_UNKNOW									= 0x0E,
	IDT_WPC_RE_NEGOTIATION_TASK					= 0x0F,
	IDT_WPC_CALIBRATION_TASK					= 0x10
	//-------------------------------------------------------------------//
	/*IDT_WPC_FOD_ALARM_TASK						= 0x11,
	IDT_WPC_POWER_SW_OFF_TASK					= 0x12,
	IDT_WPC_CELLPHONE_LEFT_ALERT_TASK			= 0x13,
	IDT_WPC_ACC_OFF_TASK						= 0x14,
	IDT_WPC_EUT_SLEEP_TASK						= 0x15,
	IDT_WPC_OVERTEMPERATURE_TASK				= 0x16,
	IDT_WPC_RX_JIGGLE_ALARM_TASK				= 0x17,
	IDT_WPC_OTHER_EOF_PWR_RX_ALARM_TASK			= 0x18,
	IDT_WPC_TX_UNDER_VOLTAGE_LIMIT_TASK 		= 0x19,
	IDT_WPC_RX_FAIL_EPT_CODE_TASK				= 0x1A,//<== 20191114 add //
	IDT_WPC_OVERCURRENT_TASK					= 0x1B,
	IDT_WPC_RECEIVE_POWER_PACKET_TIMEOUT_TASK	= 0x1C,
	IDT_WPC_WIRELESS_CHARGER_OFF_TASK			= 0x1D*/
}IDT_WPC_STATE_TASK;
extern IDT_WPC_STATE_TASK	IDT_WPC_TASK_NUM;
/***********************************************************************************************************************
* Description  : Port name input/output define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
#define P8_4			(((volatile __near __bitf_T *)0xFF08)->no4)
#define CHARGE_EN		P8_4

#define P8_1			(((volatile __near __bitf_T *)0xFF08)->no1)
#define TEST_TP2		P8_1

#define P8_0			(((volatile __near __bitf_T *)0xFF08)->no0)
#define TEST_TP3		P8_0

#define P1_2			(((volatile __near __bitf_T *)0xFF01)->no2)
#define TEST_TP4		P1_2

#define P1_1			(((volatile __near __bitf_T *)0xFF01)->no1)
#define TEST_TP5		P1_1

#define P1_0			(((volatile __near __bitf_T *)0xFF01)->no0)
#define TEST_TP6		P1_0

#define P8_3			(((volatile __near __bitf_T *)0xFF08)->no3)
#define ACC_ON_RL78		P8_3

#define CHARGE_DISABLE	{CHARGE_EN = 0;}
#define CHARGE_ENABLE	{CHARGE_EN = 1;}
/***********************************************************************************************************************
WCHC01 system mode Status initial flag struct define
***********************************************************************************************************************/
struct WCHC01_SYSTEM_FLAG
{
	volatile unsigned char INITIAL_END_FLAG	:1;
	volatile unsigned char RESREVED			:7;
};extern struct WCHC01_SYSTEM_FLAG WCHC01_SYS;
/***********************************************************************************************************************
wpc system mode Status flag struct define
***********************************************************************************************************************/
struct WPC_SYSTEM_MODE_FLAG
{
	volatile unsigned char	Shutdown_Mode	:1;
	volatile unsigned char	Normal_Mode		:1;
	volatile unsigned char	Sleep_Mode		:1;
	volatile unsigned char  EUT_Sleep_Mode	:1;
	volatile unsigned char	Reserved		:4;
};
extern struct WPC_SYSTEM_MODE_FLAG	WPC_System;

/***********************************************************************************************************************
wpc function Status flag struct define
***********************************************************************************************************************/
struct WPC_FUNCTION_STATUS_FLAG
{
	unsigned int 	ACC_Off_Dly3_5T_Flag				:1;
	unsigned int	Temperature_Detect_Start_Flag		:1;
	unsigned int 	IDT_Program_Debug_Mode_Flag			:1;
	unsigned int 	P9261_ReStart_End_Flag				:1;
	unsigned int 	OpenFOD_Detect_Time_Flag			:1;	// 20200113 //
	unsigned int 	Over_Temperature_Alarm_Status		:1;
	unsigned int	P9261_State_Update_End_Flag			:1;
	unsigned int 	Forget_Remove_End_Flag				:1;
	unsigned int	Exception_Handing_Flag				:1;
	unsigned int	Hold_Open_FOD_Flag					:1;
	unsigned int	First_Open_FOD_Flag					:1;
	unsigned int	Charge_Load_In_Tx_Flag				:1;
	unsigned int 	Reserved							:4;
};
extern struct WPC_FUNCTION_STATUS_FLAG WPC_Function_Status;

/***********************************************************************************************************************
wpc function Status flag struct define
***********************************************************************************************************************/
struct WPC_ABNORMAL_ALARM_STATUS_FLAG
{
	unsigned char	P9261_Error_Alarm_Flag			:1;
	unsigned char 	Rx_OverTemp_Alarm_Flag			:1;
	unsigned char 	Tx_Extern_OverTemp_Alarm_Flag	:1;
	unsigned char 	Rx_Charge_Complete_Alarm_Flag	:1;
	unsigned char 	Rx_Full_Charged_Alarm_Flag		:1;
	unsigned char 	Reserved						:3;
};
extern struct WPC_ABNORMAL_ALARM_STATUS_FLAG Abnormal_Event;

//==================================================================//
/*
struct WPC_OTHER_ALARM_STATUS_FLAG
{
	unsigned char	Other_Eof_Pwr_Rx_Alarm_Status_Flag	:1;
	unsigned char 	Rx_Jiggle_Status_Flag				:1;
	unsigned char 	Rx_Ept_Code_Flag					:1;
	unsigned char 	Receiver_Timeout_Flag				:1;
	unsigned char	Wireless_Charge_Off_Flag			:1;
	unsigned char 	Rx_Over_Temperature_Flag			:1;	 //20200218 modify //
	unsigned char	Reserved							:2;
};
extern struct WPC_OTHER_ALARM_STATUS_FLAG	Other_Alarm;
*/
/***********************************************************************************************************************
IDT WPC Status flag struct define
***********************************************************************************************************************/
struct IDT_WPC_FLAG
{
	unsigned int   	Startup_Task_Flag						:1;
	unsigned int	Idle_Task_Flag							:1;
	unsigned int	Analog_Ping_Task_Flag 					:1;
	unsigned int	Q_Measurement_Task_Flag 				:1;
	unsigned int	Digital_Ping_Task_Flag					:1;
	unsigned int	Identification_Task_Flag				:1;
	unsigned int	Rx_Removed_Detection_Task_Flag			:1;
	unsigned int	Configuration_Task_Flag					:1;
	unsigned int	Power_Transfer_Init_Task_Flag			:1;
	unsigned int	Power_Transfer_Task_Flag				:1;
	unsigned int	Id_Confirmation_Task_Flag				:1;
	unsigned int	Remove_Power_Task_Flag					:1;
	unsigned int	Restart_Task_Flag						:1;
	unsigned int	Negotiation_Task_Flag					:1;
	unsigned int	Re_Negotiation_Task_Flag				:1;
	unsigned int	Calibration_Task_Flag					:1;
	//-----------------------------------------------------------//
	/*unsigned long	Fod_Alarm_Task_Flag						:1;
	unsigned long	Power_Sw_Off_Task_Flag					:1;
	unsigned long	Cellphone_Left_Alert_Task_Flag			:1;
	unsigned long	Acc_Off_Task_Flag						:1;
	unsigned long	EUT_Sleep_Task_Flag						:1;
	unsigned long	Over_Temperature_Alarm_Task_Flag		:1;
	unsigned long	Rx_Jiggle_Alarm_Task_Flag				:1;
	unsigned long	Other_Eof_Pwr_Rx_Alarm_Task_Flag		:1;
	unsigned long	Tx_Under_Voltage_Limit_Task_Flag		:1;
	//------------------------------------------------------------// 20191114 add //
	unsigned long	Rx_Fail_Ept_Code_Task_Flag				:1;
	unsigned long	Rx_Over_Temperature_Alarm_Task_Flag		:1;
	unsigned long	Rx_Over_Current_Ept_Code_Task_Flag		:1;
	unsigned long	Received_Power_Packet_Timeout_Task_Flag	:1;
	unsigned long	Wireless_Charger_Off_Task_Flag			:1;
	unsigned long	Reserved_Flag							:2;*/
};
extern struct IDT_WPC_FLAG IDT_WPC_State;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
//extern unsigned char p9261_led_state;
//extern unsigned int caculator_time;
extern unsigned int	UVOL_Wait_Time;
//extern unsigned int	MCU_Initial_Dly_Time;
//extern unsigned char Coil_Current_Count;
//extern unsigned int Door_Close_Waitting_Time;
extern unsigned char FOD_Alarm_Count;
extern unsigned int Read_I2C_Data_Duty_Time;
extern unsigned int I2C_DATA_CYCLE_COUNT;

extern unsigned int EUT_Waitting_Time;
extern unsigned int ACC_Off_Reset_Time;
extern unsigned int Over_Temperature_Reset_Time;
extern volatile unsigned char IDT_WPC_TASK;
extern volatile unsigned char IDT_WPC_NEXT_TASK;
//extern unsigned int *IDT_WPC_State_address;
//extern unsigned int *WPC_Function_State_Flag_address;

extern volatile unsigned int	*WPC_System_Address;
//-------------------------------------------------------------------------------//
extern unsigned char System_Init_Time;
extern volatile unsigned char WPC_MODE_CASE;
extern volatile unsigned char WPC_NEXT_MODE_CASE;


//-------------------------------------------------------------------------------//
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void IDT_WPC_Startup_Task(void);
void IDT_WPC_Idle_Task(void);
void IDT_WPC_Analog_Ping_Task(void);
void IDT_WPC_Q_Measurement_Task(void);
void IDT_WPC_Digital_Ping_Task(void);
void IDT_WPC_Identification_Task(void);
void IDT_WPC_Rx_Removed_Detection_Task(void);
void IDT_WPC_Configuration_Task(void);
void IDT_WPC_Power_Transfer_Init_Task(void);
void IDT_WCP_Power_Transfer_Task(void);
void IDT_WPC_Id_Confirmation_Task(void);
void IDT_WPC_Remove_Power_Task(void);
void IDT_WPC_Restart_Task(void);
void IDT_WPC_Negotiation_Task(void);
void IDT_Unknow(void);
void IDT_WPC_Re_Negotiation_Task(void);
void IDT_WPC_Calibration_Task(void);
void IDT_FOD_Alarm_Task(void);
void IDT_Power_Sw_Off_Task(void);
void IDT_Cellphone_Left_Alert_Task(void);
void IDT_ACC_Off_Task(void);
void IDT_EUT_Sleep_Task(void);
void IDT_Temperature_Alarm_Task(void);
void IDT_RX_Jiggle_Alarm_Task(void);
void IDT_Other_Eof_Pwr_Rx_Alarm_Task(void);
void IDT_TX_Under_Voltage_Limit_Task(void);
//-----------------------------------------------------//
void IDT_RX_Fail_Ept_Code_Task(void);
void IDT_Current_Alarm_Task(void);
void IDT_Receive_Power_Packet_Timeout_Task(void);
void IDT_Wireless_Charger_Off_Task(void);

void Reset_Buzzer_LED_State(void);
void IDT_WPC_Processer_Task(void);
void WPC_System_Initial(void);
void IO_Interface_Init(void);
void Variable_Init(void);
void P9261_Reg_Msg_Init(void);
void P9261_Restart_Init(void);
void I2C_Variable_Init(void);
void Phone_Placement_Detect(void);
void OverTemperature_Detect(void);
void OpenFOD_Alarm_Active_Detect(void);
void ChargeLoan_In_Tx_Check(void); // 20200511 //
//-------------------------------------------------------------------------//
void IDT_P9261_TX_Status_Processer(void);
void WPC_Processer_Task(void);
void WPC_Shutdown_Task(void);
void WPC_Abnormal_Alarm_Task(void);
void WPC_Sleep_Task(void);
void WPC_EUT_Sleep_Task(void);
void WPC_Normal_Task(void);
void WPC_Mode_Select(void);
void MCU_IO_Status_Init(void);
void WPC_Sleep_Reset(void);
void Abnormal_Alarm_Output_Event(void);
//-------------------------------------------------------------------------//
unsigned char Clear_IDT_WPC_State_Flag(unsigned long const *address);
unsigned char Clear_WPC_Function_State_Flag(unsigned long const *address);
unsigned char Clear_Abnormal_Event_Flag(unsigned long const *address);

unsigned char Mapping_WPC_System_Address(unsigned long *address);
unsigned char Clear_Flag_Group(unsigned long const *address);
//-------------------------------------------------------------------------//
void Engineer_Mode_Task(void);
#endif