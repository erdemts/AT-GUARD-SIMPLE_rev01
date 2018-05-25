/*******************************************************************************
 * 
 * FileName     : 
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
#ifndef _EM4095READER_H
#define _EM4095READER_H

/* DEFINE CONSTANTS HERE -----------------------------------------------------*/
#define F_OSC                           (3000000)// ( 12000000 )
#define F_PWM                            ( 125000 )
/* DEFINE TYPES HERE ---------------------------------------------------------*/
typedef union
{
    volatile unsigned char cAllRFFlags;
    struct
    {
        unsigned bBitValue		:1;
        unsigned bStoreData		:1;
        unsigned bRE_FE_TOGGLE          :1;
        unsigned bRFTimeOutFlag         :1;
        unsigned bNewCard		:1;
        unsigned b5                     :1;
        unsigned b6                     :1;
        unsigned b7                     :1;
    };
}RF_STRUCT;
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
extern RF_STRUCT xRF;
extern volatile unsigned long int ClockFreq;

/* DEFINITION OF MACROS ------------------------------------------------------*/

/* DECLARATION OF FUNCTIONS --------------------------------------------------*/
/*******************************************************************************
 *
 * Function     : void vInitializeRF( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vInitializeRF( void );

/*******************************************************************************
 *
 * Function     : void vRFPowerUp( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFPowerUp( void );

/*******************************************************************************
 *
 * Function     : void vRFPowerDown( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFPowerDown( void );

/*******************************************************************************
 *
 * Function     : void vRFCaptureIsr( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFCaptureIsr( void );

/*******************************************************************************
 *
 * Function     : void vRFTimerIsr( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFTimerIsr( void );

/*******************************************************************************
 *
 * Function     : void vEM4095_StartReading( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cReadEM4102( unsigned char *cBuffer );


#endif	// #ifndef _EM4095_H
