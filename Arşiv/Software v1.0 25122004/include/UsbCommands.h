/*******************************************************************************
 *
 * FileName     :
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
#ifndef USBCOMMANDS_H
#define USBCOMMANDS_H

/* DEFINE CONSTANTS HERE -----------------------------------------------------*/
//#define STX                     ( 0x0F )
//#define ETX                     ( 0x04 )
// HOST COMMANDS
// STX, COMMMAND, DATA SIZE, DATA1,...., DATAn, CRCH, CRCL, ETX

#define cmd_GET_SERIAL                      ( 101 )
#define cmd_ADD_TAG                         ( 102 )
#define cmd_DEL_ONE_TAG                     ( 103 )
#define cmd_CHECK_TAG                       ( 104 )
#define cmd_SET_TIME_DATE                   ( 105 )
#define cmd_GET_TIME_DATE                   ( 106 )
#define cmd_START_TRANSFER_TAG_LOG          ( 107 )
#define cmd_GET_NEXT_TAG_LOG                ( 108 )
#define cmd_DEL_ALL_TAG_LOGS                ( 109 )
#define cmd_DEL_ALL_TAGS                    ( 110 )
#define cmd_READ_TAG                        ( 111 )     //5 SANİYE İÇİNDE KART OKUTULMAZ İSE UYARI MESAJI YOLLA rsp_TIMEOUT_OCCURED yollar.
#define cmd_BEEP                            ( 112 )
#define cmd_START_TRANSFER_SENSOR_LOG       ( 113 )
#define cmd_GET_NEXT_SENSOR_LOG             ( 114 )
#define cmd_DEL_ALL_SENSOR_LOGS             ( 115 )
#define cmd_GET_TAG_CAPACITY                ( 116 )     // Cihazın kapasitesi eklenebilecek tag sayıları. ( 1: USERS / 2: NODES / 3: EVENT )
#define cmd_ADD_ALARM_FOR_A_DAY             ( 117 )
#define cmd_DEL_ALL_ALARMS_FOR_A_DAY        ( 118 )



#define cmd_SHOW_EEPROM                     ( 119 )     // For debugging

#define rsp_GET_SERIAL                      ( 201 )
#define rsp_TAG_ADDED                       ( 202 )
#define rsp_TAG_ALREADY_ADDED               ( 203 )
#define rsp_TAG_DELETED                     ( 204 )
#define rsp_UNKNOWN_TAG                     ( 205 )
#define rsp_TIME_DATE_SYNCED                ( 206 )
#define rsp_TIME_DATE                       ( 207 )
#define rsp_TAG_LOG_TRANSFER_READY          ( 208 )
#define rsp_NEXT_TAG_LOG                    ( 209 )
#define rsp_ALL_TAG_LOGS_DELETED            ( 210 )
#define rsp_ALL_USERS_DELETED               ( 211 )
#define rsp_ALL_NODES_DELETED               ( 212 )
#define rsp_ALL_TAGS_DELETED                ( 213 )
#define rsp_TAG_NUMBER                      ( 214 )
#define rsp_TAG_LOG_TRANSFER_COMPLATE       ( 215 )
#define rsp_USER_MEMORY_FULL                ( 216 )
#define rsp_NODE_MEMORY_FULL                ( 217 )
#define rsp_EVENT_MEMORY_FULL               ( 218 )
#define rsp_ALL_EVENTS_DELETED              ( 219 )
#define rsp_SENSOR_LOG_TRANSFER_READY       ( 220 )
#define rsp_NEXT_SENSOR_LOG                 ( 221 )
#define rsp_SENSOR_LOG_TRANSFER_COMPLATE    ( 222 )
#define rsp_ALL_SENSOR_LOGS_DELETED         ( 223 )
#define rsp_WRONG_PARAMETER                 ( 224 )     // cmd_ADD_TAG, cmd_DEL_ONE_TAG komutlarında, tag tipi parametresi 1,2,3 dışında bir değer olursa gelir.
                                                        // cmd_DEL_ALL_TAGS komutunda, tag tipi parametresi 1,2,3,4 dışında bir değer olursa gelir.
