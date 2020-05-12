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
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/F13 V2.03.04.01 [17 May 2019]
* Device(s)    : R5F10BAG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Serial module.
* Creation Date: 2020/4/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "p9261_read_task.h"
//#include "led_task.h"
#include "wpc_application.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_iic00_interrupt(vect=INTIIC00)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t   g_iic00_master_status_flag;  /* iic00 start flag for send address check by master mode */
extern volatile uint8_t * gp_iic00_tx_address;         /* iic00 send data pointer by master mode */
extern volatile uint16_t  g_iic00_tx_count;            /* iic00 send data size by master mode */
extern volatile uint8_t * gp_iic00_rx_address;         /* iic00 receive data pointer by master mode */
extern volatile uint16_t  g_iic00_rx_count;            /* iic00 receive data size by master mode */
extern volatile uint16_t  g_iic00_rx_length;           /* iic00 receive data length by master mode */
/* Start user code for global. Do not edit comment generated here */
unsigned int ntc_adc_tmp;
unsigned int coil_q_tmp;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_iic00_interrupt
* Description  : This function is INTIIC00 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_iic00_interrupt(void)
{
    if (((SSR00 & _0002_SAU_PARITY_ERROR) == 0x0002U) && (g_iic00_tx_count != 0U))
    {
        r_iic00_callback_master_error(MD_NACK);
    }
    else if(((SSR00 & _0001_SAU_OVERRUN_ERROR) == 0x0001U) && (g_iic00_tx_count != 0U))
    {
        r_iic00_callback_master_error(MD_OVERRUN);
    }
    else
    {
        /* Control for master send */
        if ((g_iic00_master_status_flag & _01_SAU_IIC_SEND_FLAG) == 1U)
        {
            if (g_iic00_tx_count > 0U)
            {
                SDR00L = *gp_iic00_tx_address;
                gp_iic00_tx_address++;
                g_iic00_tx_count--;
            }
            else
            {
                R_IIC00_StopCondition();
                r_iic00_callback_master_sendend();
            }
        }
        /* Control for master receive */
        else 
        {
            if ((g_iic00_master_status_flag & _04_SAU_IIC_SENDED_ADDRESS_FLAG) == 0U)
            {
                ST0 |= _0001_SAU_CH0_STOP_TRG_ON;
                SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
                SCR00 |= _4000_SAU_RECEPTION;
                SS0 |= _0001_SAU_CH0_START_TRG_ON;
                g_iic00_master_status_flag |= _04_SAU_IIC_SENDED_ADDRESS_FLAG;

                if (g_iic00_rx_length == 1U)
                {
                    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable IIC00 out */
                }

                SDR00L = 0xFFU;
            }
            else
            {
                if (g_iic00_rx_count < g_iic00_rx_length)
                {
                    *gp_iic00_rx_address = SDR00L;
                    gp_iic00_rx_address++;
                    g_iic00_rx_count++;
                    
                    if (g_iic00_rx_count == (g_iic00_rx_length - 1U))
                    {
                        SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable IIC00 out */
                        SDR00L = 0xFFU;
                    }
                    else if (g_iic00_rx_count == g_iic00_rx_length)
                    {
                        R_IIC00_StopCondition();
                        r_iic00_callback_master_receiveend();
                    }
                    else
                    {
                        SDR00L = 0xFFU;
                    }
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_error
* Description  : This function is a callback function when IIC00 master error occurs.
* Arguments    : flag -
*                    status flag
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
	g_iic00_tx_count = 0;
	SIR00 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;
	R_IIC00_StopCondition();
    r_iic00_callback_master_sendend();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_receiveend
* Description  : This function is a callback function when IIC00 finishes master reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
	if(P9261_IIC_FLOW==0xD0)
	{
		P9261_Reg_State.TX_STATE_MESSAGE = (P9261_Message&0x1F);
		P9261_TX_STATE_MSG = P9261_Reg_State.TX_STATE_MESSAGE;
		P9261_Address[0] = 0x06;
		P9261_Address[1] = 0x91;
		P9261_IIC_FLOW = 0xD1;
	}
	else if(P9261_IIC_FLOW==0xD1)
	{
		P9261_Message &= 0x7F;	// filter high bit (bit 7) //
		if(P9261_Message>0x1F){P9261_Message = CLEAR;} // Tx_Status data filter processer - 20200424 //
		P9261_Reg_State.TX_STATUS_MESSAGE = (P9261_Message&0x1F);
		P9261_TX_STATUS_MSG = P9261_Reg_State.TX_STATUS_MESSAGE;
		P9261_Address[0] = 0x06;
		P9261_Address[1] = 0xC0;
		P9261_IIC_FLOW = 0xD2;
	}
	else if(P9261_IIC_FLOW==0xD2)
	{
		P9261_Reg_State.COMBINED_MESSAGE = (P9261_Message&0x0A);
		P9261_COMBINED_MSG = P9261_Reg_State.COMBINED_MESSAGE;
		P9261_Address[0] = 0x06;
		P9261_Address[1] = 0xAF;
		P9261_IIC_FLOW = 0xD3;
	}
	else if(P9261_IIC_FLOW==0xD3)
	{
		P9261_Reg_State.NTC_ADC_OUT = (P9261_Message&0x0F);
		ntc_adc_tmp = P9261_Reg_State.NTC_ADC_OUT;
		ntc_adc_tmp = ntc_adc_tmp<<8;
		P9261_Address[0] = 0x06;
		P9261_Address[1] = 0xAE;
		P9261_IIC_FLOW = 0xD4;
	}
	//-------capture coil q data ---------------------------------------------//
	else if(P9261_IIC_FLOW==0xD4)
	{
		P9261_Reg_State.NTC_ADC_OUT = P9261_Message;
		ntc_adc_tmp |= P9261_Reg_State.NTC_ADC_OUT;
		P9261_NTC_ADC_OUT = ntc_adc_tmp;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x2D;
		P9261_IIC_FLOW = 0xD5;
	}
	else if(P9261_IIC_FLOW==0xD5)
	{
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Message;
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Reg_State.COIL_Q_MESSAGE<<8;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x2C;
		P9261_IIC_FLOW = 0xD6;
	}
	else if(P9261_IIC_FLOW==0xD6)
	{
		P9261_Reg_State.COIL_Q_MESSAGE |= P9261_Message;
		if(P9261_Reg_State.COIL_Q_MESSAGE==CLEAR) {P9261_Reg_State.COIL_Q_MESSAGE = 0xFFFF;}
		COIL_1_Q_Message = P9261_Reg_State.COIL_Q_MESSAGE;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x39;
		P9261_IIC_FLOW = 0xD7;
	}
	else if(P9261_IIC_FLOW==0xD7)
	{
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Message;
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Reg_State.COIL_Q_MESSAGE<<8;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x38;
		P9261_IIC_FLOW = 0xD8;
	}
	else if(P9261_IIC_FLOW==0xD8)
	{
		P9261_Reg_State.COIL_Q_MESSAGE |= P9261_Message;
		if(P9261_Reg_State.COIL_Q_MESSAGE==CLEAR) {P9261_Reg_State.COIL_Q_MESSAGE = 0xFFFF;}
		COIL_2_Q_Message = P9261_Reg_State.COIL_Q_MESSAGE;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x45;
		P9261_IIC_FLOW = 0xD9;
	}
	else if(P9261_IIC_FLOW==0xD9)
	{
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Message;
		P9261_Reg_State.COIL_Q_MESSAGE = P9261_Reg_State.COIL_Q_MESSAGE<<8;
		P9261_Address[0] = 0x08;
		P9261_Address[1] = 0x44;
		P9261_IIC_FLOW = 0xDA;
	}
	else if(P9261_IIC_FLOW==0xDA)
	{
		P9261_Reg_State.COIL_Q_MESSAGE |= P9261_Message;
		if(P9261_Reg_State.COIL_Q_MESSAGE==CLEAR) {P9261_Reg_State.COIL_Q_MESSAGE = 0xFFFF;}
		COIL_3_Q_Message = P9261_Reg_State.COIL_Q_MESSAGE;
		P9261_Address[0] = 0x06;
		P9261_Address[1] = 0x90;
		P9261_IIC_FLOW = 0xD0;
		P9261_IIC_State.iic_receiver_end_flag = TRUE;
		//P9261_IIC_State.read_reg_start_flag = FALSE;
	}else{}
	P9261_IIC_State.next_reg_flag = TRUE;
	Read_Next_Register_Delay_Time = NEXT_DATA_TIME;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iic00_callback_master_sendend
* Description  : This function is a callback function when IIC00 finishes master transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iic00_callback_master_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
	R_IIC00_Master_Receive(P9261_ADDRESS,&P9261_Message,1);// i2c - read //
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
