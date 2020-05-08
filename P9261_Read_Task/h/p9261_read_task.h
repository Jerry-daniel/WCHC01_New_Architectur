/***********************************************************************************************************************
* File Name    : p9261_read_task.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _P9261_READ_TASK_H
#define _P9261_READ_TASK_H
/***********************************************************************************************************************
p9261 definitions
***********************************************************************************************************************/
#define P9261_ADDRESS	0xC2
#define RESET_COIL		0x00
#define COIL_1			0x01
#define COIL_2			0x02
#define COIL_3			0x03
/***********************************************************************************************************************
p9261 IIC Status flag struct define
***********************************************************************************************************************/
struct P9261_IIC_FLAG
{
	unsigned char	read_reg_start_flag			:1;
	unsigned char	iic_send_end_flag			:1;	
	unsigned char	iic_receiver_end_flag		:1;
	unsigned char	next_reg_flag				:1;
	unsigned char	active_flag					:1;
	unsigned char 	reserved_flag				:4;
};
extern struct P9261_IIC_FLAG P9261_IIC_State;

/***********************************************************************************************************************
P9261 0x0690(Tx_State)Register message status enum definitions
***********************************************************************************************************************/
typedef enum
{
	STATE_startup							= 0x00,
	STATE_idle								= 0x01,
	STATE_analog_ping						= 0x02,
	STATE_q_measurement						= 0x03,
	STATE_digital_ping						= 0x04,
	STATE_wpc_identificantion				= 0x05,
	STATE_rx_removed_detection				= 0x06,
	STATE_wpc_configuration					= 0x07,
	STATE_power_transfer_initialization		= 0x08,
	STATE_power_transfer_state				= 0x09,
	STATE_id_confirmation					= 0x0A,
	STATE_remove_power						= 0x0B,
	STATE_restart							= 0x0C,
	STATE_wpc_negotiation 					= 0x0D,
	unknow_state							= 0x0E,
	STATE_wpc_re_negotiation 				= 0x0F,
	STATE_calibration 						= 0x10
}P9261_TRANSMITTER_STATE;
extern P9261_TRANSMITTER_STATE	P9261_TX_STATE_MSG;
/***********************************************************************************************************************
P9261 0x0691(Tx_Status)Register message status enum definitions
***********************************************************************************************************************/
typedef enum
{
	STATUS_ok										= 0x00,
	STATUS_qfod_alarm								= 0x01,
	STATUS_charge_complete_rx_ept_code				= 0x02,
	unknow_status									= 0x03,
	STATUS_no_response_rx_ept_code					= 0x04,
	STATUS_internal_fault_rx_ept_code				= 0x05,
	STATUS_over_temperature_rx_ept_code				= 0x06,
	STATUS_over_current_rx_ept_code					= 0x07,
	STATUS_full_charged								= 0x08,
	STATUS_other_end_of_power_reason_rx_ept_code	= 0x09,
	STATUS_negotiation_failed						= 0x0A,
	STATUS_control_error_packet_timeout				= 0x0B,
	STATUS_received_power_packet_timeout			= 0x0C,
	STATUS_tnext_timing_violation					= 0x0D,
	STATUS_packet_defect_during_id_config			= 0x0E,
	STATUS_lllegal_packet_during_power_transfer		= 0x0F,
	STATUS_tx_extern_over_temperature_limit			= 0x10,
	STATUS_tx_over_voltage_limit					= 0x11,
	STATUS_tx_die_temperature_over_limit			= 0x12,
	STATUS_lllegal_packet_during_id_config			= 0x13,
	STATUS_tx_under_voltage_limit					= 0x14,
	STATUS_wireless_charger_off						= 0x15,
	STATUS_peps_interrupted							= 0x16,
	STATUS_standby									= 0x17,
	STATUS_power_loss_fod_alarm						= 0x18,
	STATUS_open_fod_alarm							= 0x19,
	STATUS_tx_overcurrent_alarm						= 0x1A,
	STATUS_rx_jiggle_alarm							= 0x1B
}P9261_TRANSMITTER_STATUS;
extern P9261_TRANSMITTER_STATUS	P9261_TX_STATUS_MSG;
/***********************************************************************************************************************
P9261 0x06C0(List of Values for Combined Status)Register message status enum definitions
***********************************************************************************************************************/
typedef enum
{
	COMBINED_over_temperature_threshold	= 0x00,
	COMBINED_over_voltage_threshold		= 0x01,
	COMBINED_over_power_from_rx			= 0x02,
	COMBINED_open_fod					= 0x03,
	COMBINED_real_fod					= 0x04,
	COMBINED_charge_complete_from_rx	= 0x05,
	COMBINED_peps_alarm					= 0x06,
	COMBINED_can_message_on_off			= 0x07,
	COMBINED_charging					= 0x08,
	COMBINED_standby					= 0x09,
	COMBINED_under_voltage_threshold	= 0x0A
}P9261_COMBINED_STATUS_VALUE;
extern P9261_COMBINED_STATUS_VALUE P9261_COMBINED_MSG;
/***********************************************************************************************************************
P9261 0x06C0(List of Values for Combined Status) message flag define
***********************************************************************************************************************/
struct P9261_COMBINED_MESSAGE
{
	unsigned int In_Over_Temperature_Threshold	:1;
	unsigned int In_Over_Voltage_Threshold		:1;
	unsigned int In_Over_Power_Rx				:1;
	unsigned int In_Open_FOD					:1;
	unsigned int In_Ploss_FOD					:1;
	unsigned int In_Charge_complete_From_Rx		:1;
	unsigned int In_PEPS_Alarm					:1;
	unsigned int In_CAN_Message_On_Off			:1;
	unsigned int In_Charging					:1;
	unsigned int In_Standby						:1;
	unsigned int In_Under_Voltage_Threshold		:1;	
	unsigned int Reserved						:5;
};
extern struct P9261_COMBINED_MESSAGE P9261_CombinedMessage;

