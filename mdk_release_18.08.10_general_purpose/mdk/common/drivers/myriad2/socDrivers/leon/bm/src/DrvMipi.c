///
/// @file DrvMipi.c
/// 
/// 
/// @author Lucian Vancea
/// @ingroup DrvMipi
/// @{
/// @brief MIPI Driver Implementation
///

#include <DrvRegUtils.h>
#include "registersMyriad.h"
#include "mv_types.h"
#include "assert.h"
#include <stdio.h>

#include "DrvMipi.h"
#include "DrvMipiDefines.h"
#include "DrvMssDefines.h"

#define DRV_MIPI_FREQ_RANGE_NO (40)



// next 5 entities are used to check that the number of lanes per controller is correct
#define D_DRV_MIPI_LANES_FIELD (5)
#define D_DRV_MIPI_NO_POSSIBLE_LANE_CONFIGS_PER_CTRL (6)
static u32 dDrvMipiLanesField[D_DRV_MIPI_LANES_FIELD] = { 0, 1, 3, 5, 7}; // field in the HS_CTRL reg which specifies how many lanes to be used, all possible values, number of lanes is (value + 1 )
// this 2 arrays specify the number of lane configs from the previous array that can be used, starting from position 0 and counting up
static u32 dDrvMipiRxNoPossibleLaneConfigsPerCtrl[D_DRV_MIPI_NO_POSSIBLE_LANE_CONFIGS_PER_CTRL] = {5, 2, 3, 4, 3, 2}; // how many of them are possible per controller in rx mode
static u32 dDrvMipiTxNoPossibleLaneConfigsPerCtrl[D_DRV_MIPI_NO_POSSIBLE_LANE_CONFIGS_PER_CTRL] = {0, 0, 0, 3, 0, 3}; // how many of them are possible per controller in tx mode





static u16 dDrvMipiFreqRangeMHz[DRV_MIPI_FREQ_RANGE_NO] = {
80, 90, 100, 110, 130, 140, 150, 170, 180, 200, 220, 240, 250, 270,
300, 330, 360, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900,
950, 1000, 1050, 1100, 1150, 1200, 1250, 1300, 1350, 1400, 1450, 1500};

static u8 dDrvMipihsfreqrange[DRV_MIPI_FREQ_RANGE_NO - 1] = {
0,16,32,1,17,33,2,18,34,3,19,35,4,20,5,21,37,6,22,7,23,8,24,9,25,41,57,10,26,42,58,11,27,43,59,12,28,44,60};

// #define DRV_MIPI_DEBUG
// #define DRV_MIPI_DEBUG1
// #define DRV_MIPI_DEBUG2

#ifdef DRV_MIPI_DEBUG
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

#ifdef DRV_MIPI_DEBUG1
#define DPRINTF2(...) printf(__VA_ARGS__)
#else
#define DPRINTF2(...)
#endif

#ifdef DRV_MIPI_DEBUG2
#define DPRINTF3(...) printf(__VA_ARGS__)
#else
#define DPRINTF3(...)
#endif


// if for some reason, only one PHY has to be cleared
void DrvMipiTestClear(u8 MipiCtrlNo)
{
      // test clear
     SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, 1<<MipiCtrlNo);
     SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, 0);
}

void DrvMipiTestClearAll(void)
{
      // test clear all
     SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, 0x3F);
     SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, 0);
}


 u8 DrvMipiTestModeSendCmd(u8 MipiCtrlNo, u8 Command, u8 *DataIn, u8 *DataOut, u8 Length)
 {
      int i;
      u32 RetVal, DOut;

      // test input data
      assert(MipiCtrlNo <= 5);
/*
                 1____2
      CLR      __|    |________________________________________________________
                              3_________________7
      EN       _______________|                 |_____________________________
                                  4_________6                   9_____10
      CLK      ___________________|         |___________________|     |_______
                                       5                     8
      DIN      ------------------------<COMMAND>------------<  DATA  >---------
*/

     // SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, (~(1<<MipiCtrlNo)) & 0x3F);      // keep TEST CLR hi for other D-PHYs
      // send test command
      SET_REG_WORD(MIPI_DPHY_TEST_EN_ADDR,  1<<MipiCtrlNo); //3
      SET_REG_WORD(MIPI_DPHY_TEST_CLK_ADDR, 1<<MipiCtrlNo); //4
      SET_REG_WORD(MIPI_DPHY_TEST_DIN_ADDR, Command);       //5
      SET_REG_WORD(MIPI_DPHY_TEST_CLK_ADDR, 0);             //6
      SET_REG_WORD(MIPI_DPHY_TEST_EN_ADDR,  0);             //7


      // because DOUT FROM phys 0&1 share a register and so do 2&3 and 4&5
      RetVal = GET_REG_WORD_VAL(MIPI_DPHY_TEST_DOUT_DPHY01_ADDR + 4*(MipiCtrlNo>>1));
      // Higher numbered dphy's output is placed on bites 8->15
      RetVal >>= 8*(MipiCtrlNo & 1);
      RetVal &= 0xFF;

      if (DataIn != NULL)
      // send specific data , repeat steps 8, 9 and 19
          for (i=0 ; i<Length ; ++i)
          {
                    SET_REG_WORD(MIPI_DPHY_TEST_DIN_ADDR, (*DataIn) & 0xFF);//8
                    ++DataIn;
                    SET_REG_WORD(MIPI_DPHY_TEST_CLK_ADDR, 1<<MipiCtrlNo); //9
                    SET_REG_WORD(MIPI_DPHY_TEST_CLK_ADDR, 0);             //10

                    DOut = GET_REG_WORD_VAL(MIPI_DPHY_TEST_DOUT_DPHY01_ADDR + 4*(MipiCtrlNo>>1));
                    // Higher numbered dphy's output is placed on bites 8->15
                    DOut >>= 8*(MipiCtrlNo & 1);
                    if (DataOut != NULL)
                    {
                       *DataOut = DOut & 0xFF;
                       ++DataOut;
                    }
          }


     // SET_REG_WORD(MIPI_DPHY_TEST_CLR_ADDR, 0x3F);      // keep TEST CLR hi for all D-PHYs
      return RetVal;
 }

 // desiredClkKHz should be double of the clock lane
 u32 DrvMipiSetRxRange(u32 mipiPhyNo, u32 desiredClkKHz)
 {
        int i; // pll params
        u8 dataIn, dataOut;


         //find the range
        for (i=0 ; i<DRV_MIPI_FREQ_RANGE_NO ; ++i )
        {
                if (desiredClkKHz <= (dDrvMipiFreqRangeMHz[i] * 1000))
                      break;
        }

        // if smaller then min freq or bigger then max freq, not possible
        if ((i==0) && (desiredClkKHz<D_DRV_MIPI_CTRL_MIN_TRAN_RATE_MBPS*1000))
            return -1;

        if ((i == (DRV_MIPI_FREQ_RANGE_NO-1)) && (desiredClkKHz > (dDrvMipiFreqRangeMHz[i]*1000)))
            return -1;

        // programm the range
        dataIn = (dDrvMipihsfreqrange[i-1] << 1) & 0x7E;
        DrvMipiTestModeSendCmd(mipiPhyNo, D_MIPI_TCMD_HS_RX_CTRL_L0, &dataIn, &dataOut, 1);

        DPRINTF2("    Prog hsfreqrange for DPHY %d with %08X\n",mipiPhyNo, dataIn);
        return i;
 }


