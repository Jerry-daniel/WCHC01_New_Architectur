/***********************************************************************************************************************
* File Name    : trunk_task.h
* Version      : CodeGenerator for RL78/F13 V2.03.03.01 [28 Oct 2018]
* Device(s)    : R5F10BMG
* Tool-Chain   : CCRL
* Description  : This file includes wpc_application definition.
* Creation Date: 2019/9/1
* Author	   : Daniel.Hsu
***********************************************************************************************************************/
#ifndef _TRUNK_TASK_H
#define _TRUNK_TASK_H
/***********************************************************************************************************************
* Description  : Normal define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/

/***********************************************************************************************************************
* Description  : Port name input/output define.
* Creation Date: 2019/7/30
***********************************************************************************************************************/
#define P1_3			(((volatile __near __bitf_T *)0xFF01)->no3)
#define TRUNK_EN		P1_3
/***********************************************************************************************************************
Trunk Status flag struct define
***********************************************************************************************************************/
struct TRUNK_STATUS_FLAG
{
	unsigned char   Trunk_Open_Flag				:1;
	unsigned char 	reserved_flag				:7;
};
extern struct TRUNK_STATUS_FLAG Trunk_Status;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern unsigned int Trunk_State_Change_Time;
//extern unsigned int *Trunk_State_Flag_address;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void Trunk_En_Detection(void);
unsigned char Clear_Trunk_State_Flag(unsigned long const *address);

#endif