/***********************************************************************************************************************
P9261 State message flag define
***********************************************************************************************************************/
struct P9261_TX_STATE_MESSAGE
{
	unsigned long	Startup_State							:1;
	unsigned long	Idle_State								:1;
	unsigned long	Analog_Ping_State						:1;
	unsigned long	Q_Measurement_State						:1;
	unsigned long	Digital_Ping_State						:1;
	unsigned long	Identification_State					:1;
	unsigned long	Rx_Removed_Detection_State				:1;
	unsigned long	Configuration_State						:1;
	unsigned long	Power_Transfer_Init_State				:1;
	unsigned long	Power_Transfer_State					:1;
	unsigned long	Id_Confirmation_State					:1;
	unsigned long	Remove_Power_State						:1;
	unsigned long	Restart_State							:1;
	unsigned long	Negotiation_State						:1;
	unsigned long	Unknow_State							:1; // test //
	unsigned long	Re_Negotiation_State					:1;
	unsigned long	Calibration_State						:1;
	unsigned long	Reserved_State							:15;
};
extern struct P9261_TX_STATE_MESSAGE	P9261_TxState_Message;
//============================================================================//
struct P9261_TX_STATUS_MESSAGE
{
	unsigned long	Ok_Status								:1;
	unsigned long	Q_FOD_Alarm_Status						:1;
	unsigned long	Charge_Complete_Rx_EPT_Code_Status		:1;
	unsigned long	No_Response_Rx_EPT_Code_Status			:1;
	unsigned long	Internal_Fault_Rx_EPT_Code_Status		:1;
	unsigned long	Rx_OverTemperature_EPT_Code_Status		:1;
	unsigned long	Rx_OverCurrent_EPT_Code_Status			:1;
	unsigned long	Full_Chargeed_Status					:1;
	unsigned long	Other_End_Of_Power_Rx_EPT_Code_Status	:1;
	unsigned long	Negotiation_Failed_Status				:1;
	unsigned long	Control_Error_Packet_Timeout_Status		:1;
	unsigned long	Received_Power_Packet_Timeout_Status	:1;
	unsigned long	Tnext_Timing_Violation_Status			:1;
	unsigned long	Packet_Defect_During_ID_Config_Status	:1;
	unsigned long	Illegal_Packet_During_Power_Tx_Status	:1;
	unsigned long	Tx_External_OverTemp_Limit_Status		:1;
	unsigned long	Tx_OverVoltage_Limit_Status				:1;
	unsigned long	Tx_Die_Temp_OverLimit_Status			:1;
	unsigned long	Illegal_Packet_During_ID_Config_Status	:1;
	unsigned long	Tx_Under_Voltage_Limit_Status			:1;
	unsigned long	Wireless_Charger_Off_Status				:1;
	unsigned long	PEPS_Interrupted_Status					:1;
	unsigned long	Standby_Status							:1;
	unsigned long	Power_Loss_Fod_Alarm_Status				:1;
	unsigned long	Open_FOD_Alarm_Status					:1;
	unsigned long	Tx_OverCurrent_Alarm_Status				:1;
	unsigned long	Rx_Jiggle_Alarm_Status					:1;
	unsigned long	Reserved_Status							:5;
	//unsigned long	Open_Fod_Alarm_Status					:1;
	//unsigned long	Over_Temperature_Alarm_Status			:1;
};
extern struct P9261_TX_STATUS_MESSAGE	P9261_TxStatus_Message;