u32 DrvMipiRxClkTermCtrl(u32 mipiPhyNo, eDrvMipiTermResMode TermMode)
{
    u8 retVal;
    u8 dataIn, dataOut;
    if (TermMode == DRV_MIPI_TERM_RES_ENABLE)
    {
       dataIn = (2<<6) |   // for command D_MIPI_TCMD_HS_RX_CTRL_CL, set 0
                (0<<5) |   // for command D_MIPI_TCMD_HS_RX_CTRL_CL, 1 - control TERM RES from test interface ; 0 - control by FSM
                0       ;  // don't touch the other things
    }
    else
    {
       dataIn = (2<<6) |   // for command D_MIPI_TCMD_HS_RX_CTRL_CL, set 0
                (1<<5) |   // for command D_MIPI_TCMD_HS_RX_CTRL_CL, 1 - control TERM RES from test interface ; 0 - control by FSM
                (0<<4) |   // for command D_MIPI_TCMD_HS_RX_CTRL_CL, 1 - enable term res, 0 disable term res
                0;         // don't touch the other things
    }

    retVal = DrvMipiTestModeSendCmd(mipiPhyNo, D_MIPI_TCMD_HS_RX_CTRL_CL, &dataIn, &dataOut, 1);

    return (retVal << 8) | (dataOut & 0xFF) ;
}




 // note mipiCtrlNo refers to phy number - this should be changed to reflect this
 u32 DrvMipiPllProg(u32 mipiCtrlNo, u32 refClkKHz, u32 desiredClkKHz)
 {
       int m, n, i; // pll params
       int MemM, MemN;
       u32 MemFreqDifference, TempFreqDifference, MemPllFreq;
       float DivFreq;
       float PllFreq;

       u8 dataIn, dataOut;

       int Solutions;
       MemM = 0;
       MemN = 0;
       MemPllFreq = 0;

       //      Fout = (M/ N) * RefClk;
       //      40MHz >= RefClk / n >= 5

       // m = 1, 3, 5, ... , 297, 299 => M = 2, 4, 6, ..., 298, 300
       // n = 0, 1, 2, ... , 98, 99   => N = 1, 2, 3, ..., 99, 100
       MemFreqDifference = 0xFFFFFFFF; // init with the worst case
       Solutions = 0;

       DPRINTF3("  DRV MIPI mipi PHY no %d\n", mipiCtrlNo);
       DPRINTF3("  DRV MIPI refClkKHz %d\n", refClkKHz);
       DPRINTF3("  DRV MIPI desiredClkKHz %d\n", desiredClkKHz);

        for (n = 0; n<0x64; n++)
        {
             DivFreq = (float)refClkKHz  / (float)(n+1);
             DPRINTF3("n %d\n", n);
             if (DivFreq >= 5000)
                     if (DivFreq <= 40000)
                     {
                             // so N is good, lets find M
                             for (m = 1 ; m < 300 ; m+=2)
                             {
                                 // DPRINTF1("m %d\n", m);
                                  PllFreq = DivFreq * (m+1);
                                  if ((u32)PllFreq > desiredClkKHz)
                                      TempFreqDifference = (u32)PllFreq - desiredClkKHz;
                                  else
                                      TempFreqDifference = desiredClkKHz - (u32)PllFreq ;

                                  if (TempFreqDifference < MemFreqDifference)
                                  {
                                      // if there is a better match for M & N, memorise
                                      MemM = m;
                                      MemN = n;
                                      MemFreqDifference = TempFreqDifference;
                                      MemPllFreq = (u32)PllFreq;
                                      ++Solutions;
                                  }
                             }
                     }

        }

        if (Solutions == 0)
              return 0; // no matching freq found

        //find the range
        for (i=0 ; i< DRV_MIPI_FREQ_RANGE_NO ; ++i )
        {
                if (MemPllFreq <= (dDrvMipiFreqRangeMHz[i] * 1000))
                      break;
        }

        // if smaller then min freq or bigger then max freq, not possible
        if ((i==0) && (desiredClkKHz<D_DRV_MIPI_CTRL_MIN_TRAN_RATE_MBPS*1000))
            return 0;

        if ((i == (DRV_MIPI_FREQ_RANGE_NO-1)) && (MemPllFreq > (dDrvMipiFreqRangeMHz[i]*1000)))
            return 0;

        // with M and N found that have the closest match to the desired frequency, programm the PLL
        DPRINTF1(" DRV MIPI programming params\n");

        // programm the range
        dataIn = (dDrvMipihsfreqrange[i-1] << 1) & 0x7E;
        DrvMipiTestModeSendCmd(mipiCtrlNo, D_MIPI_TCMD_HS_RX_CTRL_L0, &dataIn, &dataOut, 1);

        // program N
        dataIn = MemN;
        DrvMipiTestModeSendCmd(mipiCtrlNo, D_MIPI_TCMD_PLL_IN_DIV, &dataIn, &dataOut, 1);

        // use N & M  - this has to be done before M is programmed
        dataIn = 0x30;
        DrvMipiTestModeSendCmd(mipiCtrlNo, D_MIPI_TCMD_PLL_IN_LOOP_DIV, &dataIn, &dataOut, 1);


        // program M low part
        dataIn = MemM & 0x1F;
        DrvMipiTestModeSendCmd(mipiCtrlNo, D_MIPI_TCMD_PLL_LOOP_DIV, &dataIn, &dataOut, 1);

        // program M high part
        dataIn = ((MemM >> 5) & 0xF) | 0x80;
        DrvMipiTestModeSendCmd(mipiCtrlNo, D_MIPI_TCMD_PLL_LOOP_DIV, &dataIn, &dataOut, 1);

        return MemPllFreq;
 }

 // if timeout is 0 waits forever for the PLL to lock
 // otherwise it will wait for timeout cicles

