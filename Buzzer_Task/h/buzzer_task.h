/***********************************************************************************************************************
* File Name    : buzzer_task.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _BUZZER_TASK_H
#define _BUZZER_TASK_H
/***********************************************************************************************************************
* Description  : Normal define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/

/***********************************************************************************************************************
* Description  : Port name input/output define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
#define P8_5			(((volatile __near __bitf_T *)0xFF08)->no5)
#define BUZZER			P8_5
/***********************************************************************************************************************
Buzzer Status flag struct define
***********************************************************************************************************************/
struct BUZZER_STATUS_FLAG
{
	unsigned int	Active_End_Flag				:1;
	unsigned int	Cycle_Delay_End_Flag		:1;
	unsigned int	Buzzer_Working_End_Flag		:1;
	unsigned int   	Alarm_En_Flag				:1; // form buzzer duty on flag //
	unsigned int	PWM_Go_Flag					:1;	// from pwm buzzer ON trigger flag //
	unsigned int	Cycle_Delay_Start_Flag		:1;
	unsigned int 	Charge_Start_Flag			:1; // form wpc power charge start buzzer trigger flag //
	unsigned int 	Charge_Complete_Flag		:1; // form wpc power charge complete buzzer trigger flag //
	unsigned int	Error_Alarm_Active_Flag		:1;
	unsigned int	Forget_Alarm_Active_Flag	:1;
	unsigned int	Error_Buzzer_Trigger_Flag	:1;	// from wpc error or phone forger alarm buzzer trigger flag //
	unsigned int	Forget_Buzzer_Tigger_Flag	:1;
	unsigned int	reserved_Flag				:4;
};
extern struct BUZZER_STATUS_FLAG Buzzer_State;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern unsigned int Buzzer_On_Time;
extern unsigned int Buzzer_Off_Time;
extern unsigned int Buzzer_Cycle_Delay_Time;
extern unsigned char Buzzer_Active_Cycle_Time;
extern unsigned int *Buzzer_State_Flag_address;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void Error_Alarm_Buzzer_Task(void);
void PhoneForger_Buzzer_Task(void);
void Buzzer_Disable(void);

void Charge_Starting_Buzzer_Out(void);	/*20190807 add */
void Charge_Starting_Buzzer_Task(void);	/*20190807 add */

void Charger_Complete_Buzzer_Out(void);
void Charger_Complete_Buzzer_Task(void);

void Error_State_Buzzer_Out(void);
void Error_Buzzer_Update(void);			/*20190807 add */
void Buzzer_Cycle_Delay(void);
void Error_Buzzer_Alarm_Event(void);
void PhoneForger_Buzzer_Alarm_Event(void);

void PhoneForget_Buzzer_Out(void);
void PhoneForger_Buzzer_Update(void);	/*20190807 add */

unsigned char Init_Buzzer_State_Flag(unsigned long const *address,unsigned int flag_set);


#endif