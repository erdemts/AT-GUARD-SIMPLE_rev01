/*******************************************************************************
 * 
 * FileName     : main.c
 * Dependencies : 
 * Description  :
 * Processor    : PIC18LF14K50
 * Compiler     : MPLAB C18 v3.43
 * Linker       : 
 * Company      : Inno Technology Incorporated
 *
 * Software License Agreement
 * Copyright (C) 2011 - 2012 Inno Technology Inc.  All rights
 * reserved.
 * 
 * Version    Author                     Date             Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 2.0        Erdem Tayfun Salman        16/01/2012       Initial Version
 ******************************************************************************/
/** INCLUDES *******************************************************/
#include "includes.h"

/* CONFIGURATION -------------------------------------------------------------*/
#pragma config CPUDIV = CLKDIV4
//#pragma config CPUDIV = NOCLKDIV
#pragma config USBDIV = OFF
#pragma config FOSC   = HS
//#pragma config FOSC   = IRC
#pragma config PCLKEN = OFF
#pragma config PLLEN  = OFF
#pragma config FCMEN  = OFF
#pragma config IESO   = ON
#pragma config PWRTEN = OFF
//#pragma config BOREN  = ON
#pragma config BOREN  = OFF
#pragma config BORV   = 19
#pragma config WDTEN  = ON
//#pragma config WDTPS  = 128
#pragma config WDTPS  = 256     //1 second weak-up
//#pragma config WDTPS  = 2048
#pragma config MCLRE  = ON
#pragma config HFOFST = ON
#pragma config STVREN = ON
#pragma config LVP    = OFF
#pragma config XINST  = ON
#pragma config BBSIZ  = OFF
#pragma config CP0    = ON
#pragma config CP1    = ON
#pragma config CPB    = ON
#pragma config WRT0   = OFF
#pragma config WRT1   = OFF
#pragma config WRTB   = OFF
#pragma config WRTC   = ON
#pragma config EBTR0  = OFF
#pragma config EBTR1  = OFF
#pragma config EBTRB  = OFF

 
/* DEFINE LOCAL CONSTANTS HERE -----------------------------------------------*/
#define STX                     ( 0x0F )
#define ETX                     ( 0x04 )

#define TIMEOUT                 ( 45 )

#define USER                    ( 1 )
#define NODE                    ( 2 )
#define EVENT                   ( 3 )
#define ALL                     ( 4 )

#define PASS                    ( 1 )
#define DENY                    ( 2 )
#define MEMORY_FULL             ( 3 )

#define MAX_USER_COUNT          ( 1024ul )
#define MAX_NODE_COUNT          ( 1024ul )
#define MAX_EVENT_COUNT         ( 1024ul )
#define MAX_TAG_LOG_COUNT       ( 16000ul )
#define MAX_SENSOR_LOG          ( 1000ul )
#define MAX_ALARM_PER_DAY       ( 100ul )
#define ALARM_CHECK_PERIODE     ( 100ul )

#define USER_START_ADDRESS      ( 0ul )                                                         //< 0 - 3072 > + 3 Byte pad
#define NODE_START_ADDRESS      ( USER_START_ADDRESS + ( MAX_USER_COUNT * 3ul ) + 3ul )         //< 3075 - 6147 > + 3 Byte pad
#define EVENT_START_ADDRESS     ( NODE_START_ADDRESS + ( MAX_NODE_COUNT * 3ul ) + 3ul )         //< 6150 - 9222 > + 3 Byte pad
#define LOG_START_ADDRESS       ( EVENT_START_ADDRESS + ( MAX_EVENT_COUNT * 3ul ) + 3ul )       //< 9225 - 121225 > + 7 Byte pad
#define SENSOR_LOG_START_ADDRESS ( LOG_START_ADDRESS + ( MAX_TAG_LOG_COUNT * 7ul ) + 7ul )      //< 121232 - 126232 > + 5 Byte pad
#define ALARM_START_ADDRESS     ( SENSOR_LOG_START_ADDRESS + ( MAX_SENSOR_LOG * 5ul ) + 5ul )   //< 126237 - 128344 >

//RF definitions
#define	TRUE				( 1 )
#define	FALSE				( 0 )
#define CCP_CAPTURE_RE                  ( 5 )
#define CCP_CAPTURE_FE                	( 4 )

#define DELAY1MHZ			( 2 ) // For TMR3 no prescaler
#define DELAY3MHZ			( 2 ) // 
#define DELAY12MHZ			( 6 ) // For TMR3 no prescaler
#define DELAY20MHZ			( 6 )
#define DELAY48MHZ			( 12 )
#define RF_DELAY			( DELAY3MHZ )

#define RFBUFFER_SIZE 			( 20 )
#define END_OF_RFBUFFER 		( cRFBuffer_index == sizeof( cRFBuffer ) )

#define SM_CHECK_RF                     ( 1 )
#define SM_READ_TAG                     ( 2 )


/* DEFINE LOCAL TYPES HERE ---------------------------------------------------*/
typedef union
{
    unsigned char cAllFlags;
    struct
    {
        unsigned bTimeOutFlag           :1;
        unsigned bTimeOutEnable         :1;
        unsigned bUsbDedect		:1;
        unsigned bUsbPlugged		:1;
        unsigned bUserTagReaded     	:1;
        unsigned bAlarmOn          	:1;
        unsigned bCheckAlarm    	:1;
        unsigned b8			:1;
    };
}FLAG_STRUCT;

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
        unsigned bRFDedect              :1;
        unsigned b6                     :1;
        unsigned b7                     :1;
    };
}RF_STRUCT;


/* DEFINE LOCAL MACROS HERE --------------------------------------------------*/
#define	EX_CARD ( ( cCardId[ 2 ] == cExCardId[ 2 ] ) && ( cCardId[ 3 ] == cExCardId[ 3 ] ) && ( cCardId[ 4 ] == cExCardId[ 4 ] ) )
#define bit_set( var, bitno ) 		( ( var ) |= 1 << ( bitno ) )
#define bit_clear( var, bitno ) 	( ( var ) &= ~( 1 << ( bitno ) ) )
#define bit_test( data, bitno ) 	( ( data >> bitno )& 0x01 )

/* DEFINE LOCAL VARIABLES HERE -----------------------------------------------*/
#pragma romdata serial_no_section = 0x1000
const rom int mySerialNumber = 0x3039;
#pragma romdata

#pragma udata
TIME_STRUCT xTime;

/* DEFINE RF VARIABLES HERE --------------------------------------------------*/
RF_STRUCT xRF;
unsigned char cBitVal;
unsigned char cDataTransferred = 0;
volatile unsigned char	cTimerOverflows = 0;
unsigned int iOldClock = 0;

volatile unsigned char cRFBuffer[ RFBUFFER_SIZE ];
unsigned char cRFBuffer_index = 0;
unsigned char cRFBuffer_bitIndex = 0;

unsigned char cRFTimeOut;
/*----------------------------------------------------------------------------*/

volatile FLAG_STRUCT xFlags;
unsigned char cCardId[ 5 ];
unsigned char cExCardId[ 5 ];
volatile unsigned char cTimeOutCounter;

volatile unsigned int iUserTagCount;
volatile unsigned int iNodeTagCount;
volatile unsigned int iEventTagCount;
volatile unsigned int iTagLogCount;
volatile unsigned int iSensorLogCount;
volatile unsigned int iTransferCounter;
volatile unsigned char cAlarmCheckCounter = 0;      //AlarmÃ„Â±n ne kadar zamanda bir kontrol edileceÃ„Å¸ini tutar
volatile unsigned char cAlarmRunningTimer = 0;      //AlarmÃ„Â±n ÃƒÂ§alma sÃƒÂ¼resini tutar
volatile unsigned char cAlarmTime;                  //AlarmÃ„Â±n ne kadar sÃƒÂ¼re ÃƒÂ§alÃ„Â±Ã…Å¸acaÃ„Å¸Ã„Â±nÃ„Â± tutar
volatile unsigned char cAlarmCountForCurrentDay;
volatile unsigned char cExAlarmMinute;

