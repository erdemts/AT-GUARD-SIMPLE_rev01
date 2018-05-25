/*******************************************************************************
 *
 * FileName     : osc.c
 * Dependencies :
 * Description  :
 * Processor    : PIC18F15K50
 * Compiler     : MPLAB C18
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
#include "includes.h"
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
 * Function     :
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vInitializeOSC( void )
{
    OSCCONbits.IDLEN = 0;
    vSetOscMode(OSC_MODE_3MHZ);
}

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
void vSetOscMode( unsigned char cOscMode )
{
    if(cOscMode == OSC_MODE_1MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS0 = 1;
        OSCCON2bits.PRI_SD = 0;
        OSCTUNEbits.SPLLEN = 0;
    }
    else if(cOscMode == OSC_MODE_3MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS0 = 0;
        OSCCON2bits.PRI_SD = 1;
        OSCTUNEbits.SPLLEN = 0;
    }
    else if(cOscMode == OSC_MODE_12MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS0 = 0;
        OSCCON2bits.PRI_SD = 1;
        OSCTUNEbits.SPLLEN = 1;
    }
}