#define rsp_GET_TAG_CAPACITY                ( 225 )
#define rsp_TIMEOUT_OCCURED                 ( 226 )

#define rsp_ALARM_ADDED                     ( 227 )
#define rsp_ALARMS_DELETED                  ( 228 )
#define rsp_ALARM_MEMORY_FULL               ( 229 )

#define rsp_EEPROM                          ( 230 )     // For debugging


/*------------------------------------------------------------------------------
 * 
 * HOST COMMAND     : Get Serial Number
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Command ( cmd_GET_SERIAL )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 * 
 * 
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_GET_SERIAL )
 * BYTE 3 : Data size ( 2 )
 * BYTE 4 : Serial number high byte
 * BYTE 5 : Serial number low byte
 * BYTE 6 : CRCH
 * BYTE 7 : CRCL
 * BYTE 8 : ETX
 * 
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Add a tag
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_ADD_TAG )
 * BYTE 3 : DATA SIZE ( 4 )
 * BYTE 4 : TAG TYPE ( 1: USERS / 2: NODES / 3: EVENT )
 * BYTE 5 : TAG_DATA[ 2 ]
 * BYTE 6 : TAG_DATA[ 1 ]
 * BYTE 7 : TAG_DATA[ 0 ]
 * BYTE 8 : CRCH
 * BYTE 9 : CRCL
 * BYTE 10: ETX
 *
 *
 * DEVICE RESPONSE  : Tag added.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TAG_ADDED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 * DEVICE RESPONSE  : Tag already added.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TAG_ALREADY_ADDED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 * DEVICE RESPONSE  : User memory full, tag can't add.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_USER_MEMORY_FULL )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 * DEVICE RESPONSE  : Node memory full, tag can't add.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_NODE_MEMORY_FULL )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 * DEVICE RESPONSE  : Event memory full, tag can't add.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_NODE_MEMORY_FULL )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Delete a tag
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_DEL_ONE_TAG )
 * BYTE 3 : DATA SIZE ( 4 )
 * BYTE 4 : TAG TYPE ( 1: USERS / 2: NODES / 3: EVENT )
 * BYTE 5 : TAG_DATA[ 2 ]
 * BYTE 6 : TAG_DATA[ 1 ]
 * BYTE 7 : TAG_DATA[ 0 ]
 * BYTE 7 : CRCH
 * BYTE 9 : CRCL
 * BYTE 10: ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TAG_DELETED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_UNKNOWN_TAG )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Check a tag
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_CHECK_TAG )
 * BYTE 3 : DATA SIZE ( 3 )
 * BYTE 4 : TAG_DATA[ 2 ]
 * BYTE 5 : TAG_DATA[ 1 ]
 * BYTE 6 : TAG_DATA[ 0 ]
 * BYTE 7 : CRCH
 * BYTE 8 : CRCL
 * BYTE 9 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TAG_ALREADY_ADDED )
 * BYTE 3 : Data size ( 1 )
 * BYTE 4 : Tag type ( 1: USERS / 2: NODES / 3: EVENT )
 * BYTE 5 : CRCH
 * BYTE 6 : CRCL
 * BYTE 7 : ETX
 * 
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_UNKNOWN_TAG )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 * 
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Set Calender Time
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_SET_TIME_DATE                   ( 105 ) )
 * BYTE 3 : DATA SIZE ( 7 )
 * BYTE 4 : SECOND  // 0 - 59
 * BYTE 5 : MINUTE  // 0 - 59
 * BYTE 6 : HOUR    // 0 - 23
 * BYTE 7 : DAY OF WEEK    // 1 - 7
 * BYTE 8 : DAY     // 1 - 31
 * BYTE 9 : MONTH   // 1 - 12
 * BYTE 10: YEAR    // 0 - 63
 * BYTE 11: CRCH
 * BYTE 12: CRCL
 * BYTE 13: ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TIME_DATE_SYNCED                ( 206 ) )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Get Calender Time
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_GET_TIME_DATE )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TIME_DATE )
 * BYTE 3 : Data size ( 6 )
 * BYTE 4 : SECOND  // 0 - 59
 * BYTE 5 : MINUTE  // 0 - 59
 * BYTE 6 : HOUR    // 0 - 23
 * BYTE 7 : DAY OF WEEK    // 1 - 7
 * BYTE 8 : DAY     // 1 - 31
 * BYTE 9 : MONTH   // 1 - 12
 * BYTE 10: YEAR    // 0 - 63
 * BYTE 11: CRCH
 * BYTE 12: CRCL
 * BYTE 13: ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Start transfer for tag logs
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_START_TRANSFER_TAG_LOG          ( 107 ) )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TAG_LOG_TRANSFER_READY          ( 208 ) )
 * BYTE 3 : Data size ( 8 )
 * BYTE 4 : Total users count High Byte
 * BYTE 4 : Total users count Low Byte
 * BYTE 5 : Total nodes count High Byte
 * BYTE 5 : Total nodes count Low Byte
 * BYTE 6 : Total event count High Byte
 * BYTE 6 : Total event count Low Byte
 * BYTE 7 : Total log count High Byte
 * BYTE 8 : Total log count Low Byte
 * BYTE 9 : CRCH
 * BYTE 10: CRCL
 * BYTE 11: ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Get next tag log
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_GET_NEXT_TAG_LOG                ( 108 ) )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_NEXT_TAG_LOG                    ( 209 ) )
 * BYTE 3 : Data size ( 9 )
 * BYTE 4 : TAG_DATA[ 2 ]
 * BYTE 5 : TAG_DATA[ 1 ]
 * BYTE 6 : TAG_DATA[ 0 ]
 * BYTE 7 : SECOND  // 0 - 59
 * BYTE 8 : MINUTE  // 0 - 59
 * BYTE 9 : HOUR    // 0 - 23
 * BYTE 10: DAY     // 1 - 31
 * BYTE 11: MONTH   // 1 - 12
 * BYTE 12: YEAR    // 0 - 63
 * BYTE 13: CRCH
 * BYTE 14: CRCL
 * BYTE 15: ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Delete all tag logs in device memory
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_DEL_ALL_TAG_LOGS )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_ALL_TAG_LOGS_DELETED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Delete all selected tag types or all tags
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_DEL_ALL_TAGS )
 * BYTE 3 : DATA SIZE ( 1 )
 * BYTE 4 : TAG TYPE ( 1: USERS / 2: NODES / 3: EVENT / 4: ALL )
 * BYTE 5 : CRCH
 * BYTE 6 : CRCL
 * BYTE 7 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_ALL_USERS_DELETED / rsp_ALL_NODES_DELETED / rsp_ALL_EVENTS_DELETED / rsp_ALL_TAGS_DELETED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Read a tag and send now.
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_READ_TAG )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  : if a card reads in 5 secons.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_NEXT_LOG )
 * BYTE 3 : Data size ( 3 )
 * BYTE 4 : TAG_DATA[ 2 ]
 * BYTE 5 : TAG_DATA[ 1 ]
 * BYTE 6 : TAG_DATA[ 0 ]
 * BYTE 7 : CRCH
 * BYTE 8 : CRCL
 * BYTE 9 : ETX
 *
 * 
 * DEVICE RESPONSE  : if a card not reads in 5 secons.
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_TIMEOUT_OCCURED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 * 
 * 5 SANİYE İÇİNDE KART OKUTULMAZ İSE UYARI MESAJI YOLLA rsp_TIMEOUT_OCCURED yollar.
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Generate a beep signal from buzzer.
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_BEEP )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  : No response
 * ------------------
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Start transfer for sensor logs
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_START_TRANSFER_SENSOR_LOG )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_SENSOR_LOG_TRANSFER_READY )
 * BYTE 3 : Data size ( 2 )
 * BYTE 4 : Total log count High Byte
 * BYTE 5 : Total log count Low Byte
 * BYTE 6 : CRCH
 * BYTE 7 : CRCL
 * BYTE 8 : ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Get next sensor log
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_GET_NEXT_SENSOR_LOG )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( cmd_GET_NEXT_SENSOR_LOG )
 * BYTE 3 : Data size ( 7 )
 * BYTE 4 : SENSOR_DATA[ 0 ]
 * BYTE 5 : SECOND  // 0 - 59
 * BYTE 6 : MINUTE  // 0 - 59
 * BYTE 7 : HOUR    // 0 - 23
 * BYTE 8 : DAY     // 1 - 31
 * BYTE 9: MONTH   // 1 - 12
 * BYTE 10: YEAR    // 0 - 63
 * BYTE 11: CRCH
 * BYTE 12: CRCL
 * BYTE 13: ETX
 *
 *----------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Delete all logs in device memory
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_DEL_ALL_SENSOR_LOGS )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_ALL_SENSOR_LOGS_DELETED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Get tag capacity
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_GET_TAG_CAPACITY )
 * BYTE 3 : DATA SIZE ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_GET_TAG_CAPACITY )
 * BYTE 3 : Data size ( 6 )
 * BYTE 4 : Total users count High Byte
 * BYTE 5 : Total users count Low Byte
 * BYTE 6 : Total nodes count High Byte
 * BYTE 7 : Total nodes count Low Byte
 * BYTE 8 : Total event count High Byte
 * BYTE 9 : Total event count Low Byte
 * BYTE 10: CRCH
 * BYTE 11: CRCL
 * BYTE 12: ETX
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Add a alarm for a day
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_ADD_ALARM_FOR_A_DAY )
 * BYTE 3 : DATA SIZE ( 4 )
 * BYTE 4 : DAY OF WEEK ( 1 - 7 ) //Sunday = 1
 * BYTE 5 : HOUR
 * BYTE 6 : SECOND
 * BYTE 7 : ALARM TIME ( 1 - 255 ) // 1 to 255 seconds
 * BYTE 8 : CRCH
 * BYTE 9 : CRCL
 * BYTE 10: ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_ALARM_ADDED )
 * BYTE 3 : Data size ( 0 )
 * BYTE 4 : CRCH
 * BYTE 5 : CRCL
 * BYTE 6 : ETX
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 * HOST COMMAND     : Add a alarm for a day
 * ------------------
 *
 * BYTE 1 : STX
 * BYTE 2 : COMMAND ( cmd_DEL_ALL_ALARMS_FOR_A_DAY )
 * BYTE 3 : DATA SIZE ( 1 )
 * BYTE 4 : DAY OF WEEK ( 1 - 7 ) //Sunday = 1
 * BYTE 5 : CRCH
 * BYTE 6 : CRCL
 * BYTE 7 : ETX
 *
 *
 * DEVICE RESPONSE  :
 * ------------------
 * BYTE 1 : STX
 * BYTE 2 : Response ( rsp_ALARMS_DELETED )
 * BYTE 3 : Data size ( 1 )
 * BYTE 4 : DAY OF WEEK ( 1 - 7 ) //Sunday = 1
 * BYTE 5 : CRCH
 * BYTE 6 : CRCL
 * BYTE 7 : ETX
 *
 *----------------------------------------------------------------------------*/


/* DEFINE TYPES HERE ---------------------------------------------------------*/
/* DECLARE EXTERNAL VARIABLES HERE -------------------------------------------*/
/* DEFINITION OF MACROS ------------------------------------------------------*/
/* DECLARATION OF FUNCTIONS --------------------------------------------------*/


#endif