int DrvMipiWaitPllLock(u32 mipiCtrlNo, u32 timeout)
{
     if (timeout == 0)
         while (1)
         {
              if ((GET_REG_WORD_VAL(MIPI_DPHY_PLL_LOCK_ADDR) & (1<<mipiCtrlNo)) == (u32)(1<<mipiCtrlNo))
                  return 1;
         }
     else
         do
         {
              if ((GET_REG_WORD_VAL(MIPI_DPHY_PLL_LOCK_ADDR) & (1<<mipiCtrlNo)) == (u32)(1<<mipiCtrlNo))
                  return 1;
         }
         while (timeout-- > 0);

    return 0;
}

u32 DrvMipiInitDphy(u32 mipiDphyNo)
{
   u32 RegOldVal;

   RegOldVal = GET_REG_WORD_VAL(MIPI_DPHY_INIT_CTRL_ADDR);
   DPRINTF3("  DRV MIPI %s -before- %08x\n", __FUNCTION__, RegOldVal);


   SET_REG_WORD(MIPI_DPHY_INIT_CTRL_ADDR, RegOldVal |
                                          (D_MIPI_DPHY_INIT_CTRL_EN_0 << mipiDphyNo));



   SET_REG_WORD(MIPI_DPHY_INIT_CTRL_ADDR,  RegOldVal |
                                          (D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_0 << mipiDphyNo) | // according to the spec 5.2.1 SHUTDOWNZ is first then RSTZ
                                          (D_MIPI_DPHY_INIT_CTRL_EN_0 << mipiDphyNo));



   SET_REG_WORD(MIPI_DPHY_INIT_CTRL_ADDR, RegOldVal |
                                          (D_MIPI_DPHY_INIT_CTRL_CLK_CONT_0 << mipiDphyNo) |
                                          (D_MIPI_DPHY_INIT_CTRL_SHUTDOWNZ_0 << mipiDphyNo)|
                                          (D_MIPI_DPHY_INIT_CTRL_EN_0 << mipiDphyNo) |
                                          (D_MIPI_DPHY_INIT_CTRL_RSTZ_0 << mipiDphyNo) );
   DPRINTF3("  DRV MIPI %s -after - %08x\n", __FUNCTION__,  GET_REG_WORD_VAL(MIPI_DPHY_INIT_CTRL_ADDR));

   return RegOldVal;
}


void DrvMipiTxLPData(u32 mipiLaneNo, u32 data)
{
         SET_REG_WORD(MIPI_TX_LPDT_DATA_ADDR, data);
         SET_REG_WORD(MIPI_TX_LPDT_REQ_ADDR,     1 <<  mipiLaneNo);     // request LP tx on lane
}


void DrvMipiTxHSData(u32 ctrlNo, u32 data)
{
    if ((ctrlNo == DRV_MIPI_C3) || (ctrlNo == DRV_MIPI_C5))
    {
        SET_REG_WORD(MIPI0_TX_HS_SP_DATA_ADDR + 0x100*ctrlNo, data);
        SET_REG_WORD(MIPI0_TX_HS_SP_REQ_ADDR  + 0x100*ctrlNo, 1);
    }
    // else do nothing, other controllers can't transmit
}



/* CRC16 Polynomial, logically inverted 0x1021 for x^16+x^15+x^5+x^0 - MIPI DSI spec, annex B*/
static u16 CalculateCRC16( u8 *pcDataStream, u16 sNumberOfDataBytes )
 {
 /*
 sCRC16Result: the return of this function,
 sByteCounter: address pointer to count the number of the
 calculated data bytes
 cBitCounter: counter for bit shift (0 to 7)
 cCurrentData: byte size buffer to duplicate the calculated data
 byte for a bit shift operation
 */
 u16 sByteCounter;
 u8 cBitCounter;
 u8 cCurrentData;
 u16 sCRC16Result = 0xFFFF;
 static u16 gsCRC16GenerationCode = 0x8408;

          if ( sNumberOfDataBytes > 0 )
          {
                for ( sByteCounter = 0 ; sByteCounter < sNumberOfDataBytes ; sByteCounter++ )
                {
                      cCurrentData = *( pcDataStream + sByteCounter );
                      for ( cBitCounter = 0; cBitCounter < 8; cBitCounter++ )
                      {
                                if ( ( ( sCRC16Result & 0x0001 ) ^ ( ( 0x0001 * cCurrentData) & 0x0001 ) ) > 0 )
                                        sCRC16Result = ( ( sCRC16Result >> 1 ) & 0x7FFF ) ^ gsCRC16GenerationCode;
                                else
                                        sCRC16Result = ( sCRC16Result >> 1 ) & 0x7FFF;
                                cCurrentData = (cCurrentData >> 1 ) & 0x7F;
                      }
                 }
           }

 return sCRC16Result;
 }


