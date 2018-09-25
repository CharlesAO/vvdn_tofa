///
/// @file DrvEth.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvEth
/// @{
/// @brief     Driver for GRETH GBIT module
///



// 1: Includes
// ----------------------------------------------------------------------------

#include <DrvRegUtils.h>
#include "registersMyriad.h"
#include "mv_types.h"
#include "stdio.h"
#include "DrvEth.h"
#include "assert.h"
#include <DrvI2cMaster.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
static u32 EthernetI2CErrorHandler(I2CM_StatusType i2cCommsError, u32 slaveAddr, u32 regAddr);

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data 
// ----------------------------------------------------------------------------
static I2CM_Device Ethi2cHandle;
static u32 PHYAddr;
static tyI2cConfig EthI2CConfig =
    {
        .device                = 0,
        .sclPin                = 0,
        .sdaPin                = 0,
        .speedKhz              = 0,
        .addressSize           = 0,
        .errorHandler          = &EthernetI2CErrorHandler,
    };

static s32 DrvEthI2CConfigured = 0;

static u8 ETHprotocolWrite[] =
    {
        S_ADDR_WR,
        R_ADDR_L,
        DATAW,
        LOOP_MINUS_1
    };

static u8 ETHprotocolRead[] =
    {
        S_ADDR_WR,
        R_ADDR_L,
        S_ADDR_RD,
        DATAR,
        LOOP_MINUS_1
    };

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static u32 EthernetI2CErrorHandler(I2CM_StatusType i2cCommsError, u32 __attribute__((unused)) slaveAddr, u32 __attribute__((unused)) regAddr)
{
    //printf("\nI2C Error (%d) Slave (%02X) Reg (%02X)",i2cCommsError,slaveAddr,regAddr);
    return i2cCommsError; // Because we haven't really handled the error, pass it back to the caller
}

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void DrvEthTWISetup(u32 EthPhyDevice, u32 Myriad2I2CInterface, u8 sclPin, u8 sdaPin, u32 speedKhz, u32 addrSize)
{
    PHYAddr=EthPhyDevice;
    EthI2CConfig.device=Myriad2I2CInterface;
    EthI2CConfig.sclPin=sclPin;
    EthI2CConfig.sdaPin=sdaPin;
    EthI2CConfig.speedKhz=speedKhz;
    EthI2CConfig.addressSize=addrSize;
    DrvI2cMInitFromConfig(&Ethi2cHandle, &EthI2CConfig);    // setup i2c module
    DrvEthI2CConfigured=1;
    return;
}

u32 DrvEthTWIRead(u32 RegAddr, u32 *ErrorCode)
{
    u8 bytes[2];
    if (DrvEthI2CConfigured==1){
        unsigned int myVal;
        DrvI2cMTransaction(&Ethi2cHandle, PHYAddr, RegAddr, ETHprotocolRead, bytes, 2);
        myVal=bytes[0]<<8 | (bytes[1]);
        if (ErrorCode!=NULL){
            //Mark transmission OK
            *ErrorCode=0;
        }
        return myVal;
    }else{
        //Some error here
        if (ErrorCode!=NULL){
            *ErrorCode=-1;
        }
        return -1;
    }
}

void DrvEthTWIWrite(u32 RegAddr, u32 Value,u32 *ErrorCode)
{
    u8 bytes[2];
    if (DrvEthI2CConfigured==1){
        bytes[0]=(Value & 0xFF00) >> 8;
        bytes[1]=(Value & 0xFF);
        DrvI2cMTransaction(&Ethi2cHandle, PHYAddr, RegAddr, ETHprotocolWrite, bytes, 2);
        if (ErrorCode!=NULL){
            //Mark transmission OK
            *ErrorCode=0;
        }
    }else{
        //Some error here or just return
        if (ErrorCode!=NULL){
            //Mark transmission NOK
            *ErrorCode=-1;
        }
    }
    return;
}