unsigned char cRxBufferR;
unsigned char cCommand;
unsigned char cDataSize;
unsigned char cDataBuffer[ 64 ];
unsigned char cDataBufferR;
unsigned char cDataBufferW;
unsigned int iCRC;
unsigned int iCRCReceived;
unsigned short long slEEpromAddress;

volatile unsigned char MachineState;
volatile unsigned char cWeakUpState;
volatile unsigned char cWeakUpCounter = 0;
volatile unsigned char RFState;

volatile unsigned char OscClockFreq;
volatile unsigned char k;
volatile unsigned char cWidth;





#define SM_WEAKUP_RUNNING           0
#define SM_WEAKUP_POWER_SAVE        1
#define SM_WEAKUP_DEEP_SLEEP        2
#define TMR1H_RUNNING               0xE0
#define TMR1L_RUNNING               0x64
#define TMR1H_POWER_SAVE            0xD0
#define TMR1L_POWER_SAVE            0x64
#define TMR1H_DEEP_SLEEP            0
#define TMR1L_DEEP_SLEEP            0


//USB Variables
#pragma udata
//The ReceivedDataBuffer[] and ToSendDataBuffer[] arrays are used as
//USB packet buffers in this firmware.  Therefore, they must be located in
//a USB module accessible portion of microcontroller RAM.
#pragma udata USB_VARIABLES=0x260

#define RX_DATA_BUFFER_ADDRESS
#define TX_DATA_BUFFER_ADDRESS

unsigned char ReceivedDataBuffer[64] RX_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] TX_DATA_BUFFER_ADDRESS;

USB_HANDLE USBOutHandle = 0;    //USB handle.  Must be initialized to 0 at startup.
USB_HANDLE USBInHandle = 0;     //USB handle.  Must be initialized to 0 at startup.


/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DECLARE LOCAL FUNCTIONS HERE ----------------------------------------------*/
void low_isr(void);
void high_isr(void);
void vInitializeBoard(void);
void vUSBTask( void );
void vBeep( unsigned char cType );
void ProcessIO(void);
void USBCBSuspend(void);
void USBCBWakeFromSuspend(void);
void USBCB_SOF_Handler(void);
void USBCBErrorHandler(void);
void USBCBCheckOtherReq(void);
void USBCBStdSetDscHandler(void);
void USBCBInitEP(void);
void USBCBSendResume(void);
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size);
unsigned int vUpdateCRC(unsigned char data, unsigned int crcVariable );
unsigned int iGetTagCount( unsigned char cTagType );
unsigned short long cCheckTag( unsigned char* cBuffer );
unsigned char cAddTag( unsigned char* cBuffer, unsigned char cTagType );
void vDelOneTag( unsigned short long lAddress, unsigned char cTagType );
unsigned int iCheckLogCount( void );
void vAddLog( unsigned char* cBuffer );
void vSendResponse( unsigned char cResponse, unsigned char cDataSize_ );
void vDelAllTags( unsigned char cTagTypes );
void vSetCardReadingTimeOut( void );
void vCheckAlarm( void );

void vRFPowerUp( void );
void vRFPowerDown( void );
unsigned char cReadEM4102( unsigned char *cBuffer );
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
/* DEFINE LOCAL FUNCTIONS HERE -----------------------------------------------*/
#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
  _asm GOTO low_isr _endasm
}
#pragma code /* return to the default code section */
#pragma interruptlow low_isr
void low_isr (void)
{
    if( PIE2bits.TMR3IE && PIR2bits.TMR3IF )
    {        
        ++cTimerOverflows;
        PIR2bits.TMR3IF = 0;

        if(xFlags.bTimeOutEnable)
        {
            ++cTimeOutCounter;
            if(cTimeOutCounter == TIMEOUT )
            {
                xFlags.bTimeOutEnable = 0;
                xFlags.bTimeOutFlag = 1;
                cTimeOutCounter = 0;                
            }
        }
    }
    if( INTCON3bits.INT1IE && INTCON3bits.INT1IF )
    {
        if( RFState == SM_READ_TAG )
        {
            cWidth = TMR0L;
            TMR0L = 0;
            INTCON2bits.INTEDG1 ^= 1;
            
            if(xFlags.bUsbPlugged)
            {
                if( cWidth > 70 )           // 65 Check for a phase change
                {
                    xRF.bBitValue ^= 1; // Invert the save bit value
                    xRF.bStoreData = TRUE;     // Force a bit store
                }   
            }
            else
            {
                if( cWidth > 65 )           // 65 Check for a phase change             
                {
                    xRF.bBitValue ^= 1; // Invert the save bit value
                    xRF.bStoreData = TRUE;     // Force a bit store
                }
            }

            
            if( xRF.bStoreData )
            {
                cRFBuffer[cRFBuffer_index] >>= 1;
                if(xRF.bBitValue)
                {
                   cRFBuffer[cRFBuffer_index] |= 0b10000000;
                }
                ++cDataTransferred;

                if( ++cRFBuffer_bitIndex == 8 )
                {
                    cRFBuffer_bitIndex = 0;
                    ++cRFBuffer_index;
                }
            }
            xRF.bStoreData = ~xRF.bStoreData;
        }
        if( RFState == SM_CHECK_RF )
        {
            if( RF_DEMOD_OUT )
            {
                RFState = SM_READ_TAG;
                
            }
        }
        k = PORTC;
        INTCON3bits.INT1IF = 0;
    }
}

#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
  _asm GOTO high_isr _endasm
}
#pragma code /* return to the default code section */
#pragma interrupt high_isr
void high_isr (void)
{    
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif

    if( INTCONbits.INT0IE && INTCONbits.INT0IF )
    {
        xFlags.bUsbDedect = 1;
        INTCONbits.INT0IE = 0;
        INTCONbits.INT0IF = 0;
    }

//    if(PIR1bits.SSPIF)
//    {
//        k = PORTC;
//        PIR1bits.SSPIF = 0;
//    }
}


/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *****************************************************************************/
void main(void)
{
    unsigned char i;
  
    vInitializeBoard();

    iNodeTagCount = iGetTagCount( NODE );
    iUserTagCount = iGetTagCount( USER );
    iEventTagCount = iGetTagCount( EVENT );
    iTagLogCount = iCheckLogCount();

    vBeep( PASS );

    while(1)
    {
//        vSetOscMode(OSC_FREQ_1MHZ);
//        vSetOscMode(OSC_FREQ_3MHZ);
//        vUSBTask();
//        Sleep();
//        while(!OSCCONbits.OSTS);
        vUSBTask();
        if(!xFlags.bUsbPlugged)
        {
            ++cAlarmCheckCounter;
            if(cAlarmCheckCounter == 55)
            {
                cAlarmCheckCounter = 0;
                vCheckAlarm();
            }
            if(xFlags.bAlarmOn)
            {
                cAlarmCheckCounter = 0;
                for( k = 0; k < 4; k++ )
                {
                    BUZZER = 1;
                    LED = 1;
                    Delay10KTCYx( 4 );
                    BUZZER = 0;
                    LED = 0;
                    Delay10KTCYx( 4 );
                }
                ++cAlarmRunningTimer;
                if(cAlarmRunningTimer == cAlarmTime)
                {
                    xFlags.bAlarmOn = 0;
                }
            }
            
            if( RFState == SM_CHECK_RF )
            {
                vRFPowerUp();
                if(OscClockFreq == OSC_FREQ_1MHZ)
                {
                    Delay10TCYx(25);
                }
                else if(OscClockFreq == OSC_FREQ_3MHZ)
                {
                    Delay100TCYx(3);
                }
                else if(OscClockFreq == OSC_FREQ_12MHZ)
                {
                    Delay100TCYx(40);
                }
                if( RFState == SM_CHECK_RF )
                {
                    vRFPowerDown();
                }
            }
            if(RFState == SM_READ_TAG)
            {
                vSetOscMode(OSC_FREQ_3MHZ);
//                Delay100TCYx(10);
//                while(!OSCCONbits.OSTS);
                for(i=0; i<1; i++)
                {
                    if( cReadEM4102( cCardId ) )//0x6C, 0x00, 0x5C, 0x33, 0x7C
                    {                           //0x4A, 0x00, 0xE4, 0x64, 0x85
                        xRF.bNewCard = 1;
                    }
                }
                if(xRF.bNewCard)
                {
                    xRF.bNewCard = 0;                    
                    if( cCheckTag( cCardId ) != ( -1 ) )
                    {
                        if( xFlags.bUserTagReaded ) //User tag must be read first.
                        {
                            vBeep( PASS );
                            if( !EX_CARD )
                            {
                                vAddLog( cCardId );
                                cExCardId[ 2 ] = cCardId[ 2 ];
                                cExCardId[ 3 ] = cCardId[ 3 ];
                                cExCardId[ 4 ] = cCardId[ 4 ];        
                            }
                        }
                        else
                        {
                            vBeep( DENY );                            
                        }
                    }
                    else
                    {
                        vBeep( DENY );                      
                    }
                }
                vSetOscMode(OSC_FREQ_1MHZ);
                Delay100TCYx(100);
            }                    
            RFState = SM_CHECK_RF;
            Sleep();
        }
        else
        {
            ClrWdt();
            if( !BUS_SENSE )
            {
                Delay10TCYx( 10 );
                if( !BUS_SENSE )
                {
                    xFlags.bUsbPlugged = 0;
                    if( USBDeviceState != DETACHED_STATE )
                    {
                        USBDeviceDetach();
                    }
                    INTCONbits.INT0IF = 0;
                    INTCONbits.INT0IE = 1;
                    vSetOscMode(OSC_FREQ_3MHZ);
                    RFState = SM_CHECK_RF;
                    Sleep();
                }
            }
        }
    }
}

