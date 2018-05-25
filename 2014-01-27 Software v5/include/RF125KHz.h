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

#define	TRUE				( 1 )
#define	FALSE				( 0 )
#define CCP_CAPTURE_RE                  ( 5 )
#define CCP_CAPTURE_FE                	( 4 )

#define DELAY3MHZ			( 2 ) // For TMR3 no prescaler
#define DELAY12MHZ			( 6 ) // For TMR3 no prescaler
#define DELAY20MHZ			( 6 )
#define DELAY48MHZ			( 12 )
#define RF_DELAY			( DELAY3MHZ )

#define RFBUFFER_SIZE 			( 30 )
#define END_OF_RFBUFFER 		( cRFBuffer_index == sizeof( cRFBuffer ) )
/* DEFINE TYPES HERE ---------------------------------------------------------*/
// cRFBuffer[0] = 0x00;
//    cRFBuffer[1] = 0xF8;
//    cRFBuffer[2] = 0x4F;
//    cRFBuffer[3] = 0x13;
//    cRFBuffer[4] = 0x00;
//    cRFBuffer[5] = 0x9D;
//    cRFBuffer[6] = 0x01;
//    cRFBuffer[7] = 0x03;
//    cRFBuffer[8] = 0xB0;
//    cRFBuffer[9] = 0xF8;
//    cRFBuffer[10] = 0x4F;
//    cRFBuffer[11] = 0x13;
//    cRFBuffer[12] = 0x00;
//    cRFBuffer[13] = 0x9D;
//    cRFBuffer[14] = 0x01;
//    cRFBuffer[15] = 0x03;
//    cRFBuffer[16] = 0xB0;
//    cRFBuffer[17] = 0xF8;
//    cRFBuffer[18] = 0x4F;
//    cRFBuffer[19] = 0x13;
//    cRFBuffer[20] = 0x00;
//    cRFBuffer[21] = 0x9D;
//    cRFBuffer[22] = 0x01;
//    cRFBuffer[23] = 0x03;
//    cRFBuffer[24] = 0xB0;
//    cRFBuffer[25] = 0xF8;
//    cRFBuffer[26] = 0x4F;
//    cRFBuffer[27] = 0x13;
//    cRFBuffer[28] = 0x00;
//    cRFBuffer[29] = 0x9D;
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
extern unsigned char cBitVal;
extern unsigned char cDataTransferred;
extern volatile unsigned char	cTimerOverflows;
extern unsigned int iOldClock;
extern volatile unsigned char cRFBuffer[ RFBUFFER_SIZE ];
extern unsigned char cRFBuffer_index;
extern unsigned char cRFBuffer_bitIndex;

extern unsigned char cCardId[ 5 ];


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

/*******************************************************************************
 *
 * Function     : void vShiftRight( unsigned char* pcData, unsigned char cBitValue )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vShiftRight( unsigned char* pcData, unsigned char cBitValue );

void vRFBufferFill( unsigned char data );
void vRFBufferInvert( void );

unsigned char cHeaderSearchEM4102( void );
unsigned char cDecodeDataEM4102( unsigned char* data );
void vMirrorData( unsigned char* data );
#endif	// #ifndef _EM4095_H
