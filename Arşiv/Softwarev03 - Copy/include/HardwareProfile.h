/*******************************************************************************
 *
 * FileName     : HardwareProfile.h ( AT GUARD Simple Hardware profile file )
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
 * 1.0        Erdem Tayfun Salman        16/01/2012       Initial Version
 ******************************************************************************/
#ifndef HARDWAREPROFILE_H
#define HARDWAREPROFILE_H

#define CLOCK_FREQ          12000000
#define GetSystemClock()    CLOCK_FREQ

//USB STACK DEFINES
//#define USE_SELF_POWER_SENSE_IO
#define tris_self_power     TRISAbits.TRISA2    // Input
#if defined(USE_SELF_POWER_SENSE_IO)
#define self_power          PORTAbits.RA2
#else
#define self_power          1
#endif

//#define USE_USB_BUS_SENSE_IO
#define tris_usb_bus_sense  TRISAbits.TRISA1    // Input
#if defined(USE_USB_BUS_SENSE_IO)
#define USB_BUS_SENSE       PORTAbits.RA1
#else
#define USB_BUS_SENSE       1
#endif

//I2C Pins
#define SDA_TRIS            ( TRISBbits.TRISB4 )
#define SCL_TRIS            ( TRISBbits.TRISB6 )
#define I2C_POWER_TRIS      ( TRISBbits.TRISB7 )
#define I2C_POWER           ( LATBbits.LATB7 )

//RF Pins
#define RF_POWER_TRIS       ( TRISCbits.TRISC4 )
#define RF_POWER            ( LATCbits.LATC4 )
#define RF_PWM_OUT_TRIS     ( TRISCbits.TRISC5 )
#define RF_PWM_OUT          ( LATCbits.LATC5 )
#define RF_DEMOD_OUT        ( PORTCbits.RC1 )            // Data read in interrupt service routine
#define RF_DEMOD_OUT_TRIS   ( TRISCbits.TRISC1 )         // Data read in interrupt service routine

//Led and buzzer
#define LED_TRIS            ( TRISBbits.TRISB5 )
#define LED                 ( LATBbits.LATB5 )
#define BUZZER_TRIS         ( TRISCbits.TRISC3 )
#define BUZZER              ( LATCbits.LATC3 )

//USB bus sense
#define BUS_SENSE_TRIS      ( TRISCbits.TRISC0 )
#define BUS_SENSE           ( PORTCbits.RC0 )


#endif
