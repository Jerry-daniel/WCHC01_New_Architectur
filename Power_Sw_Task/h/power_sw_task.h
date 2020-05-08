/***********************************************************************************************************************
* File Name    : power_sw_task.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _POWER_SW_TASK_H
#define _POWER_SW_TASK_H
/***********************************************************************************************************************
* Description  : Normal define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/

/***********************************************************************************************************************
* Description  : Port name input/output define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
#define P3_0			(((volatile __near __bitf_T *)0xFF03)->no0)
#define POWER_SW		P3_0
/***********************************************************************************************************************
power_sw Status flag struct define
***********************************************************************************************************************/
struct	POWER_SW_FLAG
{
	unsigned char	Last_Edge_State					:1;
	unsigned char	Negative_Edge_State				:1;
	unsigned char 	Negative_Edge_Detect_End_Flag	:1;
	unsigned char 	Low_Level_Hold_Flag				:1;
	unsigned char	Active_Flag						:1;
	unsigned char	Press_Key_Overtime_Flag			:1;
	unsigned char 	reserved_flag					:2;
};
extern struct POWER_SW_FLAG	Power_Sw;
/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern unsigned int Power_Sw_Hold_Time;
//extern unsigned int *Power_Sw_State_Flag_address;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void Power_Sw_Detection(void);
unsigned char Clear_Power_Sw_State_Flag(unsigned long const *address);

#endif