/******************************************************************************
 * Function:        void vInitializeBoard(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the demo code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void vInitializeBoard(void)
{
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;

    SRCON0 = 0;
    CM1CON0 = 0;
    CM2CON0 = 0;
    ANSEL = 0;
    ANSELH = 0;
    CCP1CON = 0;
    
    TRISCbits.RC6 = 0;//TMR1 OCS sökülünce pinler output 0 yapılır.
    TRISCbits.RC7 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    
    I2C_POWER_TRIS = 0;
    I2C_POWER = 0;
    
    LED_TRIS = 0;
    LED = 0;

    BUZZER_TRIS = 0;
    BUZZER = 0;
    
    //INT0 for USB dedect
    BUS_SENSE_TRIS = 1;

    RF_DEMOD_OUT_TRIS = 1;
    
    SDA_TRIS = 0;
    SDA = 0;
    SCL_TRIS = 0;
    SCL = 0;
    //Initialize RTCC and set default time
    vInitializeRTCC();
    vInitializeOSC();

    USBDeviceInit();
    USBOutHandle = 0;
    USBInHandle = 0;

    //Enable INT0 for usb dedect
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 1; //interrupt on rising edge    

    //Reset all flags
    xFlags.cAllFlags = 0;
    xRF.cAllRFFlags = 0;
    RFState = SM_CHECK_RF;
    cAlarmCheckCounter = 0;
    //Configure interrupts
    RCONbits.IPEN = 1;		// Enable interrupt priorities
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}//end UserInit

/*******************************************************************************
 *
 * Function     : void vUSBTask( void )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vUSBTask( void )
{
    if( xFlags.bUsbDedect )
    {
        Delay10TCYx( 10 );
        if( BUS_SENSE )
        {
            vSetOscMode(OSC_FREQ_12MHZ);            
            xFlags.bUsbDedect = 0;
            xFlags.bUsbPlugged = 1;
            USBDeviceAttach();
        }
    }
    if( ( USBDeviceState == CONFIGURED_STATE ) && ( xFlags.bUsbPlugged ) )
    {
        ProcessIO();
    }
}

/*******************************************************************************
 *
 * Function     : void vBeep( unsigned char cType )
 * PreCondition : None
 * Input        : None
 * Output       : None
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vBeep( unsigned char cType )
{
    unsigned char i;

    switch( cType )
    {
        case PASS:
            BUZZER = 1;
            LED = 1;
            if(OscClockFreq == OSC_FREQ_3MHZ)
            {
                Delay10KTCYx( 3 );
            }
            else if(OscClockFreq == OSC_FREQ_12MHZ)
            {
                Delay10KTCYx( 12 );
            }
            else if(OscClockFreq == OSC_FREQ_1MHZ)
            {
                Delay10KTCYx( 1 );
            }
            BUZZER = 0;
            LED = 0;
            break;

        case DENY:
            for( i = 0; i < 6; i++ )
            {
                BUZZER = 1;
                LED = 1;
                if(OscClockFreq == OSC_FREQ_3MHZ)
                {
                    Delay10KTCYx( 1 );
                }
                else if(OscClockFreq == OSC_FREQ_12MHZ)
                {
                    Delay10KTCYx( 4 );
                }
                else if(OscClockFreq == OSC_FREQ_1MHZ)
                {
                    Delay1KTCYx( 4 );
                }

                BUZZER = 0;
                LED = 0;
                if(OscClockFreq == OSC_FREQ_3MHZ)
                {
                    Delay10KTCYx( 2 );
                }
                else if(OscClockFreq == OSC_FREQ_12MHZ)
                {
                    Delay10KTCYx( 5 );
                }
                else if(OscClockFreq == OSC_FREQ_1MHZ)
                {
                    Delay1KTCYx( 4 );
                }
            }
            break;

        case MEMORY_FULL:
            for( i = 0; i < 3; i++ )
            {
                BUZZER = 1;
                LED = 1;
                Delay10KTCYx( 100 );
                BUZZER = 0;
                LED = 0;
                Delay10KTCYx( 50 );
            }
            break;
    }
}

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{
    unsigned short long k;
    unsigned char i;
    unsigned int iCountFor;
    unsigned int iMaxCountFor;
   
    // User Application USB tasks
    if( ( USBDeviceState < CONFIGURED_STATE ) || ( USBSuspendControl == 1 ) ) return;

    //Check if we have received an OUT data packet from the host
    if(!HIDRxHandleBusy(USBOutHandle))
    {
        cRxBufferR = 0;
        iCRC = 0;
        if( ReceivedDataBuffer[ cRxBufferR ] == STX ) //STX, SERIAL NO, COMMAND, DATA SIZE, DATA, CRCH, CRCL, ETX
        {
            cRxBufferR++;

            cCommand = ReceivedDataBuffer[ cRxBufferR ];
            iCRC = vUpdateCRC( ReceivedDataBuffer[ cRxBufferR ], iCRC );
            cRxBufferR++;
            cDataSize = ReceivedDataBuffer[ cRxBufferR ];
            iCRC = vUpdateCRC( ReceivedDataBuffer[ cRxBufferR ], iCRC );
            cRxBufferR++;
            for( i = 0; i < cDataSize ; i++ )
            {
                cDataBuffer[ i ] = ReceivedDataBuffer[ cRxBufferR ];
                iCRC = vUpdateCRC( ReceivedDataBuffer[ cRxBufferR ], iCRC );
                cDataBufferR++;
                cRxBufferR++;
            }
            iCRCReceived = 256 * ReceivedDataBuffer[ cRxBufferR ];
            cRxBufferR++;
            iCRCReceived += ReceivedDataBuffer[ cRxBufferR ];

            if( iCRC == iCRCReceived )
            {
                for( i = 64; i > 0 ; i-- ) //Empty usb tx buffer
                {
                    ToSendDataBuffer[ i - 1 ] = 0;
                }
                switch( cCommand )
                {
                    case cmd_GET_SERIAL:
                        cDataBuffer[ 0 ] = ( mySerialNumber >> 8 ) & 0xFF;
                        cDataBuffer[ 1 ] = mySerialNumber & 0xFF;
                        vSendResponse( rsp_GET_SERIAL, 2 );
                        break;

                    case cmd_ADD_TAG:
                        switch( cDataBuffer[ 0 ] )
                        {
                            case USER:
                                iCountFor = iUserTagCount;
                                iMaxCountFor = MAX_USER_COUNT;
                                i = 1;
                                break;
                            case NODE:
                                iCountFor = iNodeTagCount;
                                iMaxCountFor = MAX_NODE_COUNT;
                                i = 1;
                                break;
                            case EVENT:
                                iCountFor = iEventTagCount;
                                iMaxCountFor = MAX_EVENT_COUNT;
                                i = 1;
                                break;
                            default:
                                i = 0;
                                break;
                        }
                        if( i == 1 )
                        {
                            if( iCountFor < iMaxCountFor )
                            {
                                cCardId[ 2 ] = cDataBuffer[ 1 ];
                                cCardId[ 3 ] = cDataBuffer[ 2 ];
                                cCardId[ 4 ] = cDataBuffer[ 3 ];
                                if( cCheckTag( cCardId ) == ( -1 ) )
                                {
                                    cAddTag( cCardId, cDataBuffer[ 0 ] );
                                    vSendResponse( rsp_TAG_ADDED, 0 );
                                }
                                else
                                {
                                    vSendResponse( rsp_TAG_ALREADY_ADDED, 0 );
                                }
                            }
                            else
                            {
                                switch( cDataBuffer[ 0 ] )
                                {
                                    case USER:
                                        vSendResponse( rsp_USER_MEMORY_FULL, 0 );
                                        break;
                                    case NODE:
                                        vSendResponse( rsp_NODE_MEMORY_FULL, 0 );
                                        break;
                                    case EVENT:
                                        vSendResponse( rsp_EVENT_MEMORY_FULL, 0 );
                                        break;
                                }
                            }
                        }
                        else
                        {
                            vSendResponse( rsp_WRONG_PARAMETER, 0 );
                        }
                        break;

                    case cmd_DEL_ONE_TAG:
                        cCardId[ 2 ] = cDataBuffer[ 1 ];
                        cCardId[ 3 ] = cDataBuffer[ 2 ];
                        cCardId[ 4 ] = cDataBuffer[ 3 ];
                        k = cCheckTag( cCardId );
                        if( k != ( -1 ) )
                        {
                            switch( cDataBuffer[ 0 ] )
                            {
                                case USER:
                                    vDelOneTag( k, USER );
                                    vSendResponse( rsp_TAG_DELETED, 0 );
                                    break;
                                case NODE:
                                    vDelOneTag( k, NODE );
                                    vSendResponse( rsp_TAG_DELETED, 0 );
                                    break;
                                case EVENT:
                                    vDelOneTag( k, EVENT );
                                    vSendResponse( rsp_TAG_DELETED, 0 );
                                    break;
                                default:
                                    vSendResponse( rsp_WRONG_PARAMETER, 0 );
                                    break;
                            }
                        }
                        else
                        {
                            vSendResponse( rsp_UNKNOWN_TAG, 0 );
                        }
                        break;

//                    case cmd_CHECK_TAG:
//                        cCardId[ 2 ] = cDataBuffer[ 0 ];
//                        cCardId[ 3 ] = cDataBuffer[ 1 ];
//                        cCardId[ 4 ] = cDataBuffer[ 2 ];
//                        k = cCheckTag( cCardId );
//                        if( k != ( -1 ) )
//                        {
//                            if( k < ( USER_START_ADDRESS + ( MAX_USER_COUNT * 3ul ) ) )
//                            {
//                                cDataBuffer[ 0 ] = USER;
//                            }
//                            else if( k < ( NODE_START_ADDRESS + (MAX_NODE_COUNT * 3ul ) ) )
//                            {
//                                cDataBuffer[ 0 ] = NODE;
//                            }
//                            else if( k < ( EVENT_START_ADDRESS + ( MAX_EVENT_COUNT * 3ul ) ) )
//                            {
//                                cDataBuffer[ 0 ] = EVENT;
//                            }
//                            vSendResponse( rsp_TAG_ALREADY_ADDED, 1 );
//                        }
//                        else
//                        {
//                            vSendResponse( rsp_UNKNOWN_TAG, 0 );
//                        }
//                        break;
//
//                    case cmd_SET_TIME_DATE:
//                        xTime.SEC   = cDataBuffer[ 0 ];
//                        xTime.MIN   = cDataBuffer[ 1 ];
//                        xTime.HOUR  = cDataBuffer[ 2 ];
//                        xTime.DAY_OF_WEEK  = cDataBuffer[ 3 ];
//                        xTime.DATE  = cDataBuffer[ 4 ];
//                        xTime.MONTH = cDataBuffer[ 5 ];
//                        xTime.YEAR  = cDataBuffer[ 6 ];
//                        vSetTimeDate( &xTime );
//                        vSendResponse( rsp_TIME_DATE_SYNCED, 0 );
//                        break;
//
//                    case cmd_GET_TIME_DATE:
//                        vGetTimeDate( &xTime );
//                        cDataBuffer[ 0 ] = xTime.SEC;
//                        cDataBuffer[ 1 ] = xTime.MIN;
//                        cDataBuffer[ 2 ] = xTime.HOUR;
//                        cDataBuffer[ 3 ] = xTime.DAY_OF_WEEK;
//                        cDataBuffer[ 4 ] = xTime.DATE;
//                        cDataBuffer[ 5 ] = xTime.MONTH;
//                        cDataBuffer[ 6 ] = xTime.YEAR;
//                        vSendResponse( rsp_TIME_DATE, 7 );
//                        break;
//
//                    case cmd_START_TRANSFER_TAG_LOG:
//                        iTransferCounter = 0;
//                        cDataBuffer[ 0 ] = ( unsigned char) ( iUserTagCount >> 8 ) & 0xFF;
//                        cDataBuffer[ 1 ] = ( unsigned char) iUserTagCount & 0xFF;
//                        cDataBuffer[ 2 ] = ( unsigned char) ( iNodeTagCount >> 8 ) & 0xFF;
//                        cDataBuffer[ 3 ] = ( unsigned char) iNodeTagCount & 0xFF;
//                        cDataBuffer[ 4 ] = ( unsigned char) ( iEventTagCount >> 8 ) & 0xFF;
//                        cDataBuffer[ 5 ] = ( unsigned char) iEventTagCount & 0xFF;
//                        cDataBuffer[ 6 ] = ( unsigned char) ( iTagLogCount >> 8 ) & 0xFF;
//                        cDataBuffer[ 7 ] = ( unsigned char) iTagLogCount & 0xFF;
//                        vSendResponse( rsp_TAG_LOG_TRANSFER_READY, 8 );
//                        break;
//
//                    case cmd_GET_NEXT_TAG_LOG:
//                        if( iTransferCounter < iTagLogCount )
//                        {
//                            INTCONbits.GIEH = 0;
//                            INTCONbits.GIEL = 0;
//                            k = LOG_START_ADDRESS + ( iTransferCounter * 7 );
//                            vOpenI2C();
//
//                            cDataBuffer[ 0 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 1 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 2 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 10 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 11 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 12 ] = cEEReadByte( k );
//                            k++;
//                            cDataBuffer[ 13 ] = cEEReadByte( k );
//
//                            vCloseI2C();
//                            INTCONbits.GIEH = 1;
//                            INTCONbits.GIEL = 1;
//
//                            iTransferCounter++;
//
//                            cDataBuffer[ 3 ] = ( cDataBuffer[ 10 ] >> 2 );          //SEC
//                            cDataBuffer[ 4 ] = ( ( cDataBuffer[ 10 ] & 0x03 ) << 4 );
//                            cDataBuffer[ 4 ] |= ( cDataBuffer[ 11 ] >> 4 );         //MIN
//                            cDataBuffer[ 5 ] = ( ( cDataBuffer[ 11 ] & 0x0F ) << 1 );
//                            cDataBuffer[ 5 ] |= ( cDataBuffer[ 12 ] >> 7 );         //HOUR
//                            cDataBuffer[ 6 ] = ( cDataBuffer[ 12 ] >> 2 ) & 0x1F;   //DAY
//                            cDataBuffer[ 7 ] = ( cDataBuffer[ 12 ] & 0x03 ) << 2;
//                            cDataBuffer[ 7 ] |= ( cDataBuffer[ 13 ] >> 6 );         //MONTH
//                            cDataBuffer[ 8 ] = ( cDataBuffer[ 13 ] & 0x3F );        //YEAR
//                            vSendResponse( rsp_NEXT_TAG_LOG, 9 );
//                        }
//                        else
//                        {
//                            vSendResponse( rsp_TAG_LOG_TRANSFER_COMPLATE, 0 );
//                        }
//                        break;

                    case cmd_DEL_ALL_TAG_LOGS:
                        INTCONbits.GIEH = 0;
                        INTCONbits.GIEL = 0;
                        k = LOG_START_ADDRESS;
                        vOpenI2C();
                        for( i = 0; i < 7; i++ )
                        {
                            vEEWriteByte( k, 0xFF );
                            k++;
                        }
                        vCloseI2C();
                        INTCONbits.GIEH = 1;
                        INTCONbits.GIEL = 1;
                        iTagLogCount = 0;
                        xFlags.bUserTagReaded = 0;
                        vSendResponse( rsp_ALL_TAG_LOGS_DELETED, 0 );
                        break;

                    case cmd_DEL_ALL_TAGS:
                        switch( cDataBuffer[ 0 ] )
                        {
                            case USER:
                                vDelAllTags( USER );
                                vSendResponse( rsp_ALL_USERS_DELETED, 0 );
                                break;
                            case NODE:
                                vDelAllTags( NODE );
                                vSendResponse( rsp_ALL_NODES_DELETED, 0 );
                                break;
                            case EVENT:
                                vDelAllTags( EVENT );
                                vSendResponse( rsp_ALL_EVENTS_DELETED, 0 );
                                break;
                            case ALL:
                                vDelAllTags( ALL );
                                vSendResponse( rsp_ALL_TAGS_DELETED, 0 );
                                break;
                            default:
                                vSendResponse( rsp_WRONG_PARAMETER, 0 );
                                break;
                        }
                        break;

                    case cmd_READ_TAG:  //10 saniye icinde tag okutulmazsa timeout sayesinde donguden cikilir
                        vSetCardReadingTimeOut();
                        RFState = SM_READ_TAG;
                        xRF.bNewCard = 0;
                        do
                        {
                            ClrWdt();
                            if( !xFlags.bUsbPlugged )   //USB kablosu cikarilmissa, pil harcamasin diye donguden cikilir
                                break;
                            if( xFlags.bTimeOutFlag )
                            {                                
                                break;
                            }
                            
                            if( cReadEM4102( cCardId ) )
                            {
//                                ClrWdt();
                                xRF.bNewCard = 1;
                            }                                                       
                        }
                        while( !xRF.bNewCard );

                        if( xRF.bNewCard )
                        {
                            xRF.bNewCard = 0;
                            vBeep( PASS );
                            cDataBuffer[ 0 ] = cCardId[ 2 ];
                            cDataBuffer[ 1 ] = cCardId[ 3 ];
                            cDataBuffer[ 2 ] = cCardId[ 4 ];
                            vSendResponse( rsp_TAG_NUMBER,3 );
                        }
                        if( xFlags.bTimeOutFlag )
                        {
                            xFlags.bTimeOutFlag = 0;
                            vSendResponse( rsp_TIMEOUT_OCCURED,0 );
                        }
                        break;

                    case cmd_BEEP:
                        vBeep( PASS );
                        break;

                    case cmd_START_TRANSFER_SENSOR_LOG:
                        break;

                    case cmd_GET_NEXT_SENSOR_LOG:
                        break;

                    case cmd_DEL_ALL_SENSOR_LOGS:
                        break;

                    case cmd_GET_TAG_CAPACITY:
                        cDataBuffer[ 0 ] = ( unsigned char) ( MAX_USER_COUNT >> 8 ) & 0xFF;
                        cDataBuffer[ 1 ] = ( unsigned char) MAX_USER_COUNT & 0xFF;
                        cDataBuffer[ 2 ] = ( unsigned char) ( MAX_NODE_COUNT >> 8 ) & 0xFF;
                        cDataBuffer[ 3 ] = ( unsigned char) MAX_NODE_COUNT & 0xFF;
                        cDataBuffer[ 4 ] = ( unsigned char) ( MAX_EVENT_COUNT >> 8 ) & 0xFF;
                        cDataBuffer[ 5 ] = ( unsigned char) MAX_EVENT_COUNT & 0xFF;
                        vSendResponse( rsp_GET_TAG_CAPACITY, 6 );
                        break;

                    case cmd_ADD_ALARM_FOR_A_DAY:
                        if( ( cDataBuffer[ 0 ] > 7 ) || ( cDataBuffer[ 0 ] == 0 ) )
                        {
                            vSendResponse( rsp_WRONG_PARAMETER, 0 );
                        }
                        else
                        {
                            slEEpromAddress = ALARM_START_ADDRESS + ( cDataBuffer[ 0 ] - 1 ) * ( ( 3 * MAX_ALARM_PER_DAY ) + 1 );
                            vOpenI2C();
                            cAlarmCountForCurrentDay = cEEReadByte( slEEpromAddress );
                            if( ( cAlarmCountForCurrentDay == 0xFF ) || ( cAlarmCountForCurrentDay < MAX_ALARM_PER_DAY ) )
                            {
                                if( cAlarmCountForCurrentDay == 0xFF )
                                {
                                    cAlarmCountForCurrentDay = 0;
                                }
                                vEEWriteByte( slEEpromAddress, cAlarmCountForCurrentDay + 1 );
                                slEEpromAddress += cAlarmCountForCurrentDay * 3 + 1;
                                vEEWriteByte( slEEpromAddress++, cDataBuffer[ 1 ] );
                                vEEWriteByte( slEEpromAddress++, cDataBuffer[ 2 ] );
                                vEEWriteByte( slEEpromAddress++, cDataBuffer[ 3 ] );
                                vSendResponse( rsp_ALARM_ADDED, 0 );
                            }
                            else
                            {
                                vSendResponse( rsp_ALARM_MEMORY_FULL, 0 );
                            }
                            vCloseI2C();
                        }
                        break;

                    case cmd_DEL_ALL_ALARMS_FOR_A_DAY:
                        if( ( cDataBuffer[ 0 ] > 7 ) || ( cDataBuffer[ 0 ] == 0 ) )
                        {
                            vSendResponse( rsp_WRONG_PARAMETER, 0 );
                        }
                        else
                        {
                            slEEpromAddress = ALARM_START_ADDRESS + ( cDataBuffer[ 0 ] - 1 ) * ( ( 3 * MAX_ALARM_PER_DAY ) + 1 );
                            vOpenI2C();
                            vEEWriteByte( slEEpromAddress, 0 );
                            vCloseI2C();
                            vSendResponse( rsp_ALARMS_DELETED, 0 );

                        }
                        break;

//                    case cmd_SHOW_EEPROM:
//                        INTCONbits.GIEH = 0;
//                        INTCONbits.GIEL = 0;
//                        vOpenI2C();
//                        switch( cDataBuffer[ 0 ] )
//                        {
//                            case 1:
//                            case 2:
//                            case 3:
//                            case 4:
//                            case 5:
//                            case 6:
//                            case 7:
//                                slEEpromAddress = ALARM_START_ADDRESS + ( cDataBuffer[ 0 ] - 1 ) * ( ( 3 * MAX_ALARM_PER_DAY ) + 1 ) ;
//                                for( k = 0; k < 50; k++ )
//                                {
//                                    cDataBuffer[ k ] = cEEReadByte( k + slEEpromAddress );
//                                }
//                                break;
//
//                            case 10:
//                                for( k = 0; k < 51; k++ )
//                                {
//                                    cDataBuffer[ k ] = cEEReadByte( k );
//                                }
//                                break;
//                            case 11:
//                                for( k = 3075; k < 3126; k++ )
//                                {
//                                    cDataBuffer[ k - 3075 ] = cEEReadByte( k );
//                                }
//                                break;
//                            case 12:
//                                for( k = 6150; k < 6201; k++ )
//                                {
//                                    cDataBuffer[ k - 6150 ] = cEEReadByte( k );
//                                }
//                                break;
//
//
//                        }
//                        vCloseI2C();
//                        INTCONbits.GIEH = 1;
//                        INTCONbits.GIEL = 1;
//                        vSendResponse( rsp_EEPROM, 48 );
//                        break;
                }
            }
        }
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(HID_EP, (BYTE*)&ReceivedDataBuffer, 64);
    }
}//end ProcessIO

// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA* each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

// Note *: The "usb_20.pdf" specs indicate 500uA or 2.5mA, depending upon device classification. However,
// the USB-IF has officially issued an ECN (engineering change notice) changing this to 2.5mA for all
// devices.  Make sure to re-download the latest specifications to get all of the newest ECNs.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:

	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
	//things to not work as intended.



}



/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *
 *					This call back is invoked when a wakeup from USB suspend
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// 10+ milliseconds of wakeup time, after which the device must be
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
	// Make sure the selected oscillator settings are consistent with USB
    // operation before returning from this function.
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.

	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This
 *					callback function should initialize the endpoints
 *					for the device's usage according to the current
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior,
 *                  as a USB device that has not been armed to perform remote
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex:
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup.
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;

    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager
    //properties page for the USB device, power management tab, the
    //"Allow this device to bring the computer out of standby." checkbox
    //should be checked).
    if(USBGetRemoteWakeupStatus() == TRUE)
    {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();

            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = FALSE;  //So we don't execute this code again,
                                        //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;
            do
            {
                delay_count--;
            }while(delay_count);

            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}

/*******************************************************************************
 *
 * Function     : void vUpdateCRC(unsigned char *data, unsigned int *crcVariable )
 * PreCondition : None
 * Input        : None
 * Output       : None - Calculated CRC
 * Side Effects : None
 * Overview     : CCIT 16 XMODEM CRC Algorithm
 * Note         : None
 *
 ******************************************************************************/