void DrvMipiTxWaitForStopState(u32 phy)
{
  u32 state;
         state =0;
         while ( (state & (D_MIPI_DPHY_STAT_P0_L0_STOPS | D_MIPI_DPHY_STAT_P0_L1_STOPS)) == 0 )
         {
                   state = GET_REG_WORD_VAL(MIPI_DPHY_STAT_DPHY01_ADDR + 4*(phy>>1));
                   state >>= (phy & 1) * 8;
         }
}


u32 DrvMipiTxHSLPData(u32 ctrlNo,u32 header, u32 *data)
{
    u32 length;
    u16 crc;
    u8 retVal = 0;
    length  = header & 0xFFFF;

    if (length > 10) // max 12 bytes can be sent, but 2 have to be CRC
    {
          retVal = length - 10;
          length = 10;
          header = (header & 0xFF0000) | length;
    }

    // crc of the packet
    crc = CalculateCRC16((u8*)data, length);
    // add crc to the stream
    *(((u8*) data)+length)   = crc&0xFF;
    *(((u8*) data)+length+1) = (crc>>8) & 0xFF;

    //send data
    if ((ctrlNo == DRV_MIPI_C3) || (ctrlNo == DRV_MIPI_C5))
    {
           //set data
           SET_REG_WORD(MIPI0_TX_HS_DCS_PH_ADDR    + 0x100*ctrlNo, header);

           SET_REG_WORD(MIPI0_TX_HS_DCS_DATA0_ADDR + 0x100*ctrlNo, data[0]);
           if ((length+2) > 4)
           {
               SET_REG_WORD(MIPI0_TX_HS_DCS_DATA1_ADDR + 0x100*ctrlNo, data[1]);
               if ((length+2) > 8)
                      SET_REG_WORD(MIPI0_TX_HS_DCS_DATA2_ADDR + 0x100*ctrlNo, data[2]);
               else
                      SET_REG_WORD(MIPI0_TX_HS_DCS_DATA2_ADDR + 0x100*ctrlNo, 0);
           } else
           {
               SET_REG_WORD(MIPI0_TX_HS_DCS_DATA1_ADDR + 0x100*ctrlNo, 0);
               SET_REG_WORD(MIPI0_TX_HS_DCS_DATA2_ADDR + 0x100*ctrlNo, 0);
           }

           // send request
           SET_REG_WORD(MIPI0_TX_HS_DCS_REQ_ADDR   + 0x100*ctrlNo, 1);
    }
    // else do nothing, other controllers can't transmit
    return retVal;
}



void drvLocalLpEvent(tDrvMipiCallBack callBackLpdtEvent, tDrvMipiCallBack callBackUlpsEnter, tDrvMipiCallBack callBackUlpsExit)
{
    u32 mipiReg;

    u32 i;

    DPRINTF2("%s\n",__FUNCTION__);

    if (callBackLpdtEvent != NULL)
    {
            // check which LP events are on
            mipiReg = GET_REG_WORD_VAL(MIPI_RX_LP_EVENT_ADDR);

            for (i=0;i<12;++i)
                if ( (mipiReg & (1<<i)) == (u32)(1<<i) )
                {
                        callBackLpdtEvent(i, GET_REG_WORD_VAL(MIPI_RX_LPDT_DATA_LANE0_ADDR + i*4));  // give back the lane number and the data
                }

            for (i=12;i<24;++i)
                if ( (mipiReg & (1<<i)) == (u32)(1<<i) )
                {
                        callBackLpdtEvent(i, GET_REG_WORD_VAL(MIPI_RX_LP_TRIG_VAL_LANE0_ADDR + (i-12)*4));  // give back the lane number + 12 (tells it's trigger not lpdt data), and trigger value
                }
           // clear LP events
           SET_REG_WORD(MIPI_RX_LP_EVENT_ADDR, mipiReg);
    }

    if (callBackUlpsEnter != NULL)
    {
            // check which lanes have entered ULPS
            mipiReg = GET_REG_WORD_VAL(MIPI_RX_ULPS_EVENT_ADDR);
            for (i=0;i<18;++i)
                if ( (mipiReg & (1<<i)) == (u32)(1<<i) )
                {
                         callBackUlpsEnter(i, 0x0);
                }
           // clear ULPS enter events
           SET_REG_WORD(MIPI_RX_ULPS_EVENT_ADDR, mipiReg);
    }



   if (callBackUlpsExit != NULL)
   {
           // check which lanes have exited ULPS
           mipiReg = GET_REG_WORD_VAL(MIPI_RX_ULPS_EXIT_EVENT_ADDR);
            for (i=0;i<18;++i)
                if ( (mipiReg & (1<<i)) == (u32)(1<<i) )
                {
                         callBackUlpsExit(i, 0x0);  //
                }
           // clear ULPS enter events
           SET_REG_WORD(MIPI_RX_ULPS_EXIT_EVENT_ADDR, mipiReg);
   }

}

void drvLocalRxHsEvent(tDrvMipiCallBack callBack, u32 statReg)
{
    u32 i;
    DPRINTF2("%s\n",__FUNCTION__);

    if (callBack != NULL)
        for (i=0;i<6;++i)
             if ((statReg & (D_MIPI_IRQ_STAT_HS_RX_EVENT_0<<i)) == (u32)(D_MIPI_IRQ_STAT_HS_RX_EVENT_0<<i))
                    callBack(i, GET_REG_WORD_VAL(MIPI0_RX_HS_PH_ADDR + 0x100 * i)); // return ctrl number and header info

}

