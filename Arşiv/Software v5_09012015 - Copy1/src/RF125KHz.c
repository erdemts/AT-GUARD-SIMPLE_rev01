/*******************************************************************************
 * 
 * FileName     : 
 * Dependencies : 
 * Description  :
 * Processor    : 
 * Compiler     : 
 * Linker       : 
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




/* Define RF pins in HardwareProfile.h
#define RF_POWER_TRIS       ( TRISCbits.TRISC4 )
#define RF_POWER            ( LATCbits.LATC4 )
#define RF_PWM_OUT_TRIS     ( TRISCbits.TRISC5 )
#define RF_PWM_OUT          ( LATCbits.LATC5 )
#define RF_DEMOD_OUT        ( PORTCbits.RC1 )         // Data read in interrupt service routine
#define RF_DEMOD_OUT_TRIS   ( TRISCbits.TRISC1 )         // Data read in interrupt service ro
*/

/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
#define bit_set( var, bitno ) 		( ( var ) |= 1 << ( bitno ) )
#define bit_clear( var, bitno ) 	( ( var ) &= ~( 1 << ( bitno ) ) )
#define bit_test( data, bitno ) 	( ( data >> bitno )& 0x01 )

/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
RF_STRUCT xRF;
unsigned char cBitVal;
unsigned char cDataTransferred = 0;
volatile unsigned char	cTimerOverflows = 0;
unsigned int iOldClock = 0;

volatile unsigned char cRFBuffer[ RFBUFFER_SIZE ];
unsigned char cRFBuffer_index = 0;
unsigned char cRFBuffer_bitIndex = 0;

/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
void vOpenPWM( void );
void vRFReadEdge( char edge );
unsigned char cRFGet( unsigned char cNumBits);
void vRFBufferFill( unsigned char data );
void vRFBufferInvert( void );
char cRFBufferGetBit( void );
void vShiftRight( unsigned char* pcData, unsigned char cBitValue );
void vShiftRightArray( unsigned char* data, unsigned char byte, unsigned char bit_value );
unsigned char cHeaderSearchEM4102( void );
unsigned char cDecodeDataEM4102( unsigned char* data );
void vMirrorData( unsigned char* data );

/* DEFINE FUNCTIONS HERE -----------------------------------------------------*/
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
void vInitializeRF( void )
{
    xRF.bBitValue = TRUE;
    xRF.bStoreData = TRUE;
    xRF.bRE_FE_TOGGLE = 1;
    xRF.bRFTimeOutFlag = 0;

    RF_POWER_TRIS = 0;
    RF_PWM_OUT_TRIS = 0;
    RF_DEMOD_OUT_TRIS = 1;
    RF_PWM_OUT = 1;
    RF_POWER = 0;

    //INT1 used to capture edge of RF_DEMOD_OUT signal
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1IP = 1;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 0;

    //TMR0 for measuring pulse width
    T0CON = 0b11000001;
    TMR0L = 0;
    TMR0H = 0;
    INTCON2bits.TMR0IP = 0;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 0;

    //TMR3 for timeout
    T3CON = 0b00000001; //1:8 Prescaler, Timer3 off
    T3CONbits.TMR3ON = 0;
    IPR2bits.TMR3IP = 0;
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 0;    // Disable TMR3 interrupt
}

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
void vRFPowerUp( void )
{
    RF_PWM_OUT = 1;
    RF_POWER = 1;
//    REFCON0 = 0b11100000;
//    REFCON1 = 0xE8;
//    REFCON2 = 0x0F;
    REFCON1 = 0xE0;
    REFCON2 = 0b00001100;
//    vOpenPWM();
    T2CON = 0b00000000;
    PIE1bits.TMR2IE = 0;//Bunu uygun biryerde 1 kez yapalim
    CCP1CON = 0b00001100;
    CCPR1L  = 0b00000011;
    PR2 = 5;
    T2CONbits.TMR2ON = 1;
//    Delay100TCYx(1);

    TMR3H = 0;
    TMR3L = 0;
    T3CONbits.TMR3ON = 1;
    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
 }

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
void vRFPowerDown( void )
{
    RF_POWER = 0;
    ClosePWM1();
    RF_PWM_OUT = 1;
    REFCON1 = 0;
    REFCON2 = 0;

    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 0;
    T3CONbits.TMR3ON = 0;
    RF_PWM_OUT = 1;
}


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
void vRFTimerIsr( void )
{
    ++cTimerOverflows;
}

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
unsigned char cReadEM4102( unsigned char *cBuffer )
{
    unsigned char i,k;

    vRFPowerUp();
//    Delay100TCYx(50);
    cRFGet( sizeof( cRFBuffer ) * 8);
    vRFPowerDown();

    for( i = 0 ; i < 2 ; ++i )
    {
//        ClrWdt();
        while( !END_OF_RFBUFFER )
        {
            if( cHeaderSearchEM4102() )       // Try to find 9 consecutive 1s
            {
                if( cDecodeDataEM4102( cBuffer ) )  // Try to decode the data after the header
                {
                    vMirrorData( cBuffer );
                    vRFBufferFill( 0xAA );
//                    vBufferFill( cRFBuffer, 0xAA );     // Prevents false detection
                    return TRUE;             // Return sucessful read
                }
            }            
        }
        vRFBufferInvert();                // Invert the buffer because the
        cRFBuffer_bitIndex = 0;
        cRFBuffer_index = 0;
    }
    vRFBufferFill( 0xAA );
//    vBufferFill( cRFBuffer, 0xAA );
    
    return FALSE;
}

