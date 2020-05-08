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
* File Name    : r_cg_serial.c
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
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t   g_iic00_master_status_flag; /* iic00 start flag for send address check by master mode */
volatile uint8_t * gp_iic00_tx_address;        /* iic00 send data pointer by master mode */
volatile uint16_t  g_iic00_tx_count;           /* iic00 send data size by master mode */
volatile uint8_t * gp_iic00_rx_address;        /* iic00 receive data pointer by master mode */
volatile uint16_t  g_iic00_rx_count;           /* iic00 receive data size by master mode */
volatile uint16_t  g_iic00_rx_length;          /* iic00 receive data length by master mode */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS0 = _0001_SAU_CK00_FCLK_1 | _0000_SAU_CK01_FCLK_0;
    R_IIC00_Create();
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Create
* Description  : This function initializes the IIC00 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Create(void)
{
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable IIC00 */
    IICMK00 = 1U;    /* disable INTIIC00 interrupt */
    IICIF00 = 0U;    /* clear INTIIC00 interrupt flag */
    /* Set INTIIC00 low priority */
    IICPR100 = 1U;
    IICPR000 = 1U;
    SIR00 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_CLOCK_MODE_CKS |
            _0000_SAU_TRIGGER_SOFTWARE | _0000_SAU_EDGE_FALL | _0004_SAU_MODE_IIC | _0000_SAU_TRANSFER_END;
    SCR00 = _0000_SAU_TIMING_1 | _0000_SAU_PARITY_NONE | _0000_SAU_MSB | _0010_SAU_STOP_1 | _0007_SAU_LENGTH_8;
    SDR00 = _9E00_IIC00_DIVISOR;
    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1 | _0001_SAU_CH0_DATA_OUTPUT_1;
    /* Set SCL00, SDA00 pin */
    POM1 |= 0x40U;
    P1 |= 0xC0U;
    PM1 &= 0x3FU;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Master_Send
* Description  : This function starts transferring data for IIC00 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iic00_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear IIC00 master status flag */
    adr &= 0xFEU;    /* send mode */
    g_iic00_master_status_flag = _01_SAU_IIC_SEND_FLAG;            /* set master status flag */
    SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR00 |= _8000_SAU_TRANSMISSION;
    /* Set paramater */
    g_iic00_tx_count = tx_num;
    gp_iic00_tx_address = tx_buf;
    /* Start condition */
    R_IIC00_StartCondition();
    IICIF00 = 0U;   /* clear INTIIC00 interrupt flag */
    IICMK00 = 0U;   /* enable INTIIC00 */
    SDR00L = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Master_Receive
* Description  : This function starts receiving data for IIC00 in master mode.
* Arguments    : adr -
*                    set address for select slave
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iic00_master_status_flag = _00_SAU_IIC_MASTER_FLAG_CLEAR;    /* clear master status flag */
    adr |= 0x01U;    /* receive mode */
    g_iic00_master_status_flag = _02_SAU_IIC_RECEIVE_FLAG;         /* set master status flag */
    SCR00 &= ~_C000_SAU_RECEPTION_TRANSMISSION;
    SCR00 |= _8000_SAU_TRANSMISSION;
    /* Set parameter */
    g_iic00_rx_length = rx_num;
    g_iic00_rx_count = 0U;
    gp_iic00_rx_address = rx_buf;
    /* Start condition */
    R_IIC00_StartCondition();
    IICIF00 = 0U;   /* clear INTIIC00 interrupt flag */
    IICMK00 = 0U;   /* enable INTIIC00 */
    SDR00L = adr;
}

/***********************************************************************************************************************
* Function Name: R_IIC00_Stop
* Description  : This function stops the IIC00 operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_Stop(void)
{
    /* Stop transfer */
    IICMK00 = 1U;    /* disable INTIIC00 */
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable IIC00 */
    IICIF00 = 0U;    /* clear INTIIC00 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: R_IIC00_StartCondition
* Description  : This function starts IIC00 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_StartCondition(void)
{
    volatile uint8_t w_count;
    
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;    /* clear IIC00 SDA */
    
    /* Wait 5 us */
    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 &= ~_0100_SAU_CH0_CLOCK_OUTPUT_1;    /* clear IIC00 SCL */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;            /* enable IIC00 output */
    SS0 |= _0001_SAU_CH0_START_TRG_ON;              /* enable IIC00 */
}

/***********************************************************************************************************************
* Function Name: R_IIC00_StopCondition
* Description  : This function stops IIC00 condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IIC00_StopCondition(void)
{
    volatile uint8_t w_count;
    
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;           /* disable IIC00 */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;       /* disable IIC00 output */
    SO0 &= ~_0001_SAU_CH0_DATA_OUTPUT_1;        /* clear IIC00 SDA */
    SO0 |= _0100_SAU_CH0_CLOCK_OUTPUT_1; /* set IIC00 SCL */
    
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= IIC00_WAITTIME; w_count++)
    {
        NOP();
    }
    
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;         /* set IIC00 SDA */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