//============================================================================//
/*
struct P9261_STATE_MESSAGE
{
	unsigned long	Startup_Status							:1;
	unsigned long	Idle_Status								:1;
	unsigned long	Analog_Ping_Status						:1;
	unsigned long	Q_Measurement_Status					:1;
	unsigned long	Digital_Ping_Status						:1;
	unsigned long	Identification_Status					:1;
	unsigned long	Rx_Removed_Detection_Status				:1;
	unsigned long	Configuration_Status					:1;
	unsigned long	Power_Transfer_Init_Status				:1;
	unsigned long	Power_Transfer_Status					:1;
	unsigned long	Id_Confirmation_Status					:1;
	unsigned long	Remove_Power_Status						:1;
	unsigned long	Restart_Status							:1;
	unsigned long	Negotiation_Status						:1;
	unsigned long	Unknow_Status							:1; // test //
	unsigned long	Re_Negotiation_Status					:1;
	unsigned long	Calibration_Status						:1;
	//-----------------------------------------------------//
	unsigned long	Q_Fod_Alarm_Status						:1;
	unsigned long	Open_Fod_Alarm_Status					:1;
	unsigned long	Power_Loss_Fod_Alarm_Status				:1;
	unsigned long	Other_End_Of_Power_Rx_Alarm_Status		:1;
	unsigned long	Charge_Complete_From_Rx_Status			:1;
	unsigned long	Full_Chargeed_Status					:1;
	unsigned long	Rx_Jiggle_Alarm_Status					:1;
	unsigned long	Over_Temperature_Alarm_Status			:1;
	unsigned long	Tx_Under_Voltage_Limit_Status			:1;
	
	unsigned long	No_Response_Rx_Ept_Code_Status			:1;
	unsigned long	Internal_Fault_Rx_Ept_Code_Status		:1;
	unsigned long	Rx_Over_Temperature_Alarm_Status		:1;
	unsigned long	Rx_Over_Current_Ept_Code_Status			:1;
	unsigned long	Received_Power_Packet_Timeout_Status	:1;
	unsigned long	Wireless_Charger_Off_Status				:1;
	
	unsigned long	Reserved_Status							:32;
};
extern struct P9261_STATE_MESSAGE	P9261_RegMessage;
*/
/***********************************************************************************************************************
P9261 register struct definitions
***********************************************************************************************************************/
typedef struct
{
	volatile unsigned char  COIL_NUMBER			:8;
	volatile unsigned char	TX_STATE_MESSAGE	:8;
	volatile unsigned char	TX_STATUS_MESSAGE	:8;
	volatile unsigned char	COMBINED_MESSAGE	:8;
	volatile unsigned char	NTC_ADC_OUT			:8;
	volatile unsigned int   COIL_Q_MESSAGE		:16;
	volatile unsigned int	RPP					:16;
}P9261_REG_STATE;
extern P9261_REG_STATE P9261_Reg_State;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
//extern unsigned int RX_ID;
//extern unsigned int		time_array[20];
//extern unsigned char	q_array[20];
extern unsigned char 	Read_Next_Register_Delay_Time;
extern unsigned char 	COIL_DETECT;
extern unsigned char 	LAST_COIL;
extern unsigned char	Last_P9261_Tx_State_Code;
extern unsigned char 	Last_P9261_Tx_Status_Code;
extern unsigned int		P9261_NTC_ADC_OUT;
extern unsigned int 	COIL_1_Q_Message;
extern unsigned int 	COIL_2_Q_Message;
extern unsigned int 	COIL_3_Q_Message;
extern unsigned int		COIL_Current;
extern unsigned int 	Last_COIL_Current;
extern unsigned char	P9261_Message;
extern unsigned char 	P9261_Address[2];
extern unsigned char	P9261_IIC_FLOW;
//extern unsigned int *P9261_IIC_State_Flag_address;
//extern unsigned int	*P9261_Combined_Flag_address;
//extern unsigned int *P9261_RegMessage_Flag_address;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void Receive_P9261_Tx_State_Task(unsigned char P9261_TX_STATE);
void Receive_P9261_Tx_Status_Task(unsigned char P9261_TX_STATUS);
void Receive_P9261_Combined_Msg_Task(unsigned char P9261_COMBINED);
void Read_P9261_Register_Message(void);
void P9261_Data_Update(void);
void P9261_Read_Cmd_Start(void);
void P9261_Register_Reading(void);
void I2C_Commumication_Start(void);
void OpenFOD_Alarm_Algorithm(void);
void OpenFOD_Detect_Restart(void);
unsigned char Clear_P9261_IIC_State_Flag(unsigned long const *address);
unsigned char Clear_P9261_Combined_State_Flag(unsigned long const *address);
unsigned char Clear_P9261_Tx_State_Flag(unsigned long const *address);
unsigned char Clear_P9261_Tx_Status_Flag(unsigned long const *address);

#endif