/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/
/*******************************************************************************
 *
 * Function     : void vOpenPWM( unsigned char cClockSource )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vOpenPWM( void )
{
    OpenTimer2( TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1 );
    SetOutputPWM1( SINGLE_OUT, PWM_MODE_1 );
    SetDCPWM1( 48 );
    OpenPWM1( 23 );
}

/*******************************************************************************
 *
 * Function     : void RF_readEdge( char edge )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFReadEdge( char edge )
{
   if( edge )
   {
       INTCON2bits.INTEDG1 = 1;
       xRF.bRE_FE_TOGGLE = 1;
   }
   else
   {
       INTCON2bits.INTEDG1 = 0;
       xRF.bRE_FE_TOGGLE = 0;
   }
}

/*****************************************************************************
 * Function:        char RF_get( char numBits, char edgee );
 *
 * PreCondition:    None
 *
 * Input:           1) The number of bits to read
 *		    2) TRUE start on rising edge, FALSE start on falling edge
 *
 * Output:          The number of bits read. Could be used to check for timeout
 *
 * Side Effects:    None
 *
 * Overview:        Fill the buffer with data read from the basestation
 *
 * Note:
 *****************************************************************************/
unsigned char cRFGet( unsigned char cNumBits )
{
//    vRFReadEdge( cEdgee );
    INTCON2bits.INTEDG1 = 1;
    xRF.bStoreData     = TRUE;
    xRF.bBitValue      = 0;
    cRFBuffer_index    = 0;
    cRFBuffer_bitIndex = 0;
    cDataTransferred   = 0;
    cTimerOverflows    = 0;
    iOldClock          = 0;

    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
//    TMR0H = 0;
//    TMR0L = 0;

    while( ( cDataTransferred < cNumBits ) && ( cTimerOverflows < RF_DELAY ) );

    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 0;
    cRFBuffer_index = 0;
    cRFBuffer_bitIndex = 0;
    return cDataTransferred;
}

