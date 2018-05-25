<?xml version="1.0" encoding="UTF-8"?>
<configurationDescriptor version="62">
  <logicalFolder name="root" displayName="root" projectFiles="true">
    <logicalFolder name="HeaderFiles"
                   displayName="Header Files"
                   projectFiles="true">
    </logicalFolder>
    <logicalFolder name="LinkerScript"
                   displayName="Linker Files"
                   projectFiles="true">
    </logicalFolder>
    <logicalFolder name="SourceFiles"
                   displayName="Source Files"
                   projectFiles="true">
    </logicalFolder>
    <logicalFolder name="ExternalFiles"
                   displayName="Important Files"
                   projectFiles="false">
      <itemPath>Makefile</itemPath>
    </logicalFolder>
  </logicalFolder>
  <sourceRootList>
    <Elem>../src</Elem>
  </sourceRootList>
  <projectmakefile>Makefile</projectmakefile>
  <confs>
    <conf name="default" type="2">
      <toolsSet>
        <developmentServer>localhost</developmentServer>
        <targetDevice>PIC18LF14K50</targetDevice>
        <targetHeader>AC244024</targetHeader>
        <targetPluginBoard></targetPluginBoard>
        <platformTool>RealICEPlatformTool</platformTool>
        <languageToolchain>C18</languageToolchain>
        <languageToolchainVersion>3.43</languageToolchainVersion>
        <platform>3</platform>
      </toolsSet>
      <compileType>
        <linkerTool>
          <linkerLibItems>
          </linkerLibItems>
        </linkerTool>
        <archiverTool>
        </archiverTool>
        <loading>
          <useAlternateLoadableFile>false</useAlternateLoadableFile>
          <parseOnProdLoad>false</parseOnProdLoad>
          <alternateLoadableFile></alternateLoadableFile>
        </loading>
      </compileType>
      <makeCustomizationType>
        <makeCustomizationPreStepEnabled>false</makeCustomizationPreStepEnabled>
        <makeCustomizationPreStep></makeCustomizationPreStep>
        <makeCustomizationPostStepEnabled>false</makeCustomizationPostStepEnabled>
        <makeCustomizationPostStep></makeCustomizationPostStep>
        <makeCustomizationPutChecksumInUserID>false</makeCustomizationPutChecksumInUserID>
        <makeCustomizationEnableLongLines>false</makeCustomizationEnableLongLines>
        <makeCustomizationNormalizeHexFile>false</makeCustomizationNormalizeHexFile>
      </makeCustomizationType>
      <C18>
        <property key="code-model" value="ms"/>
        <property key="data-model" value="oa-"/>
        <property key="default-char-unsigned" value="false"/>
        <property key="enable-all-optimizations" value="true"/>
        <property key="enable-int-promotion" value="false"/>
        <property key="enable-multi-bank-stack-model" value="false"/>
        <property key="enable-ob" value="true"/>
        <property key="enable-od" value="true"/>
        <property key="enable-om" value="true"/>
        <property key="enable-on" value="true"/>
        <property key="enable-op" value="true"/>
        <property key="enable-opa" value="true"/>
        <property key="enable-or" value="true"/>
        <property key="enable-os" value="true"/>
        <property key="enable-ot" value="true"/>
        <property key="enable-ou" value="true"/>
        <property key="enable-ow" value="true"/>
        <property key="extra-include-directories"
                  value="..\include;..\src;..\..\Microchip\Include;..\..\Microchip\USB"/>
        <property key="optimization-master" value="Enable all"/>
        <property key="preprocessor-macros" value=""/>
        <property key="procedural-abstraction-passes" value="4"/>
        <property key="storage-class" value="sca"/>
        <property key="verbose" value="false"/>
        <property key="warning-level" value="2"/>
      </C18>
      <C18-AS>
        <property key="cross.reference.file" value=""/>
        <property key="default.radix" value="HEX"/>
        <property key="enable.case.sensitivity" value="true"/>
        <property key="hex.output.format" value="INHX32"/>
        <property key="preprocessor.macros" value=""/>
        <property key="warning.level" value="0"/>
      </C18-AS>
      <C18-LD>
        <property key="cod-file" value="false"/>
        <property key="extra-lib-directories" value=""/>
        <property key="hex-output-format" value="INHX32"/>
        <property key="map-file" value="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"/>
      </C18-LD>
      <C18LanguageToolchain>
        <property key="extended-mode" value="false"/>
        <property key="extended-mode-mcc18" value="false"/>
        <property key="extended-mode-mpasm" value="false"/>
        <property key="extended-mode-mplink" value="false"/>
        <property key="stack-analysis" value="false"/>
        <property key="stack-analysis-mcc18" value="false"/>
        <property key="stack-analysis-mplink" value="false"/>
      </C18LanguageToolchain>
      <RealICEPlatformTool>
        <property key="AutoSelectMemRanges" value="auto"/>
        <property key="Freeze Peripherals" value="true"/>
        <property key="RIExTrigs.Five" value="OFF"/>
        <property key="RIExTrigs.Four" value="OFF"/>
        <property key="RIExTrigs.One" value="OFF"/>
        <property key="RIExTrigs.Seven" value="OFF"/>
        <property key="RIExTrigs.Six" value="OFF"/>
        <property key="RIExTrigs.Three" value="OFF"/>
        <property key="RIExTrigs.Two" value="OFF"/>
        <property key="RIExTrigs.Zero" value="OFF"/>
        <property key="SecureSegment.SegmentProgramming" value="FullChipProgramming"/>
        <property key="ToolFirmwareFilePath"
                  value="Press to browse for a specific firmware version"/>
        <property key="ToolFirmwareOption.UseLatestFirmware" value="true"/>
        <property key="debugoptions.useswbreakpoints" value="false"/>
        <property key="hwtoolclock.frcindebug" value="false"/>
        <property key="hwtoolclock.instructionspeed" value="4"/>
        <property key="hwtoolclock.units" value="mips"/>
        <property key="memories.aux" value="false"/>
        <property key="memories.bootflash" value="true"/>
        <property key="memories.configurationmemory" value="true"/>
        <property key="memories.configurationmemory2" value="true"/>
        <property key="memories.dataflash" value="true"/>
        <property key="memories.eeprom" value="true"/>
        <property key="memories.flashdata" value="true"/>
        <property key="memories.id" value="true"/>
        <property key="memories.programmemory" value="true"/>
        <property key="memories.programmemory.end" value="0x3fff"/>
        <property key="memories.programmemory.partition2" value="true"/>
        <property key="memories.programmemory.partition2.end"
                  value="${memories.programmemory.partition2.end.value}"/>
        <property key="memories.programmemory.partition2.start"
                  value="${memories.programmemory.partition2.start.value}"/>
        <property key="memories.programmemory.start" value="0x0"/>
        <property key="poweroptions.powerenable" value="false"/>
        <property key="programoptions.donoteraseauxmem" value="false"/>
        <property key="programoptions.eraseb4program" value="true"/>
        <property key="programoptions.preservedataflash" value="false"/>
        <property key="programoptions.preserveeeprom" value="false"/>
        <property key="programoptions.preserveprogramrange" value="false"/>
        <property key="programoptions.preserveprogramrange.end" value="0x3fff"/>
        <property key="programoptions.preserveprogramrange.start" value="0x0"/>
        <property key="programoptions.preserveuserid" value="false"/>
        <property key="programoptions.programcalmem" value="false"/>
        <property key="programoptions.programuserotp" value="false"/>
        <property key="programoptions.usehighvoltageonmclr" value="false"/>
        <property key="programoptions.uselvpprogramming" value="false"/>
        <property key="tracecontrol.disablemacros" valu**********************/
