/*******************************************************************************
 *
 * FileName     : 24LC256.h
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
#ifndef _EEPROM24LC1025_H
#define _EEPROM24LC1025_H

/* DEFINE CONSTANTS HERE -----------------------------------------------------*/
/* DEFINE TYPES HERE ---------------------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DEFINITION OF MACROS ------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS --------------------------------------------------*/

/*******************************************************************************
 *
 * Function     : void vOpenI2C_3MHz( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     : Initialize I2C Module
 * Note         : None
 *
 ******************************************************************************/
void vOpenI2C( void );

/*******************************************************************************
 *
 * Function     : void vOpenI2c( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     : Initialize I2C Module
 * Note         : None
 *
 ******************************************************************************/
void vCloseI2C( void );

/*******************************************************************************
 *
 * Function     : void vInitializeEEprom( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     : Initialize I2C Module
 * Note         : None
 *
 ******************************************************************************/
void vInitializeEEprom( void );

 /*******************************************************************************
 *
 * Function     : void vEEWriteByte( unsigned short long lAddres, unsigned char cData )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vEEWriteByte( unsigned short long lAddres, unsigned char cData );

/*******************************************************************************
 *
 * Function     : unsigned char cEEReadByte( unsigned int iAddress )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cEEReadByte( unsigned short long lAddress );





#endif	// #ifndef _UART_H 
 