/*******************************************************************************
 *
 * Function     : void vRFBufferFill( char data )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFBufferFill( unsigned char data )
{
    char i;
    
    unsigned char* buffer;
    buffer = cRFBuffer;
    for( i = sizeof( cRFBuffer ); i > 0  ; i-- )
    {
        buffer[i-1] = data;
    }
   
}

/*******************************************************************************
 *
 * Function     : void vRFBufferInvert( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vRFBufferInvert( void )
{
    char i;
    
    for( i = sizeof( cRFBuffer ) ; i > 0 ; --i )
    {
        cRFBuffer[ i - 1 ] = ~cRFBuffer[ i - 1 ];
    }
}



/*******************************************************************************
 *
 * Function     : char vRFbufferGetBit( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
char cRFBufferGetBit( void )
{
    if( !END_OF_RFBUFFER )
    {
        cBitVal = bit_test( cRFBuffer[ cRFBuffer_index ], cRFBuffer_bitIndex );
	if( ++cRFBuffer_bitIndex == 8 )
	{
            ++cRFBuffer_index;
            cRFBuffer_bitIndex = 0;
        }
    }
    return cBitVal;
}

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
void vShiftRight( unsigned char* pcData, unsigned char cBitValue )
{
    *pcData >>= 1;

    if( cBitValue )
    {
        *pcData |= 0b10000000;
    }
}

/*******************************************************************************
 *
 * Function     : void shift_right( unsigned char* data, unsigned char byte, unsigned char bit_value )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vShiftRightArray( unsigned char* data, unsigned char byte, unsigned char bit_value )
{
    char carry;
    char i;

    for( i = byte ; i > 1 ; --i)
    {
        carry = bit_test( *( data + ( i - 2 ) ), 0 );
        *( data + ( i - 1 ) ) = *( data + ( i - 1 ) ) >> 1;
        if( carry )	*( data + ( i - 1 ) ) = *( data + ( i - 1 ) ) | 0b10000000;
        else		*( data + ( i - 1 ) ) = *( data + ( i - 1 ) ) & 0b01111111;
    }
    *data = *data >> 1;
    if( bit_value )	*( data ) = *( data ) | 0b10000000;
    else			*( data ) = *( data ) & 0b01111111;
}

/*******************************************************************************
 *
 * Function     : char HeaderSearchEM4102( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cHeaderSearchEM4102( void )
{
    char bitCounter = 0;

   // Loops until 9 consecutive 1s are found
   // or the end of the receive buffer is reached
   while( !END_OF_RFBUFFER )
   {
      if( cRFBufferGetBit() == 1 )
      {
         if( ++bitCounter == 9 )
         {
            return TRUE;
         }
      }
      else
      {
         bitCounter = 0;
      }
   }
   return FALSE;
}

/*******************************************************************************
 *
 * Function     : char cDecodeDataEM4102( char* data )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cDecodeDataEM4102( unsigned char* data )
{
    char bitVal		= 0;
    char count 		= 1;
    char parity		= 0;
    char colParity1	= 0;
    char colParity2 	= 0;
    char colParity3 	= 0;
    char colParity4 	= 0;

    // Loop until 40 bits of data and 10 bits of parity are received
    for( count = 1 ; count <= 50 ; ++count )
    {
        bitVal = cRFBufferGetBit();
        if( END_OF_RFBUFFER )
        {
            return FALSE;
        }

        if( count % 5 == 0 )
        {
            // Check for row parity
            if( parity != bitVal )
            {
                return FALSE;
            }
            parity = 0;
        }
        else
        {
            // Store a bit of data
            vShiftRightArray( data, 5, bitVal );

            // Calculate row parity
            parity = parity ^ bitVal;

            // Calculate column parity
            switch ( count % 5 )
            {
                case 1: colParity1 = colParity1 ^ bitVal; break;
                case 2: colParity2 = colParity2 ^ bitVal; break;
                case 3: colParity3 = colParity3 ^ bitVal; break;
                case 4: colParity4 = colParity4 ^ bitVal; break;
            }
        }
    }

    // Check for column parity
    if( colParity1 != cRFBufferGetBit() || colParity2 != cRFBufferGetBit() ||
        colParity3 != cRFBufferGetBit() || colParity4 != cRFBufferGetBit() )
    {
        return FALSE;
    }

    // Check for stop bit
    if( cRFBufferGetBit() != 0 )
    {
        return FALSE;
    }

    // Prevents reading all zeros for customer ID and tag ID
    for( count=0 ; count < 5 && data[ count ] == 0 ; ++count );
    if(count == 5)
    {
        return FALSE;
    }

    // Return TRUE if no errors in decoding received transponder data
    return TRUE;
}

/*******************************************************************************
 *
 * Function     : void vMirroData( char* data )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vMirrorData( unsigned char* data )
{
    unsigned char i,j,k;
    unsigned char AD[ 5 ] = { 0,0,0,0,0 };

    for( k = 0 ; k < 5 ; ++k )
    {
        j = 7;
        for( i = 0 ; i < 8 ; ++i )
        {
            if( bit_test( data[ k ],i ) )
            bit_set( AD[ k ], j );
            else
            bit_clear( AD[ k ], j );
            --j;
        }
    }
    for( i = 0 ; i < 5 ; ++i )
    {
        data[ 4 - i ] = AD[ i ];
    }
}





























///*******************************************************************************
// *
// * Function     : void EM4095_isr_rtcc( void )
// * PreCondition : None
// * Input        : None
// * Output       : None
// * Side Effects : None
// * Overview     :
// * Note         : None
// *
// ******************************************************************************/
//void EM4095_isr_rtcc( void )
//{
//    ++cTimerOverflows;
//
//    if( ( cTimerOverflows == RF_DELAY ) && ( xRF.bRFTimeOutFlag ) )
//    {
//        cRFState = SM_RF_DATA_READY;
//        xRF.bRFTimeOutFlag = 0;
//        cTimerOverflows = 0;
//        INTCON3bits.INT1IF = 0;
//        INTCON3bits.INT1IE = 0;
//        cRFBuffer_index = 0;
//        cRFBuffer_bitIndex = 0;
//    }
//}
//







///*******************************************************************************
// *
// * Function     : void vRfTimeOut( void )
// * PreCondition : None
// * Input        : None
// * Output       : None
// * Side Effects : None
// * Overview     :
// * Note         : None
// *
// ******************************************************************************/
//void vRFTimeOut( void )
//{
//	cTimerOverflows = 0;
//	xRF.bRFTimeOutFlag = 1;
//}
//




///*******************************************************************************
// *
// * Function     : unsigned char cReadCard( unsigned char *cBuffer );
// * PreCondition : None
// * Input        : None
// * Output       : None
// * Side Effects : None
// * Overview     :
// * Note         : None
// *
// ******************************************************************************/
//unsigned char cReadCard( unsigned char *cBuffer )
//{
//	if( cReadEM4102( cBuffer ) )
//	{
//		xRF.bNewCard = 1;
//	}
//
//    if( ( xRF.bRFIdle ) && ( cRFReadingState == RF_WAIT_IDLE ) )
//	{
//	    cRFReadingState = RF_IDLE;
//	    xRF.bNewCard = 0;
//	    return FALSE;
//	}
//
//    if( ( xRF.bNewCard ) && ( cRFReadingState == RF_IDLE ) )
//    {
//	    xRF.bNewCard = 0;
//        cRFReadingState = RF_WAIT_IDLE;
//        return TRUE;
//	}
//	return FALSE;
//}


