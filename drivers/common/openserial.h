/**
\brief Declaration of the "openserial" driver.

\author Fabien Chraim <chraim@eecs.berkeley.edu>, March 2012.
*/

#ifndef __OPENSERIAL_H
#define __OPENSERIAL_H

#include "opendefs.h"

/**
\addtogroup drivers
\{
\addtogroup OpenSerial
\{
*/

//=========================== define ==========================================

/**
\brief Number of bytes of the serial output buffer, in bytes.

\warning should be exactly 256 so wrap-around on the index does not require
         the use of a slow modulo operator.
*/
#define SERIAL_OUTPUT_BUFFER_SIZE 256 // leave at 256!

/**
\brief Number of bytes of the serial input buffer, in bytes.

\warning Do not pick a number greater than 255, since its filling level is
         encoded by a single byte in the code.
*/
#define SERIAL_INPUT_BUFFER_SIZE  200

/// Modes of the openserial module.
enum {
   MODE_OFF    = 0, ///< The module is off, no serial activity.
   MODE_INPUT  = 1, ///< The serial is listening or receiving bytes.
   MODE_OUTPUT = 2  ///< The serial is transmitting bytes.
};

// frames sent mote->PC
#define SERFRAME_MOTE2PC_DATA               ((uint8_t)'D')
#define SERFRAME_MOTE2PC_STATUS             ((uint8_t)'S')
#define SERFRAME_MOTE2PC_INFO               ((uint8_t)'I')
#define SERFRAME_MOTE2PC_ERROR              ((uint8_t)'E')
#define SERFRAME_MOTE2PC_CRITICAL           ((uint8_t)'C')
#define SERFRAME_MOTE2PC_REQUEST            ((uint8_t)'R')
#define SERFRAME_MOTE2PC_SNIFFED_PACKET     ((uint8_t)'P')

// frames sent PC->mote
#define SERFRAME_PC2MOTE_SETROOT            ((uint8_t)'R')
#define SERFRAME_PC2MOTE_RESET              ((uint8_t)'Q')
#define SERFRAME_PC2MOTE_DATA               ((uint8_t)'D')
#define SERFRAME_PC2MOTE_TRIGGERSERIALECHO  ((uint8_t)'S')
#define SERFRAME_PC2MOTE_COMMAND            ((uint8_t)'C')

//=========================== typedef =========================================

enum {
   COMMAND_SET_EBPERIOD          =  0,
   COMMAND_SET_CHANNEL           =  1,
   COMMAND_SET_KAPERIOD          =  2,
   COMMAND_SET_DIOPERIOD         =  3,
   COMMAND_SET_DAOPERIOD         =  4,
   COMMAND_SET_DAGRANK           =  5,
   COMMAND_SET_SECURITY_STATUS   =  6,
   COMMAND_SET_SLOTFRAMELENGTH   =  7,
   COMMAND_SET_ACK_STATUS        =  8,
   COMMAND_SET_6P_ADD            =  9,
   COMMAND_SET_6P_DELETE         = 10,
   COMMAND_SET_6P_COUNT          = 11,
   COMMAND_SET_6P_LIST           = 12,
   COMMAND_SET_6P_CLEAR          = 13,
   COMMAND_SET_SLOTDURATION      = 14,
   COMMAND_SET_6PRESPONSE        = 15,
   COMMAND_SET_UINJECTPERIOD     = 16,
   COMMAND_MAX                   = 17,
};

//=========================== module variables ================================

typedef struct {
   // admin
   uint8_t    mode;
   uint8_t    debugPrintCounter;
   // input
   uint8_t    reqFrame[1+1+2+1]; // flag (1B), command (2B), CRC (2B), flag (1B)
   uint8_t    reqFrameIdx;
   uint8_t    lastRxByte;
   bool       busyReceiving;
   bool       inputEscaping;
   uint16_t   inputCrc;
   uint8_t    inputBufFill;
   uint8_t    inputBuf[SERIAL_INPUT_BUFFER_SIZE];
   // output
   bool       outputBufFilled;
   uint16_t   outputCrc;
   uint8_t    outputBufIdxW;
   uint8_t    outputBufIdxR;
   uint8_t    outputBuf[SERIAL_OUTPUT_BUFFER_SIZE];
} openserial_vars_t;

//=========================== prototypes ======================================

void    openserial_init(void);
owerror_t openserial_printStatus(uint8_t statusElement, uint8_t* buffer, uint8_t length);
owerror_t openserial_printInfo(uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printError(uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printCritical(uint8_t calling_component, uint8_t error_code,
                              errorparameter_t arg1,
                              errorparameter_t arg2);
owerror_t openserial_printData(uint8_t* buffer, uint8_t length);
owerror_t openserial_printPacket(uint8_t* buffer, uint8_t length, uint8_t channel);
uint8_t openserial_getNumDataBytes(void);
uint8_t openserial_getInputBuffer(uint8_t* bufferToWrite, uint8_t maxNumBytes);
void    openserial_startInput(void);
void    openserial_startOutput(void);
void    openserial_stop(void);
bool    debugPrint_outBufferIndexes(void);
void    openserial_echo(uint8_t* but, uint8_t bufLen);

// interrupt handlers
void    isr_openserial_rx(void);
void    isr_openserial_tx(void);

/**
\}
\}
*/

#endif