unsigned int vUpdateCRC(unsigned char data, unsigned int crcVariable )   //
{
    crcVariable = ( unsigned char )( crcVariable >> 8 ) | ( crcVariable << 8 );
    crcVariable ^= data;
    crcVariable ^= ( unsigned char )( crcVariable & 0xFF ) >> 4;
    crcVariable ^= ( crcVariable << 8 ) << 4;
    crcVariable ^= ( ( crcVariable & 0xFF ) << 4) << 1;
    return crcVariable;
}

/*******************************************************************************
 *
 * Function     : unsigned char cGetTagCount( unsigned char cWhat )
 * PreCondition : None
 * Input        : None
 * Output       : None - Calculated CRC
 * Side Effects : None
 * Overview     : CCIT 16 XMODEM CRC Algorithm
 * Note         : None
 *
 ******************************************************************************/
unsigned int iGetTagCount( unsigned char cTagType )
{
    UINT24_VAL slAddress;
    unsigned int iMaxCount;
    unsigned int iStartAddress;
    unsigned int i;


    switch( cTagType )
    {
        case USER:
            iMaxCount = MAX_USER_COUNT;
            iStartAddress = USER_START_ADDRESS;
            break;

        case NODE:
            iMaxCount = MAX_NODE_COUNT;
            iStartAddress = NODE_START_ADDRESS;
            break;

        case EVENT:
            iMaxCount = MAX_EVENT_COUNT;
            iStartAddress = EVENT_START_ADDRESS;
            break;
    }

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    for( i = 0; i < iMaxCount ; i++ )
    {
        ClrWdt();
        slAddress.Val = iStartAddress + ( i * 3 );
        if( cEEReadByte( slAddress.Val ) == 0xFF )
        {
            slAddress.Val++;
            if( cEEReadByte( slAddress.Val ) == 0xFF )
            {
                slAddress.Val++;
                if( cEEReadByte( slAddress.Val ) == 0xFF )
                {
                    vCloseI2C();
                    INTCONbits.GIEH = 1;
                    INTCONbits.GIEL = 1;                    
                    return i;
                }
            }
        }
    }
    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    return iMaxCount;
}

