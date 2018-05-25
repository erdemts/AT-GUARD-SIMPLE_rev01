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
//    OSCCONbits.IRCF2 = 0;
//    OSCCONbits.IRCF1 = 1;
//    OSCCONbits.IRCF0 = 1;
    vSetOscMode(OSC_FREQ_3MHZ);
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
    if(cOscMode == OSC_FREQ_1MHZ)
    {
        OSCCONbits.IDLEN = 0;
//        OSCCONbits.IRCF2 = 0;
//        OSCCONbits.IRCF1 = 1;
//        OSCCONbits.IRCF0 = 1;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS1 = 1;
        OSCCON2bits.PRI_SD = 0;
        OSCTUNEbits.SPLLEN = 0;
        OscClockFreq = OSC_FREQ_1MHZ;
    }
    else if(cOscMode == OSC_FREQ_2MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.IRCF2 = 1;
        OSCCONbits.IRCF1 = 0;
        OSCCONbits.IRCF0 = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS1 = 1;
        OSCCON2bits.PRI_SD = 0;
        OSCTUNEbits.SPLLEN = 0;
        OscClockFreq = OSC_FREQ_2MHZ;
    }
    else if(cOscMode == OSC_FREQ_3MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS1 = 0;
        OSCCON2bits.PRI_SD = 1;
        OSCTUNEbits.SPLLEN = 0;
        OscClockFreq = OSC_FREQ_3MHZ;
    }
    else if(cOscMode == OSC_FREQ_12MHZ)
    {
        OSCCONbits.IDLEN = 0;
        OSCCONbits.SCS0 = 0;
        OSCCONbits.SCS1 = 0;
        OSCCON2bits.PRI_SD = 1;
        OSCTUNEbits.SPLLEN = 1;
        OscClockFreq = OSC_FREQ_12MHZ;
    }
}