void drvLocalHsError(tDrvMipiCallBack callBackHsRxErr,
                     tDrvMipiCallBack callBackHsTxErr, u32 mipiStat) {
    u32 i;
    DPRINTF2("%s\n", __FUNCTION__);

    if (callBackHsRxErr != NULL)
        for (i = 0; i < 6; ++i)
            // check if ctrl X has reported an error and is in RX mode and is enabled
            if (((mipiStat & (D_MIPI_IRQ_STAT_HS_ERR_0 << i)) ==
                 (u32)(D_MIPI_IRQ_STAT_HS_ERR_0 << i)) &&
                ((GET_REG_WORD_VAL(MIPI0_HS_CTRL_ADDR + i * 0x100) &
                  (D_MIPI_HS_CTRL_MODE_TX | D_MIPI_HS_CTRL_EN)) ==
                 D_MIPI_HS_CTRL_EN)) {
                callBackHsRxErr(i, GET_REG_WORD_VAL(MIPI0_RX_HS_LLP_ERR_ADDR + 0x100 * i));
                // clear error reg
                SET_REG_WORD(MIPI0_RX_HS_LLP_ERR_ADDR + 0x100 * i, 0x1FF);
            }

    if (callBackHsTxErr != NULL) {
        for (i = 3; i < 6; i += 2) // only 2 tx controllers
            // check if ctrl X has reported an error and is in TX mode and is enabled
            if (((mipiStat & (D_MIPI_IRQ_STAT_HS_ERR_0 << i)) ==
                 (u32)(D_MIPI_IRQ_STAT_HS_ERR_0 << i)) &&
                ((GET_REG_WORD_VAL(MIPI0_HS_CTRL_ADDR + i * 0x100) &
                  (D_MIPI_HS_CTRL_MODE_TX | D_MIPI_HS_CTRL_EN)) ==
                 (D_MIPI_HS_CTRL_MODE_TX | D_MIPI_HS_CTRL_EN))) {
                callBackHsTxErr(i, GET_REG_WORD_VAL(MIPI0_TX_HS_LLP_ERR_ADDR + 0x100 * i));
                // clear error reg
                SET_REG_WORD(MIPI0_TX_HS_LLP_ERR_ADDR + 0x100 * i, 0xF);
            }
    }
}

void drvLocalDphyError(tDrvMipiCallBack callBackDphy, u32 statReg)
{
    u32 i;
    u32 mipiReg;

    DPRINTF2("%s\n",__FUNCTION__);
    mipiReg = GET_REG_WORD_VAL(MIPI_DPHY_INIT_CTRL_ADDR);
    if (callBackDphy != NULL)
            for (i = 0 ; i < 6 ; ++i)
            {
                  // check if the DPHY is enabled and has a reported error
                  if (((mipiReg & (1<<i)) == (u32)(1<<i)) && ((statReg & (1<<(i+1))) == (u32)(1<<(i+1))))
                  {
                                 callBackDphy(i, GET_REG_WORD_VAL(MIPI_DPHY_ERR_STAT_DPHY01_ADDR + (i>>1) * 4 )  >> (14 * (i&1))); // return DPHY number and DPHY error bits
                  }
            }
}

void drvLocalHsCfgError(tDrvMipiCallBack callBack)
{
    u32 i,j;
    u32 mipiReg;
    u32 tmp, goodNoLanes;

    DPRINTF2("%s\n",__FUNCTION__);

    // check registers
    if (callBack != NULL)
            for (i=0;i<6;++i)
            {
                  mipiReg = GET_REG_WORD_VAL(MIPI0_HS_CTRL_ADDR + i*0x100);
                  //if the controller is enabled
                  if ((mipiReg & D_MIPI_HS_CTRL_EN) == D_MIPI_HS_CTRL_EN)
                  {
                        // if TX mode
                        if ((mipiReg & D_MIPI_HS_CTRL_MODE_TX) == D_MIPI_HS_CTRL_MODE_TX)
                        {
                                // check that it's not set as TX when in can only be RX
                                if ((i!=3) && (i!=5))
                                {
                                       callBack(i, eMipiCfgErrCantBeTx);
                                }

                                // check number of active lanes selected
                                tmp = (mipiReg & D_MIPI_HS_CTRL_LANE_MASK) >> D_MIPI_HS_CTRL_LANE_POS; // get the field from the reg
                                goodNoLanes = 0;
                                for (j=0 ; j<dDrvMipiTxNoPossibleLaneConfigsPerCtrl[i]; j++)    // compare the field with the known good configs for this # controller
                                     if (tmp == dDrvMipiLanesField[j])
                                     {
                                         goodNoLanes = 1;
                                         break;
                                     }
                                if (goodNoLanes == 0)
                                     callBack(i, eMipiCfgErrWrongNoLanes);

                        }
                        else // if RX mode
                        {
                                // check number of active lanes selected
                                tmp = (mipiReg & D_MIPI_HS_CTRL_LANE_MASK) >> D_MIPI_HS_CTRL_LANE_POS;
                                goodNoLanes = 0;
                                for (j=0 ; j<dDrvMipiRxNoPossibleLaneConfigsPerCtrl[i]; j++)
                                     if (tmp == dDrvMipiLanesField[j])
                                     {
                                         goodNoLanes = 1;
                                         break;
                                     }
                                if (goodNoLanes == 0)
                                     callBack(i, eMipiCfgErrWrongNoLanes);
                        }
                  }
            }
}