void DrvEthSetDescriptor(u32 Length, u32 CtrlWord, u32 AddressOfDescriptor, u32 AddressPointedByDescriptor)
{
    *((u32 *)AddressOfDescriptor)        = (Length & DRV_ETH_TX_DESC_LENGTH_MASK) | (CtrlWord & (~DRV_ETH_TX_DESC_LENGTH_MASK));
    *((u32 *)(AddressOfDescriptor + 4))  = AddressPointedByDescriptor;
}

void DrvEthSetMAC2(u8* mac_6bytes)
{
    u32 macHi = 0, macLo = 0;
    
    macHi |= mac_6bytes[4];
    macHi |= mac_6bytes[5]<<8;
    
    macLo |= mac_6bytes[0];
    macLo |= mac_6bytes[1]<<8;
    macLo |= mac_6bytes[2]<<16;
    macLo |= mac_6bytes[3]<<24;
     
    DrvEthSetMAC(macHi, macLo);
}


void DrvEthSetMAC(u32 macHi, u32 macLo)
{
    SET_REG_WORD(DRV_ETH_MAC_MSB, macHi);
    SET_REG_WORD(DRV_ETH_MAC_LSB, macLo);     
}

void DrvEthInit(void)
{
    // reset 
    SET_REG_WORD(DRV_ETH_CTRL, DRV_ETH_CTRL_RS);
    // wait while the ctrl resets itself 
    while ((GET_REG_WORD_VAL(DRV_ETH_CTRL) & DRV_ETH_CTRL_RS) == DRV_ETH_CTRL_RS) ; 
    
    SET_REG_WORD(DRV_ETH_CTRL, DRV_ETH_CTRL_TI      // tx int enable 
                                | DRV_ETH_CTRL_RI    // rx int enable 
                                | DRV_ETH_CTRL_FD    // full duplex
                                | DRV_ETH_CTRL_PR    // promiscuous mode enabled
                                | DRV_ETH_CTRL_PI  );  // enable PHY int 
    
    SET_REG_WORD(DRV_ETH_STAT, 0xFFFFFFFF); // this should clear every bit and also make the sim not see XXX
}

void DrvEthDescriptorInit(u32 CtrlWord, u32 DescTxPointer, u32 DescRxPointer)
{
    SET_REG_WORD(DRV_ETH_TX_DESC, DescTxPointer);
    SET_REG_WORD(DRV_ETH_RX_DESC, DescRxPointer);          
    SET_REG_WORD(DRV_ETH_CTRL, GET_REG_WORD_VAL(DRV_ETH_CTRL) | CtrlWord);     
}

u32 DrvEthMdioCom(u32 data, u8 phyAddr, u8 regAddr, u8 WrNRd)
{
    u32 retVal;
    
    retVal = 0;
    // wait while the MDIO is busy
    while ((GET_REG_WORD_VAL(DRV_ETH_MDIO_CTRL) & DRV_ETH_MDIO_CS_BU) == DRV_ETH_MDIO_CS_BU);

    // write the data in the proper addr reg
    SET_REG_WORD(DRV_ETH_MDIO_CTRL, ((data   <<   DRV_ETH_MDIO_CS_DATA_POS) & DRV_ETH_MDIO_CS_DATA_MASK)    |
                                    ((phyAddr<<DRV_ETH_MDIO_CS_PHYADDR_POS) & DRV_ETH_MDIO_CS_PHYADDR_MASK) |
                                    ((regAddr<<DRV_ETH_MDIO_CS_REGADDR_POS) & DRV_ETH_MDIO_CS_REGADDR_MASK) |
                                                                               (WrNRd & DRV_ETH_MDIO_CS_WR) | 
                                                                      ((~WrNRd << 1) & DRV_ETH_MDIO_CS_RD) );

    // wait to complete the operation                                       
    while ((GET_REG_WORD_VAL(DRV_ETH_MDIO_CTRL) & DRV_ETH_MDIO_CS_BU) == DRV_ETH_MDIO_CS_BU);
    
    if (!WrNRd) 
    {
        retVal = GET_REG_WORD_VAL(DRV_ETH_MDIO_CTRL);
    }

    return retVal;
}

///@}
