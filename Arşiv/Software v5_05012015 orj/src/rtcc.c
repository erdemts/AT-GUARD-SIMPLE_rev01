/*******************************************************************************
 *
 * FileName     : rtcc.c (MCP79400 driver)
 * Dependencies : rtcc.h
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
#include "includes.h"

/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
extern TIME_STRUCT xTime;
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
unsigned char cBcdToDec( unsigned char cBcd );
unsigned char cDecToBcd( unsigned char cDec );
/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/

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
void vInitializeRTCC( void )
{
    unsigned char cVar = 0;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    cVar = cReadRTCC( ADDR_HOUR );
    vWriteRTCC( ADDR_HOUR, cVar & HOUR_24 );

    cVar = cReadRTCC( ADDR_DAY );
    vWriteRTCC( ADDR_DAY, cVar | VBATEN );

    vGetTimeDate( &xTime );
    if( ( ( xTime.DAY_OF_WEEK == 0 ) || ( xTime.DAY_OF_WEEK > 7 ) ) || ( ( xTime.DATE == 0 ) || ( xTime.DATE > 31 ) ) || ( ( xTime.MONTH == 0 ) || ( xTime.MONTH > 12 ) ) )
    {
        cVar = cReadRTCC( ADDR_DAY );
        vWriteRTCC( ADDR_DAY, cVar | VBATEN );

        vWriteRTCC( ADDR_YEAR, 0x15 );
        vWriteRTCC( ADDR_MNTH, 0x01 );
        vWriteRTCC( ADDR_DATE, 0x01 );
        vWriteRTCC( ADDR_HOUR, 0x00 );
        vWriteRTCC( ADDR_MIN, 0x00 );
        vWriteRTCC( ADDR_SEC, START_32KHZ );
    }

    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
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
void vWriteRTCC( unsigned char cRtccReg, unsigned char Var )
{
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    IdleI2C();
    StartI2C();
    while ( SSPCON2bits.SEN );

    WriteI2C( ADDR_RTCC_WRITE );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    WriteI2C( cRtccReg );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    WriteI2C( Var );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    StopI2C();
    while ( SSPCON2bits.PEN ); // wait until stop condition is over
    IdleI2C();

    //Acknowledge Polling
    while( 1 )
    {
        StartI2C();
    	IdleI2C();

    	WriteI2C( ADDR_RTCC_WRITE );
        IdleI2C();
        if( !SSPCON2bits.ACKSTAT )
            break;

        StopI2C();
    	IdleI2C();
    }
    StopI2C();
    while ( SSPCON2bits.PEN ); // wait until stop condition is over
    IdleI2C();
//    Delay1KTCYx( 70 );
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

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
unsigned char cReadRTCC( unsigned char cRtccReg )
{    
    unsigned char cData;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    IdleI2C();
    StartI2C();
    while ( SSPCON2bits.SEN );

    WriteI2C( ADDR_RTCC_WRITE );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    WriteI2C( cRtccReg );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    RestartI2C();
    IdleI2C();

    WriteI2C( ADDR_RTCC_READ );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    cData = ReadI2C();
    IdleI2C();

    StopI2C();
    while ( SSPCON2bits.PEN ); // wait until stop condition is over
    IdleI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

//    Delay1KTCYx( 70 );
    return ( cData );
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
void vSetTimeDate( TIME_STRUCT* cTimeVar )
{
    unsigned char cTemp;

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    if( ( cTimeVar->DATE == 0 ) || ( cTimeVar->DATE > 31 ) || ( cTimeVar->DAY_OF_WEEK == 0 ) || ( cTimeVar->DAY_OF_WEEK > 7 ) || ( cTimeVar->MONTH == 0 ) || ( cTimeVar->MONTH > 12 ) )
        return;

    cTemp = cDecToBcd( cTimeVar->DAY_OF_WEEK );
    cTemp &= 0x07;
    cTemp |= 0x08; //Bit4 (VBATEN) must be 1
    vWriteRTCC( ADDR_DAY, cTemp );
    cTemp = cDecToBcd( cTimeVar->SEC );
    cTemp = cTemp | 0x80;
    vWriteRTCC( ADDR_SEC, cTemp );
    vWriteRTCC( ADDR_MIN, cDecToBcd( cTimeVar->MIN ) );
    cTemp = cDecToBcd( cTimeVar->HOUR );
    cTemp = cTemp & 0x3F;
    vWriteRTCC( ADDR_HOUR , cTemp );
    vWriteRTCC( ADDR_DATE , cDecToBcd( cTimeVar->DATE ) );
    vWriteRTCC( ADDR_MNTH , cDecToBcd( cTimeVar->MONTH ) );
    vWriteRTCC( ADDR_YEAR , cDecToBcd( cTimeVar->YEAR ) );

    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    vCloseI2C();
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
void vGetTimeDate( TIME_STRUCT* cTimeVar )
{
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();
    cTimeVar->SEC = cBcdToDec( ( cReadRTCC( ADDR_SEC ) & 0x7F ) );
    cTimeVar->MIN = cBcdToDec( cReadRTCC( ADDR_MIN ) );
    cTimeVar->HOUR = cBcdToDec( ( cReadRTCC( ADDR_HOUR ) & 0x3F ) );
    cTimeVar->DAY_OF_WEEK = cBcdToDec( ( cReadRTCC( ADDR_DAY ) & 0x07 ) );
    cTimeVar->DATE = cBcdToDec( cReadRTCC( ADDR_DATE ) );
    cTimeVar->MONTH = cBcdToDec( ( cReadRTCC( ADDR_MNTH ) & 0x1F ) );
    cTimeVar->YEAR = cBcdToDec( cReadRTCC( ADDR_YEAR ) );
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    vCloseI2C();
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
void vPeriodicalyCheckRTCC( void )//periyodik olarak oscon kontrol edilir. DurmuÅŸsa baÅŸlatÄ±lÄ±r. AyrÄ±ca zaman verileri SRAM 'a kaydedilir. 2 sn'de bir last time kontrol edilir.
                                  //Last time
{

}



/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/

unsigned char cBcdToDec( unsigned char cBcd )
{
    cBcd = ( cBcd & 0b00001111 ) + ( cBcd >> 4 ) * 10;
    return cBcd;
}

unsigned char cDecToBcd( unsigned char cDec )
{
    unsigned char i = 0;

    if ( cDec > 9 )
    {
        i = ( cDec / 10 ) << 4;
        i = i | ( cDec % 10 );
    }
    else
    {
        i = cDec;
    }
    return i;
}