// irq status check function
void DrvMipiStatusCheck(tPtrDrvMipiCallBackStruct fCallBacks)
{
    u32 mipiStatReg;
    tDrvMipiCallBackStruct fCalls;

    fCalls.LpdtEvent  = NULL;
    fCalls.UlpsEnter  = NULL;
    fCalls.UlpsExit   = NULL;
    fCalls.HsCfgError = NULL;
    fCalls.RxHsEvent  = NULL;
    fCalls.HsRxError  = NULL;
    fCalls.HsTxError  = NULL;
    fCalls.DphyError  = NULL;

    if (fCallBacks != NULL) // add the new functions if they are provided
    {
        if (fCallBacks->LpdtEvent != NULL)
            fCalls.LpdtEvent = fCallBacks->LpdtEvent;

        if (fCallBacks->UlpsEnter != NULL)
            fCalls.UlpsEnter = fCallBacks->UlpsEnter;

        if (fCallBacks->UlpsExit != NULL)
            fCalls.UlpsExit = fCallBacks->UlpsExit;

        if (fCallBacks->HsCfgError != NULL)
            fCalls.HsCfgError = fCallBacks->HsCfgError;

        if (fCallBacks->RxHsEvent != NULL)
            fCalls.RxHsEvent = fCallBacks->RxHsEvent;

        if (fCallBacks->HsRxError != NULL)
            fCalls.HsRxError = fCallBacks->HsRxError;

        if (fCallBacks->HsTxError != NULL)
            fCalls.HsTxError = fCallBacks->HsTxError;


        if (fCallBacks->DphyError != NULL)
            fCalls.DphyError = fCallBacks->DphyError;
    }

    mipiStatReg = GET_REG_WORD_VAL(MIPI_IRQ_STATUS_ADDR);

    if ((mipiStatReg & D_MIPI_IRQ_STAT_LP_EVENT) == D_MIPI_IRQ_STAT_LP_EVENT)
    {
          // check for particular LP events
          drvLocalLpEvent(fCalls.LpdtEvent, fCalls.UlpsEnter, fCalls.UlpsExit);
          // clear just the LP event bit
          SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, D_MIPI_IRQ_STAT_LP_EVENT);
    }

    if ((mipiStatReg & D_MIPI_IRQ_STAT_CFG_ERR) == D_MIPI_IRQ_STAT_CFG_ERR)
    {
          // check if config is OK and report via callback
          drvLocalHsCfgError(fCalls.HsCfgError);
          // clear bad config error
          SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, D_MIPI_IRQ_STAT_CFG_ERR);
    }

    if ((mipiStatReg & D_MIPI_IRQ_STAT_HS_RX_EVENT) != 0)
    {
          drvLocalRxHsEvent(fCalls.RxHsEvent, mipiStatReg);
          // clear all RX HS event
          SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, D_MIPI_IRQ_STAT_HS_RX_EVENT);

    }

    if ((mipiStatReg & D_MIPI_IRQ_STAT_HS_ERR) != 0)
    {
          // check for errors for each enabled ctrl
          drvLocalHsError(fCalls.HsRxError, fCalls.HsTxError, mipiStatReg);
          // clear HS errors
          SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, D_MIPI_IRQ_STAT_HS_ERR);
    }

    if ((mipiStatReg & D_MIPI_IRQ_STAT_DPHY_ERR) != 0)
    {
          // check for errors and call the call back function if there are errors
          drvLocalDphyError(fCalls.DphyError, mipiStatReg);
          // clear DPHY errors
          SET_REG_WORD(MIPI_IRQ_CLEAR_ADDR, D_MIPI_IRQ_STAT_DPHY_ERR);
    }


}

