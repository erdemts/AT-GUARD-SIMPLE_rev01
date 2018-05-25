/*******************************************************************************
 *
 * FileName     : rtcc.h (MCP79400 driver)
 * Dependencies :
 * Description  :
 * Processor    : PIC18F14K50
 * Compiler     : MPLAB C18
 * Linker       : 
 * Company      : Inno Technology Incorporated
 *
 * Software License Agreement
 * Copyright (C) 2011 - 2012 Inno Technology Inc.  All rights
 * reserved.
 *
 * Version    Author                     Date             Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0        Erdem Tayfun Salman        31/01/2013       Initial Version
 ******************************************************************************/
#ifndef _OSC_H
#define _OSC_H

/* DEFINE CONSTANTS HERE -----------------------------------------------------*/
#define OSC_MODE_USB                ( 1 )
#define OSC_MODE_RF                 ( 2 )
#define OSC_MODE_DATA_PROCCES       ( 3 )
#define OSC_MODE_DEEP_SLEEP         ( 4 )
#define OSC_MODE_12MHZ              ( 5 )
#define OSC_MODE_3MHZ               ( 6 )
#define OSC_MODE_1MHZ               ( 7 )

#define OSC_FREQ_1MHZ               ( 1000000 )
#define OSC_FREQ_3MHZ               ( 3000000 )
#define OSC_FREQ_12MHZ              ( 12000000 )
/* DEFINE TYPES HERE ---------------------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
extern volatile unsigned long int ClockFreq;
/* DEFINITION OF MACROS ------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS --------------------------------------------------*/

/*******************************************************************************
 *
 * Function     :
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vInitializeOSC( void );

/*******************************************************************************
 *
 * Function     :
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vSetOscMode( unsigned char cOscMode );

#endif	// #ifndef _UART_H 
 