/*******************************************************************************
 *
 * Function     : unsigned char cCheckTag( unsigned char* cBuffer, unsigned char cTagType )
 * PreCondition : None
 * Input        : None
 * Output       : if user recorded : User id eeprom start address
 *              : else : return -1
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
 unsigned short long cCheckTag( unsigned char* cBuffer )
{
    UINT24_VAL slAddress;
    unsigned int i;

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    for( i = 0; i < iUserTagCount; i++ )
    {
        ClrWdt();
        slAddress.Val = USER_START_ADDRESS + ( i * 3 );
        if( cEEReadByte( slAddress.Val ) == cBuffer[ 2 ] )
        {
            slAddress.Val++;
            if( cEEReadByte( slAddress.Val ) == cBuffer[ 3 ] )
            {
                slAddress.Val++;
                if( cEEReadByte( slAddress.Val ) == cBuffer[ 4 ] )
                {
                    vCloseI2C();
                    INTCONbits.GIEH = 1;
                    INTCONbits.GIEL = 1;
                    xFlags.bUserTagReaded = 1;
                    return USER_START_ADDRESS + ( i * 3 );
                }
            }
        }
    }
    for( i = 0; i < iNodeTagCount; i++ )
    {
        ClrWdt();
        slAddress.Val = NODE_START_ADDRESS + ( i * 3 );
        if( cEEReadByte( slAddress.Val ) == cBuffer[ 2 ] )
        {
            slAddress.Val++;
            if( cEEReadByte( slAddress.Val ) == cBuffer[ 3 ] )
            {
                slAddress.Val++;
                if( cEEReadByte( slAddress.Val ) == cBuffer[ 4 ] )
                {
                    vCloseI2C();
                    INTCONbits.GIEH = 1;
                    INTCONbits.GIEL = 1;
                    return NODE_START_ADDRESS + ( i * 3 );
                }
            }
        }
    }
    for( i = 0; i < iEventTagCount; i++ )
    {
        ClrWdt();
        slAddress.Val = EVENT_START_ADDRESS + ( i * 3 );
        if( cEEReadByte( slAddress.Val ) == cBuffer[ 2 ] )
        {
            slAddress.Val++;
            if( cEEReadByte( slAddress.Val ) == cBuffer[ 3 ] )
            {
                slAddress.Val++;
                if( cEEReadByte( slAddress.Val ) == cBuffer[ 4 ] )
                {
                    vCloseI2C();
                    INTCONbits.GIEH = 1;
                    INTCONbits.GIEL = 1;
                    return EVENT_START_ADDRESS + ( i * 3 );
                }
            }
        }
    }

    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    return -1;
}

/*******************************************************************************
 *
 * Function     : unsigned char cAddTag( unsigned char* cBuffer, unsigned char cTagType )
 * PreCondition : None
 * Input        : None
 * Output       : if user count < MAX_USER_COUNT return 1, else return -1
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned char cAddTag( unsigned char* cBuffer, unsigned char cTagType )
{
    unsigned int iMaxCount;
    unsigned int iStartAddress;
    unsigned int iCount;
    unsigned char i;

    switch( cTagType )
    {
        case USER:
            iMaxCount = MAX_USER_COUNT;
            iStartAddress = USER_START_ADDRESS;
            iCount = iUserTagCount;
            break;

        case NODE:
            iMaxCount = MAX_NODE_COUNT;
            iStartAddress = NODE_START_ADDRESS;
            iCount = iNodeTagCount;
            break;

        case EVENT:
            iMaxCount = MAX_EVENT_COUNT;
            iStartAddress = EVENT_START_ADDRESS;
            iCount = iEventTagCount;
            break;

    }

    if( iCount < iMaxCount )
    {
        INTCONbits.GIEH = 0;
        INTCONbits.GIEL = 0;
        vOpenI2C();

        for( i = 3; i > 0; i-- )
        {
            vEEWriteByte( iStartAddress + ( iCount * 3 ) + i - 1, cBuffer[ i + 1 ] );
        }
        ++iCount;
        for( i = 3; i > 0; i-- )
        {
            vEEWriteByte( iStartAddress + ( iCount * 3 ) + i - 1, 0xFF );
        }

        vCloseI2C();
        INTCONbits.GIEH = 1;
        INTCONbits.GIEL = 1;

        switch( cTagType )
        {
            case USER:
                iUserTagCount = iCount;
                break;

            case NODE:
                iNodeTagCount = iCount;
                break;

            case EVENT:
                iEventTagCount = iCount;
                break;
        }

        return 1;
    }
    else
    {
        return -1;
    }

}

/*******************************************************************************
 *
 * Function     : void cDelTag( unsigned short long lAddress, unsigned char cTagType )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vDelOneTag( unsigned short long lAddress, unsigned char cTagType )
{
    unsigned char i;
    unsigned int iCount;
    unsigned int iStartAddress;

    switch( cTagType )
    {
        case USER:
            iCount = iUserTagCount;
            iStartAddress = USER_START_ADDRESS;
            break;

        case NODE:
            iCount = iNodeTagCount;
            iStartAddress = NODE_START_ADDRESS;
            break;

        case EVENT:
            iCount = iEventTagCount;
            iStartAddress = EVENT_START_ADDRESS;
            break;
    }

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    if( lAddress != ( iStartAddress + ( iCount - 1 ) * 3 ) )
    {
        for( i = 3; i > 0; --i )
        {
            vEEWriteByte( lAddress + i - 1, cEEReadByte( ( iStartAddress + ( iCount * 3 ) - 4 + i ) ) );
        }
    }
    for( i = 0; i <3; ++i )
    {
        vEEWriteByte( ( iStartAddress + ( ( iCount - 1 ) * 3 ) + i ), 0xFF );
    }

    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    iCount--;

    switch( cTagType )
    {
        case USER:
            iUserTagCount = iCount;
            break;

        case NODE:
            iNodeTagCount = iCount;
            break;

        case EVENT:
            iEventTagCount = iCount;
            break;
    }
}

/*******************************************************************************
 *
 * Function     : unsigned int iCheckLogCount( void )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
unsigned int iCheckLogCount( void )
{
    UINT24_VAL slAddress;
    unsigned int i;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    for( i = 0; i < MAX_TAG_LOG_COUNT ; i++ )
    {
        ClrWdt();
        slAddress.Val = LOG_START_ADDRESS + ( i * 7 );
        if( cEEReadByte( slAddress.Val ) == 0xFF )
        {
            slAddress.Val++;
            if( cEEReadByte( slAddress.Val ) == 0xFF )
            {
                slAddress.Val++;
                if( cEEReadByte( slAddress.Val ) == 0xFF )
                {
                    vCloseI2C();
                    INTCONbits.GIEH = 1;
                    INTCONbits.GIEL = 1;                    
                    return i;
                }
            }
        }
    }
    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
    return MAX_TAG_LOG_COUNT;
}

/*******************************************************************************
 *
 * Function     : void vAddLog( unsigned char* cBuffer )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vAddLog( unsigned char* cBuffer ) // Log Type : ID[ 2 ], ID[ 3 ], ID[ 4 ], TIME_STRUCT[ 3 ], TIME_STRUCT[ 2 ], TIME_STRUCT[ 1 ], TIME_STRUCT[ 0 ]
{
    unsigned short long lAddress;
    unsigned char cTemp[ 4 ];

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    if( iTagLogCount < MAX_TAG_LOG_COUNT )
    {
        vGetTimeDate( &xTime );
        
        //dayofweek, day veya mounth iÃ§in olabilecek hatalara karsi tedbir.
        if( ( xTime.DAY_OF_WEEK == 0 ) || ( xTime.DAY_OF_WEEK > 7 ) )
        {
            vGetTimeDate( &xTime );
            if( ( xTime.DAY_OF_WEEK == 0 ) || ( xTime.DAY_OF_WEEK > 7 ) )
            {
                xTime.DAY_OF_WEEK = 7;
                return;
            }
        }
        if( ( xTime.DATE == 0 ) || ( xTime.DATE > 31 ) )
        {
            vGetTimeDate( &xTime );
            if( ( xTime.DATE == 0 ) || ( xTime.DATE > 31 ) )
            {
                xTime.DATE = 7;
                return;
            }
        }
        if( ( xTime.MONTH == 0 ) || ( xTime.MONTH > 12 ) )
        {
            vGetTimeDate( &xTime );
            if( ( xTime.MONTH == 0 ) || ( xTime.MONTH > 12 ) )
            {
                xTime.MONTH = 7;
                return;
            }
        }

        cTemp[ 0 ] = xTime.SEC;
        cTemp[ 0 ] <<= 2;
        cTemp[ 0 ] |= ( xTime.MIN >> 4 );
        cTemp[ 1 ] = xTime.MIN;
        cTemp[ 1 ] <<= 4;
        cTemp[ 1 ] |= ( xTime.HOUR >> 1 );
        cTemp[ 2 ] = xTime.HOUR;
        cTemp[ 2 ] <<= 7;
        cTemp[ 2 ] |= ( xTime.DATE << 2 );
        cTemp[ 2 ] |= ( xTime.MONTH >> 2 );
        cTemp[ 3 ] = xTime.MONTH;
        cTemp[ 3 ] <<= 6;
        cTemp[ 3 ] |= xTime.YEAR;

        
        lAddress = LOG_START_ADDRESS + ( iTagLogCount * 7 );
        vOpenI2C();

        vEEWriteByte( lAddress, cBuffer[ 2 ] );
        lAddress++;
        vEEWriteByte( lAddress, cBuffer[ 3 ] );
        lAddress++;
        vEEWriteByte( lAddress, cBuffer[ 4 ] );
        lAddress++;
        vEEWriteByte( lAddress, cTemp[ 0 ] );
        lAddress++;
        vEEWriteByte( lAddress, cTemp[ 1 ] );
        lAddress++;
        vEEWriteByte( lAddress, cTemp[ 2 ] );
        lAddress++;
        vEEWriteByte( lAddress, cTemp[ 3 ] );
        lAddress++;
        vEEWriteByte( lAddress, 0xFF );
        lAddress++;
        vEEWriteByte( lAddress, 0xFF );
        lAddress++;
        vEEWriteByte( lAddress, 0xFF );

        vCloseI2C();
        iTagLogCount++;
        INTCONbits.GIEH = 1;
        INTCONbits.GIEL = 1;
    }
    else
    {
        vBeep( MEMORY_FULL );
    }
}

/*******************************************************************************
 *
 * Function     : void vSendResponse( unsigned char cResponse, unsigned char cDataSize_ )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vSendResponse( unsigned char cResponse, unsigned char cDataSize_ )
{
    unsigned char i;
    iCRC = 0;
    if(!HIDTxHandleBusy(USBInHandle))
    {
        ToSendDataBuffer[ 0 ] = STX;
        ToSendDataBuffer[ 1 ] = cResponse;
        iCRC = vUpdateCRC( ToSendDataBuffer[ 1 ], iCRC );
        ToSendDataBuffer[ 2 ] = cDataSize_;
        iCRC = vUpdateCRC( ToSendDataBuffer[ 2 ], iCRC );
        for( i = 0; i < cDataSize_; i++ )
        {
            ToSendDataBuffer[ i + 3 ] = cDataBuffer[ i ];
            iCRC = vUpdateCRC( ToSendDataBuffer[ i + 3 ], iCRC );
        }

        ToSendDataBuffer[ 3 + cDataSize_ ] = ( iCRC >> 8 ) & 0xFF;
        ToSendDataBuffer[ 4 + cDataSize_ ] = iCRC & 0xFF;
        ToSendDataBuffer[ 5 + cDataSize_ ] = ETX;

        //Prepare the USB module to send the data packet to the host
        USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer[0],64);
    }
}

/*******************************************************************************
 *
 * Function     : void vDelAllTags( unsigned char cTagTypes )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vDelAllTags( unsigned char cTagTypes )
{
    unsigned int i;

    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
    vOpenI2C();

    if( ( cTagTypes == USER ) || ( cTagTypes == ALL ) )
    {
        i = 0;
        i = 1;
        for( i = USER_START_ADDRESS; i < ( USER_START_ADDRESS + 3 ); i++ )
        {
            vEEWriteByte( i, 0xFF );
        }
        iUserTagCount = 0;
    }
    if( ( cTagTypes == NODE ) || ( cTagTypes == ALL ) )
    {
        for( i = NODE_START_ADDRESS; i < ( NODE_START_ADDRESS + 3 ); i++ )
        {
            vEEWriteByte( i, 0xFF );
        }
        iNodeTagCount = 0;
    }
    if( ( cTagTypes == EVENT ) || ( cTagTypes == ALL ) )
    {
        for( i = EVENT_START_ADDRESS; i < ( EVENT_START_ADDRESS + 3 ); i++ )
        {
            vEEWriteByte( i, 0xFF );
        }
        iEventTagCount = 0;
    }

    vCloseI2C();
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

/*******************************************************************************
 *
 * Function     : void vSetCardReadingTimeOut( void )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vSetCardReadingTimeOut( void )
{    
    xFlags.bTimeOutFlag = 0;
    xFlags.bTimeOutEnable = 1;
    cTimeOutCounter = 0;
}


/*******************************************************************************
 *
 * Function     : void vCheckAlarm( void )
 * PreCondition : None
 * Input        : None
 * Output       :
 * Side Effects : None
 * Overview     :
 * Note         : None
 *
 ******************************************************************************/