eDrvMipiInitFuncRet DrvMipiInit(tPtrDrvMipiControllerConfigStruct mipiCtrlCfg)
{
    u32 NrPhys;
    int i;
    u32 j;
    u32 __attribute((unused)) pll = 0;

    u32 LanesPerCtrl[2][DRV_MIPI_NO_CTRL] =
    {
     // For each RX controller, what are the associated DPHYs
    {
      DRV_MIPI_C0_RX_LANE_CFG, DRV_MIPI_C1_RX_LANE_CFG, DRV_MIPI_C2_RX_LANE_CFG,
      DRV_MIPI_C3_RX_LANE_CFG, DRV_MIPI_C4_RX_LANE_CFG, DRV_MIPI_C5_RX_LANE_CFG
    },

    // For each TX controller, what are the associated DPHYs
    {
      DRV_MIPI_C0_TX_LANE_CFG, DRV_MIPI_C1_TX_LANE_CFG, DRV_MIPI_C2_TX_LANE_CFG,
      DRV_MIPI_C3_TX_LANE_CFG, DRV_MIPI_C4_TX_LANE_CFG, DRV_MIPI_C5_TX_LANE_CFG
    } };


    u32 PhysPerCtrl[2][DRV_MIPI_NO_CTRL] =
    {
     // For each Rx controller, what are the D-PHYs that can used
     {
        DRV_MIPI_C0_RX_DPHYS, DRV_MIPI_C1_RX_DPHYS, DRV_MIPI_C2_RX_DPHYS,
        DRV_MIPI_C3_RX_DPHYS, DRV_MIPI_C4_RX_DPHYS, DRV_MIPI_C5_RX_DPHYS
     },
     // For each Rx controller, what are the D-PHYs that can used
     {
        DRV_MIPI_C0_TX_DPHYS, DRV_MIPI_C1_TX_DPHYS, DRV_MIPI_C2_TX_DPHYS,
        DRV_MIPI_C3_TX_DPHYS, DRV_MIPI_C4_TX_DPHYS, DRV_MIPI_C5_TX_DPHYS
     } };

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wtype-limits"
    // Test if controller number is valid
    // assert((mipiCtrlCfg->controllerNo >= 0) && (mipiCtrlCfg->controllerNo < DRV_MIPI_NO_CTRL));
    if ((mipiCtrlCfg->controllerNo < 0) || (mipiCtrlCfg->controllerNo > (DRV_MIPI_NO_CTRL-1)))
    return DRV_MIPI_CFG_INVALID_CTRL;
    #pragma GCC diagnostic pop

    // Test that controller is set as rx or tx
    //assert((mipiCtrlCfg->direction == MIPI_RX) || (mipiCtrlCfg->direction == MIPI_TX));
    if ((mipiCtrlCfg->direction != MIPI_RX) && (mipiCtrlCfg->direction != MIPI_TX))
    return DRV_MIPI_CFG_INVALID_DIR;

    // Test if controller is CSI or DSI
    //assert((mipiCtrlCfg->type == MIPI_CSI) || (mipiCtrlCfg->type == MIPI_DSI));
    if ((mipiCtrlCfg->type != MIPI_CSI) && (mipiCtrlCfg->type != MIPI_DSI))
    return DRV_MIPI_CFG_INVALID_TYPE;

    // Test if controller is tx, see that tx is allowed on this controller.
    //assign((mipiCtrlCfg->direction == MIPI_TX) && (LanesPerCtrl[1][mipiCtrlCfg->controllerNo] != 0))
    if ((mipiCtrlCfg->direction == MIPI_TX) &&
        (LanesPerCtrl[1][mipiCtrlCfg->controllerNo] == 0))
    return DRV_MIPI_CFG_NO_TX;

    // Test that number of lanes selected corresponds with the controllers capabilities
    //assert(((1 << mipiCtrlCfg->noLanes) & LanesPerCtrl[mipiCtrlCfg->direction][mipiCtrlCfg->controllerNo]) != 0);
    if (((1 << mipiCtrlCfg->noLanes) & LanesPerCtrl[mipiCtrlCfg->direction][mipiCtrlCfg->controllerNo]) == 0)
    return DRV_MIPI_CFG_INVALID_LANE_NO;

    // Test if rate is in the boundary
    //assert((mipiCtrlCfg->laneRateMbps >= D_DRV_MIPI_CTRL_MIN_TRAN_RATE_MBPS) && (mipiCtrlCfg->laneRateMbps <= D_DRV_MIPI_CTRL_MAX_TRAN_RATE_MBPS));
    if ((mipiCtrlCfg->laneRateMbps < D_DRV_MIPI_CTRL_MIN_TRAN_RATE_MBPS) || (mipiCtrlCfg->laneRateMbps > D_DRV_MIPI_CTRL_MAX_TRAN_RATE_MBPS))
    return DRV_MIPI_CFG_INVALID_RATE;



    #pragma GCC diagnostic ignored "-Wtype-limits"
    // test data mode and data type
    if (mipiCtrlCfg->direction == MIPI_RX)
    {
        // Test if data type is within 6 bits limit
        if ((mipiCtrlCfg->rxPacket.dataType < 0) || (mipiCtrlCfg->rxPacket.dataType > D_MIPI_DFORM_MASK))
            return DRV_MIPI_CFG_INVALID_RX_DATA_TYPE;

         // Test if data mode is valid
        if ((mipiCtrlCfg->rxPacket.dataMode < 0) || (mipiCtrlCfg->rxPacket.dataMode > (D_MIPI_HS_CTRL_DATA_MODE_MASK >> D_MIPI_HS_CTRL_DATA_MODE_POS)))
            return DRV_MIPI_CFG_INVALID_RX_DATA_MODE;
    }
    else
    {
        for (i=0; i<DRV_MIPI_NO_TX_PACKETS ; ++i)
        {
            // test if data type is valid
            if ((mipiCtrlCfg->txPacket[i].dataType < 0) || (mipiCtrlCfg->txPacket[i].dataType > D_MIPI_DFORM_MASK))
                return DRV_MIPI_CFG_INVALID_TX_DATA_TYPE;

            // Test if data mode is valid
            if ((mipiCtrlCfg->txPacket[i].dataMode < 0) || (mipiCtrlCfg->txPacket[i].dataMode > (D_MIPI_HS_CTRL_DATA_MODE_MASK >> D_MIPI_HS_CTRL_DATA_MODE_POS)))
                return DRV_MIPI_CFG_INVALID_TX_DATA_MODE;
        }
    }
    #pragma GCC diagnostic pop



    DPRINTF1("DRV MIPI %s ctrl no %d\n  dir %d\n  type %d\n  lanes %d\n",__FUNCTION__, mipiCtrlCfg->controllerNo,
    mipiCtrlCfg->direction, mipiCtrlCfg->type, mipiCtrlCfg->noLanes);

    // =================================================================================================================================

    // Init D-PHYs
    NrPhys = (mipiCtrlCfg->noLanes & 1) + (mipiCtrlCfg->noLanes >> 1); // First term adds 1 PHY for 1 lane, second term adds phys for even number of lanes.

    DPRINTF2("\nDRV MIPI %s NrPhys %d\n",__FUNCTION__, NrPhys);


    // Special case for controller 5 in tx mode, (no need to disable CLK lane Term res, tx only)
    if ((mipiCtrlCfg->direction == MIPI_TX) && (mipiCtrlCfg->controllerNo == MIPI_CTRL_5))
    {
        for (i = DRV_MIPI_NO_DPHY-1,  j = 0 ; i>0 ; i--)
             if (( PhysPerCtrl[mipiCtrlCfg->direction][mipiCtrlCfg->controllerNo] & (u32)(1<<i)) == (u32)(1<<i))
             {
                  j++;
                  DPRINTF2("TX j:%d  i:%d\n",j,i);
                  // i is the D-PHY number.
                  DrvMipiTestClear(i);
                  DrvMipiTestModeSendCmd(i, D_MIPI_TCMD_NORMAL, NULL, NULL, 0); // Init D-PHYs used by the controller.
                  DrvMipiInitDphy(i);                                           // Changed place for D-PHYs init from after controller init, to here.
                  DrvMipiSetRxRange(i, mipiCtrlCfg->laneRateMbps * 1000);
                  if (j==NrPhys) break; // Cycled trough all the used PHYS.
             }
    }
    else // For all the others this will work
         for (i = 0,  j = 0 ; i<DRV_MIPI_NO_DPHY ; i++)
         {
             DPRINTF2("\nDRV MIPI %s i = %d\n",__FUNCTION__, i);
             if (( PhysPerCtrl[mipiCtrlCfg->direction][mipiCtrlCfg->controllerNo] & (u32)(1<<i)) == (u32)(1<<i))
             {
                  j++;
                  DPRINTF2("RX/TX j:%d  i:%d\n",j,i);
                  // i is the D-PHY number.
                  DrvMipiTestClear(i);
                  DrvMipiTestModeSendCmd(i, D_MIPI_TCMD_NORMAL, NULL, NULL, 0); // Init D-PHYs used by the controller.
                  DrvMipiInitDphy(i);                                           // Changed place for D-PHYs init from after controller init, to here.
                  DrvMipiSetRxRange(i, mipiCtrlCfg->laneRateMbps * 1000);

                  if ((j>1) && (mipiCtrlCfg->direction == MIPI_RX))
                  {
                       DPRINTF2(" Disable term res for clock on DPHY %d\n", i);
                       DrvMipiRxClkTermCtrl(i, DRV_MIPI_TERM_RES_DISABLE);       // Disable the TERM res for RX, on clk lane for all PHYs, except first PHY
                  }

                  if (j==NrPhys) break; // Cycled trough all the used PHYS.

             }
          }


    // Init the controller
    SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100,    D_MIPI_HS_CTRL_EN |
                                  (mipiCtrlCfg->direction           << D_MIPI_HS_CTRL_DIR_POS      ) |
                                  (mipiCtrlCfg->type                << D_MIPI_HS_CTRL_MODE_POS     ) |
                                  ((mipiCtrlCfg->noLanes - 1)       << D_MIPI_HS_CTRL_LANE_POS     ) |
                                  (mipiCtrlCfg->rxPacket.dataMode   << D_MIPI_HS_CTRL_DATA_MODE_POS) |  // In case of TX, this filed is not used from here
                                  (0x3F << D_MIPI_HS_CTRL_HS_EXIT_CNT_POS)                           |  // this last two parameters will be hard coded for now.
                                  (0x0F << D_MIPI_HS_CTRL_HS_CLK_IDLE_CNT_POS));

    // Stuff to set for RX
    if (mipiCtrlCfg->direction == MIPI_RX)
    {
          SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_RX_HS_FIFO_LVL_OFFSET  , 8);    // Hardcode Rx fifo level to 8

          // Set filter for the specific packet in Rx mode
         if (mipiCtrlCfg->rxPacket.dataType >= 0x20)
             SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_RX_HS_PH_FILTER1_OFFSET, 1 << (mipiCtrlCfg->rxPacket.dataType - 0x20));
         else
             SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_RX_HS_PH_FILTER0_OFFSET, 1 << mipiCtrlCfg->rxPacket.dataType);
    }
    else
    // Stuff to set for TX
    {
          SET_REG_WORD(MIPI_TX_ULPS_WKUP_CNT_ADDR, 16500);   // Hard code for now, it has to be 1ms on ecfg clk

          SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_HS_SYNC_CFG_OFFSET,
                                                 // D_MIPI_HS_SYNC_CFG_LINE_PKT_EN | // uncomment this line to have line start and end packets
                                                  D_MIPI_HS_SYNC_CFG_FR_CNT_EN |
                                                  D_MIPI_HS_SYNC_CFG_LINE_CNT );

          for (i = 0 ; i<DRV_MIPI_NO_TX_PACKETS ; i++)
          {
                    SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_TX_HS_SECT0_PH_OFFSET + i*8,
                         ((mipiCtrlCfg->txPacket[i].dataType << D_MIPI_TX_HS_PH_DATA_IDENTIFIER_POS) & D_MIPI_TX_HS_PH_DATA_IDENTIFIER_MASK) |
                         ((mipiCtrlCfg->txPacket[i].dataMode << D_MIPI_TX_HS_PH_DATA_MODE_POS)       & D_MIPI_TX_HS_PH_DATA_MODE_MASK      ) |
                         (mipiCtrlCfg->width * mipiCtrlCfg->txPacket[i].bitsPerPixel / 8));

                    SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100 + MIPI_TX_HS_SECT0_LINE_CNT_OFFSET + i*8,
                          mipiCtrlCfg->txPacket[i].noPackets );
          }

          // set PLL
          if (mipiCtrlCfg->controllerNo == MIPI_CTRL_3)
          {
                 pll = DrvMipiPllProg(DRV_MIPI_P2, mipiCtrlCfg->refClkKHz,  mipiCtrlCfg->laneRateMbps * 1000);
                 if (DrvMipiWaitPllLock(DRV_MIPI_P2, D_DRV_MIPI_PLL_LOCK_TIMEOUT) == 0)
                     return DRV_MIPI_CFG_PLL_NOT_LOCKED;
          }
          else
          if (mipiCtrlCfg->controllerNo == MIPI_CTRL_5)
          {
                 pll = DrvMipiPllProg(DRV_MIPI_P5, mipiCtrlCfg->refClkKHz,  mipiCtrlCfg->laneRateMbps * 1000);
                 if (DrvMipiWaitPllLock(DRV_MIPI_P5, D_DRV_MIPI_PLL_LOCK_TIMEOUT) == 0)
                     return DRV_MIPI_CFG_PLL_NOT_LOCKED;
          }

          if (mipiCtrlCfg->type == MIPI_DSI)
          {
                  //enable EoTP for DSI
                  SET_REG_WORD( MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100, D_MIPI_HS_CTRL_TX_DSI_EOTP_EN | GET_REG_WORD_VAL(MIPI0_HS_CTRL_ADDR + mipiCtrlCfg->controllerNo * 0x100));
          }

          DPRINTF2("TX %d PLL:%d\n",mipiCtrlCfg->controllerNo, pll); // PLL might not be set to the exact lane rate.
    }
    return DRV_MIPI_CFG_DONE;
}

#if (defined MYRIAD2)
void DrvResetMssGpioLcdMipiLoopbackCfgRegisterValues(void)
{
    SET_REG_WORD(MSS_MIPI_CFG_ADR,     0X00000000);    //DISABLE MIPI CHANNELS
    SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, 0X00000000);    //DISABLE MIPI->CIF CONNECTION
    SET_REG_WORD(MSS_LCD_MIPI_CFG_ADR, 0X00000000);    //DISABLE LCD->MIPI CONNECTION
    SET_REG_WORD(MSS_LOOPBACK_CFG_ADR, 0X00000000);    //LCD-CIF internal loopback not needed in this test-case.

    /* By default, the LCD and CIF modules are connected to MIPI.
     * So, in order to connect them to GPIOs, the MSSGPIOCfg register has to be set:
     * bit 0 = 1 => CIF connected to GPIO
     * bit 1 = 1 => LCD connected to GPIO */
    SET_REG_WORD(MSS_GPIO_CFG_ADR, 0x03);
    return;
}
#endif

/// @}



