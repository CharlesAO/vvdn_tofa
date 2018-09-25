///
/// @file DrvEth.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvEth Eth Driver
/// @{
/// @brief     Low-level driver for GRETH GBIT interface
///


#ifndef DRV_ETH_H
#define DRV_ETH_H

#include "mv_types.h"
#include "registersMyriad.h"
#include "swcLeonUtils.h"
#include "DrvEthDefines.h"

typedef struct 
{ 
  u32 Length       : 11;   // 0 - 10
  u32 Enable       : 1;    // 11
  u32 Wrap         : 1;    // 12
  u32 IntEnable    : 1;    // 13
  u32 UnderrunError: 1;    // 14
  u32 AtLimitError : 1;    // 15
  u32 LateColision : 1;    // 16
  u32 More         : 1;    // 17
  u32 IPCheckSum   : 1;    // 18
  u32 TCPCheckSum  : 1;    // 19
  u32 UDPCheckSum  : 1;    // 20
  u32 reserved     : 11;   // 21 - 31
} tCtrlTxDescriptor;

typedef struct
{
   tCtrlTxDescriptor ctrl;
   u32               address;   
} tTxDescriptor, *pTxDescriptor;


typedef struct
{
   u32 Length        :11;  // 0 - 10
   u32 Enable        : 1;  // 11
   u32 Wrap          : 1;  // 12
   u32 IntEnable     : 1;  // 13
   u32 AlignError    : 1;  // 14
   u32 FrameTooLong  : 1;  // 15
   u32 CRCError      : 1;  // 16
   u32 OverrunError  : 1;  // 17
   u32 LengthError   : 1;  // 18
   u32 IPDetected    : 1;  // 19
   u32 IPError       : 1;  // 20
   u32 UDPDetected   : 1;  // 21
   u32 UDPError      : 1;  // 22
   u32 TCPDetected   : 1;  // 23
   u32 TCPError      : 1;  // 24
   u32 IPFragment    : 1;  // 25
   u32 MulticastAddr : 1;  // 26
   u32 reserved      : 5;  // 27-31
}  tCtrlRxDescriptor;

typedef struct
{
   tCtrlRxDescriptor ctrl;
   u32               address;   
} tRxDescriptor, *pRxDescriptor;

#ifdef __cplusplus
extern "C" {
#endif

void DrvEthSetDescriptor(u32 Length, u32 CtrlWord, u32 AddressOfDescriptor, u32 AddressPointedByDescriptor);
void DrvEthSetMAC(u32 macHi, u32 macLo);
void DrvEthSetMAC2(u8* mac_6bytes);
void DrvEthInit(void);
void DrvEthDescriptorInit(u32 CtrlWord, u32 DescTxPointer, u32 DescRxPointer);
void DrvEthTWISetup(u32 EthPhyDevice, u32 Myriad2I2CInterface, u8 sclPin, u8 sdaPin, u32 speedKhz, u32 addrSize);
//ErrorCode may also be NULL if error handling is not desired
void DrvEthTWIWrite(u32 RegAddr, u32 Value,u32 *ErrorCode);
//ErrorCode may also be NULL if error handling is not desired
u32 DrvEthTWIRead(u32 RegAddr, u32 *ErrorCode);

u32 DrvEthMdioCom(u32 data, u8 phyAddr, u8 regAddr, u8 WrNRd);

#ifdef __cplusplus
}
#endif

#endif

///@}
