/*******************************************************************************
 *
 * FileName     : 24LC256.c
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
#include "includes.h"

/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
#define CONST_BITS_24FC1025         ( 0b1010 )
#define EEPROM_PAGE_SIZE            ( 128 )
#define MAX_EEPROM_READ_SIZE        ( 65536 )
#define EEPROM_SIZE                 ( 131072 )
#define I2C_WRITE                   ( 0 )
#define I2C_READ                    ( 1 )
/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
typedef union
{
    unsigned char cByte;
    struct
    {
        unsigned RW         :1;
        unsigned A0         :1;
        unsigned A1         :1;
        unsigned B0         :1;
        unsigned CONST_BITS :4;
    };
}CONTROL_BYTE_24FC1025;
/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/

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
void vOpenI2C( void )
{
    SCL_TRIS = 1;         	// set SCL and SDA pins as inputs
    SDA_TRIS = 1;

    SSPSTAT &= 0x3F;                // power on state
    SSPCON1 = 0x00;                 // power on state
    SSPCON2 = 0x00;                 // power on state
    SSPSTAT = 0x80;
    SSPCON1 = 0x08;
    if(OscClockFreq == OSC_FREQ_1MHZ)
    {
        SSPADD = 2;                //For 83KHz, 2@1MHz
    }
    else if(OscClockFreq == OSC_FREQ_3MHZ)
    {
        SSPADD = 7;                //For 93KHz, 7@3MHz
    }
    else if(OscClockFreq == OSC_FREQ_12MHZ)
    {
        SSPADD = 29;                //For 100KHz, 29@12MHz
    }

    SSPCON1bits.SSPEN = 1;
    I2C_POWER = 1;
    Delay1KTCYx( 1 );
}

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
void vCloseI2C( void )
{
    SSPCON1 = 0;
    PIE1bits.SSPIE = 0;
    PIR1bits.SSPIF = 0;

    SDA = 0;
    SCL = 0;
    SDA_TRIS = 0;
    SCL_TRIS = 0;
    SDA = 0;
    SCL = 0;

    I2C_POWER = 0;
    Delay1KTCYx( 3 );
}

/*******************************************************************************
 *
 * Function     : cEEWriteByte( unsigned int iAddress, unsigned char cData )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vEEWriteByte( unsigned short long lAddres, unsigned char cData )
{
    CONTROL_BYTE_24FC1025 cControlByte;
    
    cControlByte.cByte = 0;
    cControlByte.CONST_BITS = CONST_BITS_24FC1025;
    cControlByte.B0 = ( lAddres >> 16 ) & 0x01;
    cControlByte.A0 = 0;
    cControlByte.A1 = 0;
    cControlByte.RW = I2C_WRITE;

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    IdleI2C();
    StartI2C();
    while ( SSPCON2bits.SEN );
    
    WriteI2C( cControlByte.cByte );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );
        
    WriteI2C( ( unsigned char ) ( ( lAddres >> 8 ) & 0xFF ) );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );
    
    WriteI2C( ( unsigned char ) ( lAddres & 0xFF ) );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );
    
    WriteI2C( cData );
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
    	
    	WriteI2C( cControlByte.cByte );
        IdleI2C();
        if( !SSPCON2bits.ACKSTAT )
            break;

        StopI2C();
    	IdleI2C();
    }
    StopI2C();
    while ( SSPCON2bits.PEN ); // wait until stop condition is over
    IdleI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}


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
unsigned char cEEReadByte( unsigned short long lAddresss )
{
    unsigned char cData = 0;
    CONTROL_BYTE_24FC1025 cControlByte;

    cControlByte.cByte = 0;
    cControlByte.CONST_BITS = CONST_BITS_24FC1025;
    cControlByte.B0 = ( lAddresss >> 16 ) & 0x01;
    cControlByte.A0 = 0;
    cControlByte.A1 = 0;
    cControlByte.RW = I2C_WRITE;

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;

    IdleI2C();
    StartI2C();
    while ( SSPCON2bits.SEN );

    WriteI2C( cControlByte.cByte );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    WriteI2C( ( unsigned char )( ( lAddresss >> 8 ) & 0xFF ) );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    WriteI2C( ( unsigned char ) ( lAddresss & 0xFF ) );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    RestartI2C();
    IdleI2C();

    WriteI2C( cControlByte.cByte + 1 );
    IdleI2C();
    while( SSPCON2bits.ACKSTAT );

    cData = ReadI2C();
    IdleI2C();

    StopI2C();
    while ( SSPCON2bits.PEN ); // wait until stop condition is over
    IdleI2C();

    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    return ( cData );
}



/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/