void vCheckAlarm( void )
{
    unsigned short long slAlarmAddress;
    unsigned char m;

    vGetTimeDate( &xTime );
    slAlarmAddress = ALARM_START_ADDRESS + ( xTime.DAY_OF_WEEK - 1 ) * ( ( 3 * MAX_ALARM_PER_DAY ) + 1 );
    vOpenI2C();
    cAlarmCountForCurrentDay = cEEReadByte( slAlarmAddress );
    if( cAlarmCountForCurrentDay != 0xFF )
    {
        for( m = 0 ; m < cAlarmCountForCurrentDay; m++ )
        {
            ClrWdt();
            slEEpromAddress = slAlarmAddress + 1 + ( m * 3 );
            if( xTime.HOUR == cEEReadByte( slEEpromAddress ) )
            {
                ++slEEpromAddress;
                if( xTime.MIN == cEEReadByte( slEEpromAddress ) )
                {
                    if( xTime.MIN != cExAlarmMinute )
                    {
                        ++slEEpromAddress;
                        cExAlarmMinute = xTime.MIN;
                        cAlarmTime = cEEReadByte( slEEpromAddress );
                        cAlarmRunningTimer = 0;
                        xFlags.bAlarmOn = 1;
                        vCloseI2C();
                        break;
                    }
                }
                else
                {
                    cExAlarmMinute = 0xFF;
                }
            }
        }
    }   
    vCloseI2C();
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
    RF_POWER_TRIS = 0;
    RF_POWER = 1;
    RF_PWM_OUT_TRIS = 0;
    RF_PWM_OUT = 1;

    REFCON1 = 0xE0;
    REFCON2 = 0b00001100;
//    REFCON1 = 0xE0;
//    REFCON2 = 0x0F;

    T2CON = 0b00000000;
    PIE1bits.TMR2IE = 0;
    if(OscClockFreq == OSC_FREQ_1MHZ)
    {
        CCP1CON = 0b00001100;
        CCPR1L  = 0b00000001;   //4
        PR2 = 1;
    }
//    else if(OscClockFreq == OSC_FREQ_2MHZ)
//    {
//        CCP1CON = 0b00001100;
//        CCPR1L  = 0b00000010;   //8
//        PR2 = 3;
//    }
    else if(OscClockFreq == OSC_FREQ_3MHZ)
    {
        CCP1CON = 0b00001100;
        CCPR1L  = 0b00000011;   //12
        PR2 = 5;
    }
    else if(OscClockFreq == OSC_FREQ_12MHZ)
    {
        CCP1CON = 0b00001100;
        CCPR1L  = 0b00001100;   //48
        PR2 = 23;
    }
    T2CONbits.TMR2ON = 1;
    if(OscClockFreq == OSC_FREQ_1MHZ)
    {
        Delay10TCYx(5);
    }
//    else if(OscClockFreq == OSC_FREQ_2MHZ)
//    {
//        Delay10TCYx(10);
//    }
    else if(OscClockFreq == OSC_FREQ_3MHZ)
    {
        Delay10TCYx(10);
    }
    else if(OscClockFreq == OSC_FREQ_12MHZ)
    {
        Delay10TCYx(40);
    }
    

    //INT1 used to capture edge of RF_DEMOD_OUT signal
    INTCON3bits.INT1IP = 0;
    INTCON2bits.INTEDG1 = 1;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
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
    CCP1CON = 0;
    T2CON = 0;
//    Delay100TCYx(5);
    RF_PWM_OUT = 1;
    REFCON1 = 0;
    REFCON2 = 0;
//    Delay100TCYx(5);
    INTCON3bits.INT1IE = 0;
    INTCON3bits.INT1IF = 0;
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
    unsigned char i;

    vRFPowerUp();
    cRFGet( sizeof( cRFBuffer ) * 8);
    vRFPowerDown();
//    vSetOscMode(OSC_FREQ_1MHZ);
//    Delay100TCYx(100);

    for( i = 0 ; i < 2 ; ++i )
    {
        ClrWdt();
        while( !END_OF_RFBUFFER )
        {
            if( cHeaderSearchEM4102() )       // Try to find 9 consecutive 1s
            {
                if( cDecodeDataEM4102( cBuffer ) )  // Try to decode the data after the header
                {
                    vMirrorData( cBuffer );
                    vRFBufferFill( 0xAA );  // Prevents false detection
                    return TRUE;             // Return sucessful read
                }
            }
        }
        vRFBufferInvert();                // Invert the buffer because the
        cRFBuffer_bitIndex = 0;
        cRFBuffer_index = 0;
    }
    vRFBufferFill( 0xAA );

    return FALSE;
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
    unsigned char cDelay;

//    if(OscClockFreq == OSC_FREQ_1MHZ)
//    {
//        cDelay = DELAY1MHZ;
//        T0CON = 0b11000001;
//        TMR0H = 0;
//        TMR0L = 0;
//
//        T3CON = 0b00000001; //1:8 Prescaler, Timer3 off
//        IPR2bits.TMR3IP = 0;
//        PIR2bits.TMR3IF = 0;
//        PIE2bits.TMR3IE = 1;    // Disable TMR3 interrupt
//    }
    if(OscClockFreq == OSC_FREQ_3MHZ)
    {
        cDelay = DELAY3MHZ;
        T0CON = 0b11000001;
        TMR0H = 0;
        TMR0L = 0;

        T3CON = 0b00000001; //1:1 Prescaler, Timer3 on
        IPR2bits.TMR3IP = 0;
        PIR2bits.TMR3IF = 0;
        PIE2bits.TMR3IE = 1;    // Disable TMR3 interrupt
    }
    else if(OscClockFreq == OSC_FREQ_12MHZ)
    {
        cDelay = DELAY12MHZ;
        T0CON = 0b11000011;
        TMR0H = 0;
        TMR0L = 0;

        T3CON = 0b00100001; //1:4 Prescaler, Timer3 on
        IPR2bits.TMR3IP = 0;
        PIR2bits.TMR3IF = 0;
        PIE2bits.TMR3IE = 1;
    }

//    INTCON2bits.INTEDG1 = 1;
    xRF.bStoreData     = TRUE;
    xRF.bBitValue      = 0;
    cRFBuffer_index    = 0;
    cRFBuffer_bitIndex = 0;
    cDataTransferred   = 0;
    cTimerOverflows    = 0;
    iOldClock          = 0;

    while( ( cDataTransferred < cNumBits ) && ( cTimerOverflows < cDelay ) );

    cRFBuffer_index = 0;
    cRFBuffer_bitIndex = 0;
    T0CON = 0;
    T3CON = 0;
    INTCON3bits.INT1IE = 0;
    INTCON3bits.INT1IF = 0;
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
    unsigned char i;

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
char cRFBufferGetBit( void ) //Buradan bittest kaldırılacak
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



/** EOF main.c *************************************************/

