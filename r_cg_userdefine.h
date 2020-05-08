/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/F13 V2.03.04.01 [17 May 2019]
* Device(s)    : R5F10BAG
* Tool-Chain   : CCRL
* Description  : This file includes user definition.
* Creation Date: 2020/4/24
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#define CLEAR					0x00
#define LEVEL_LOW				0U
#define LEVEL_HIGH				1U
#define ON          			0U
#define OFF         			1U

#define TRUE					1U
#define FALSE					0U

#define OPEN					1U
#define CLOSE					0U

#define ACC_ON					0U
#define ACC_OFF					1U

#define LED_ON					1U
#define LED_OFF					0U
#define COIL_1					0x01
#define COIL_2					0x02
#define COIL_3					0x03
#define SAMPLE_END				0x0A
#define RESET_VALUE				0xFF
#define RESET_TEMPERATURE_VALUE	0xFFFF
#define MAX_CYCLE_COUNT			0xFFFF
#define Q_DEFAULT				0xFFFF


#define ON_100ms				100
#define ON_200ms				200
#define ON_250ms				250
#define ON_300ms				300
#define ON_500ms				500
#define ON_600ms				600
#define ON_1000ms				1000


#define OFF_100ms				100
#define OFF_200ms				200
#define OFF_250ms				250
#define OFF_300ms				300
#define OFF_400ms				400
#define OFF_500ms				500
#define	OFF_800ms				800
#define OFF_1000ms				1000
#define OFF_1900ms				1900

#define DUTY_CYCLE_1			0x01
#define DUTY_CYCLE_2			0x02
#define DUTY_CYCLE_3			0x03
#define DUTY_CYCLE_4			0x04		
#define DUTY_CYCLE_5			0x05

#define DELAY_1S				1000
#define DELAY_2S				2000
#define DELAY_2_09S				2090
#define DELAY_3S				3000
#define DELAY_3_5S				3500
#define HOLD_5S_TIME			5000
#define FOD_DETECT_TIME			6000

#define I2C_DUTY_TIME			200
#define NEXT_DATA_TIME			30
#define CNT_X_0_5T				20


#define POWER_SW_PRESS_TIME		20
#define POWER_SW_HOLD_MAX_TIME	50//100

#define DOOR_TRUNK_DEBOUNCE_TIME_10ms	10
#define DOOR_TRUNK_DEBOUNCE_TIME_200ms	200
#define DOOR_TRUNK_DEBOUNCE_TIME_1000ms	1000 // for BCI test //
#define DEBOUNCE_TIME_200MS				200
#define NTC_DEFAULT_TEMPERATURE	0xFFF
#define NTC_OVER_TEMPERATURE	0x333	//Max_temperature:65C
#define NTC_DOWN_TEMPERATURE	0x4A7	//Min_temperature:50C

//#define NTC_OVER_TEMPERATURE	0x1B7 	//Max_temperature:92C
//#define NTC_DOWN_TEMPERATURE	0x30E	//Min_temperature:68C

#define HOLD_OVERTIME				0xFFFF

#define SHUTDOWN_MODE	0x01
#define NORMAL_MODE		0x02
#define SLEEP_MODE		0x04
#define EUT_SLEEP_MODE	0x08

//extern unsigned int Engineer_Count_Time;
//extern unsigned char Door_Change_Cnt;


/* End user code. Do not edit comment generated here */
#endif
