/***********************************************************************************************************************
* File Name    : led_task.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _LED_TASK_H
#define _LED_TASK_H
/***********************************************************************************************************************
* Description  : Normal define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/

/***********************************************************************************************************************
* Description  : Port name input/output define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
#define P12_5			(((volatile __near __bitf_T *)0xFF0C)->no5)
#define P9261_LED1		P12_5

#define P8_2			(((volatile __near __bitf_T *)0xFF08)->no2)
#define POWER_SW_LED1	P8_2

#define P8_6			(((volatile __near __bitf_T *)0xFF08)->no6)
#define LEDB			P8_6

#define P8_7			(((volatile __near __bitf_T *)0xFF08)->no7)
#define LEDA			P8_7

#define P4_1			(((volatile __near __bitf_T *)0xFF04)->no1)
#define POWER_OFF_IDN	P4_1

#define P1_4			(((volatile __near __bitf_T *)0xFF01)->no4)
#define CHARGE_LED		P1_4

#define P1_5			(((volatile __near __bitf_T *)0xFF01)->no5)
#define ERROR_LED		P1_5
//--------------------------------------------------------------------//
#define CHARGE_LED_ON					{CHARGE_LED = 1;ERROR_LED = 0;}
#define CHARGE_LED_OFF					{CHARGE_LED = 0;ERROR_LED = 0;}
#define ERROR_LED_ON					{CHARGE_LED = 0;ERROR_LED = 1;}
#define ERROR_LED_OFF					{CHARGE_LED = 0;ERROR_LED = 0;}
#define CHARGE_STATE_LED_OFF			{CHARGE_LED = 0;ERROR_LED = 0;}
#define POWER_SW_GREEN_LED_ON			{LEDB = 1;LEDA = 0;}
#define POWER_SW_RED_LED_ON				{LEDB = 0;LEDA = 1;}
#define POWER_SW_LED_OFF				{LEDB = 0;LEDA = 0;}
#define KEYBOARD_PWR_SW_GREEN_LED_ON	{POWER_OFF_IDN = 0;POWER_SW_LED1 = 1;}
#define KEYBOARD_PWR_SW_RED_LED_ON		{POWER_SW_LED1 = 0;POWER_OFF_IDN = 1;}
#define KEYBOARD_PWR_SW_LED_OFF			{POWER_OFF_IDN = 0;POWER_SW_LED1 = 0;}
/***********************************************************************************************************************
LED Status flag struct define
***********************************************************************************************************************/
struct LED_STATUS_FLAG
{
	unsigned char	Alarm_LED_Active_End_Flag		:1;
	unsigned char   LED_Working_End_Flag			:1;
	unsigned char   Alarm_LED_En_Flag				:1;
	unsigned char 	Warring_LED_Active_Flag			:1;
	unsigned char	Alarm_LED_Active_Flag			:1;
	unsigned char 	Rx_OverTemp_LED_Trigger_Flag	:1;
	unsigned char	Error_LED_Trigger_Flag			:1;
	unsigned char 	reserved_Flag					:1;
};
extern struct LED_STATUS_FLAG LED_Status;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern unsigned int led_time;
extern unsigned int LED_On_Time;
extern unsigned int LED_Off_Time;
extern unsigned char LED_Flash_Duty;
extern unsigned int *Led_State_Flag_address;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void Error_LED_Update(void);
void Error_LED_Flash_Out(void);
void Error_LED_Flash_Task(void);
void Error_LED_Alarm_Event(void);
void Alarm_LED_Disable(void);

void Rx_OverTemperature_LED_Alarm_Task(void);
void Rx_OverTemperature_LED_Flash_Out(void);

void IDT_Monitor_Mode_LED(void);
unsigned char Init_Led_State_Flag(unsigned long const *address,unsigned int flag_set);

#endif