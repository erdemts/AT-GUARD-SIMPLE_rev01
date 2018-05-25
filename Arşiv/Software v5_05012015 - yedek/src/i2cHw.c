/*******************************************************************************
 *
 * FileName     : i2c.c
 * Dependencies :
 * Description  :
 * Processor    : PIC32
 * Compiler     : MPLAB C32
 * Linker       : MPLAB LINK32
 * Company      : Inno Technology Incorporated
 *
 * Software License Agreement
 * Copyright (C) 2011 - 2012 Inno Technology Inc.  All rights
 * reserved.
 *
 * Version    Author                     Date             Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0        Erdem Tayfun Salman        16/01/2012       Initial Version
 ******************************************************************************/
#include "HardwareProfile.h"
/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/
/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/
 

/*******************************************************************************
 *
 * Function     : void vInitializeI2C( void );
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     : Initialize I2C Module
 * Note         : None
 *
 ******************************************************************************/
void vInitializeI2C( void )
{
    SCL_TRIS = 1;         	// set SCL and SDA pins as inputs
    SDA_TRIS = 1;
    SSPSTAT = 0x80;
    SMP	= 1;				//Enable Slew Rate Control
    SSPCON = 0x28;   		//Enable MSSP Master
    SSPADD = 0x0C;    		//400KHz @20MHz
    SSPCON2 = 0x00;   		//Clear MSSP Conrol Bits
    SSPIF = 0;
    BCLIF = 0;
}

/*******************************************************************************
 *
 * Function     : void IdleI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void IdleI2C( void )
{
    while (( SSPCON2 & 0x1F ) | R_nW );
}

/*******************************************************************************
 *
 * Function     : void StartI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void StartI2C( void )
{
    SEN = 1;
}

/*******************************************************************************
 *
 * Function     : void RestartI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void RestartI2C( void )
{
    RSEN = 1;
}	

/*******************************************************************************
 *
 * Function     : void StopI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void StopI2C( void )
{
    PEN = 1;
}		

/*******************************************************************************
 *
 * Function     : void NotAckI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void NotAckI2C( void )
{
    ACKDT=1;
    ACKEN=1;
}     	

/*******************************************************************************
 *
 * Function     : void AckI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void AckI2C( void )
{
    ACKDT=0;
    ACKEN=1;
}      	

/*******************************************************************************
 *
 * Function     : unsigned char ReadI2C( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char ReadI2C( void )
{
    RCEN = 1;           // enable master for 1 byte reception
    while ( !BF );      // wait until byte received
    return ( SSPBUF );
}

/*******************************************************************************
 *
 * Function     : unsigned char WriteI2C( unsigned char data_out )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char WriteI2C( unsigned char data_out )
{
    SSPBUF = data_out;
    while( BF );
    return ( ACKSTAT );
}