unsigned char cReadEM4102( unsigned char *cBuffer )
{
    unsigned int i;

    vRFPowerUp();
    cRFGet( sizeof( cRFBuffer ) * 8, TRUE );
    vRFPowerDown();

    for( i = 0 ; i < 2 ; ++i )
    {
        while( !END_OF_RFBUFFER )
        {
            if( cHeaderSearchEM4102() )       // Try to find 9 consecutive 1s
            {
                if( cDecodeDataEM4102( cBuffer ) )  // Try to decode the data after the header
                {
                    vMirrorData( cBuffer );
                    vRFBufferFill( 0xAA );     // Prevents false detection
                    return TRUE;             // Return sucessful read
                }
            }
        }
        vRFBufferInvert();                // Invert the buffer because the
    }
    vRFBufferFill( 0xAA );
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
//    unsigned char abc, aaa;
//    abc = (ClockFreq/(4*F_PWM)) - 1;
//    aaa = ClockFreq/(2*F_PWM);

//    abc = (F_OSC/(4*F_PWM)) - 1;

//    abc = F_OSC/(2*F_PWM);

//    abc = (char)( ( ( 1/F_PWM)  / ( 4l * (1/F_OSC) ) ) - 1l);
//    abc = abc + 1;
//    OpenTimer2( TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1 );
//    SetOutputPWM1( SINGLE_OUT, PWM_MODE_1 );
//    SetDCPWM1( 48 );//for 12MHz
//    SetDCPWM1(aaa);
//    OpenPWM1(23);//for 12MHz
//    OpenPWM1(abc);
//    OpenTimer2( TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1 );
    T2CON = 0b00000000;
    PIE1bits.TMR2IE = 0;//Bunu uygun biryerde 1 kez yapalim
    CCP1CON = 0b00001100;
    CCPR1L  = 0b00000011;
    PR2 = 5;
    T2CONbits.TMR2ON = 1;

//    SetOutputPWM1( SINGLE_OUT, PWM_MODE_1 );
//    SetDCPWM1( 12 );
//    OpenPWM1( 5 );
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
unsigned char cRFGet( unsigned char cNumBits, unsigned char cEdgee )
{
    vRFReadEdge( cEdgee );
    xRF.bStoreData     = TRUE;
    xRF.bBitValue      = 0;
    cRFBuffer_index    = 0;
    cRFBuffer_bitIndex = 0;
    cDataTransferred   = 0;
    cTimerOverflows    = 0;
    iOldClock          = 0;

    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;

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
    
    for( i = 0; i < sizeof( cRFBuffer ) ; i++ )
    {
        cRFBuffer[ i ] = data;
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
    
    for( i = sizeof( cRFBuffer ) ; i>0 ; --i )
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


