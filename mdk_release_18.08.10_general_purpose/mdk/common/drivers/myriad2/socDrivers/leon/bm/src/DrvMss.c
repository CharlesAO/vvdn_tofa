///
/// @file DrvMss.c
/// 
/// 
/// @ingroup DrvMss
/// @{
/// @brief MIPI Driver Implementation
///

#include <DrvRegUtils.h>
#include "registersMyriad.h"
#include "mv_types.h"
#include "assert.h"
#include <stdio.h>

#include "DrvMss.h"

//#define DRV_MSS_DEBUG1
//#define DRV_MSS_DEBUG2

#ifdef DRV_MSS_DEBUG1
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

#ifdef DRV_MSS_DEBUG2
#define DPRINTF2(...) printf(__VA_ARGS__)
#else
#define DPRINTF2(...)
#endif


void DrvMssResetAllMipiConnections()
{
    SET_REG_WORD(MSS_MIPI_CFG_ADR,      0);
    SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR,  0);  
    SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, 0);
    SET_REG_WORD(MSS_LCD_MIPI_CFG_ADR,  0);
    SET_REG_WORD(MSS_SIPP_MIPI_CFG_ADR, 0);
    SET_REG_WORD(MSS_GPIO_CFG_ADR,      0); // GPIOs not connected to LCD and CIF
}


int DrvMssConnectMipiToDevice(u32 MipiCtrlNo, drvMssDeviceType device)
{
     u32 result, reg;
     result = 1;  // Presume connection is possible 
     // Set ctrl as Rx if connected to CIF or SIPP Rx
     if ((device == DRV_MSS_CIF) || (device == DRV_MSS_SIPP_RX))
     {
        DPRINTF1("<DrvMss> Connected MIPI CTRL%d to ", MipiCtrlNo); //not finalized printf
        switch (MipiCtrlNo)
        {
        case 0: // can connect to CIF0 or RX0
        
                // enable ctrl 0 as RX
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C0_RX | D_DRV_MSS_MIPI_CFG_EN_0 | reg);        
                
                if (device == DRV_MSS_CIF) // connect ctrl 0 to CIF 0
                {
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, D_DRV_MSS_MIPI_RX0_TO_CIF0 | (reg & (~(D_DRV_MSS_MIPI_RX0_TO_CIF0 | D_DRV_MSS_MIPI_RX1_TO_CIF0))));

                   // make sure SIPP 0 Rx is not connected to ctrl 0 
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX0_TO_SIPP0));

                   DPRINTF1("CIF0\n");
                }
                else // connect to SIPP RX 0 
                {
                   // make sure is not connected to CIF0 
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX0_TO_CIF0));                  
                   
                   // connect to SIPP RX 0, make sure it's the only one connected to SIPP 0
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX0_TO_SIPP0 | (reg & (~(D_DRV_MSS_MIPI_RX0_TO_SIPP0 | D_DRV_MSS_MIPI_RX1_TO_SIPP0))));
                   DPRINTF1("SIPP RX0\n");
                }
                break;
                
                
        case 1: 
                // enable ctrl 1 as RX
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C1_RX | D_DRV_MSS_MIPI_CFG_EN_1 | reg);        
                
                if (device == DRV_MSS_CIF) // connect ctrl 1 to CIF 0
                {
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, D_DRV_MSS_MIPI_RX1_TO_CIF0 | (reg & (~(D_DRV_MSS_MIPI_RX0_TO_CIF0 | D_DRV_MSS_MIPI_RX1_TO_CIF0))));                   
                   
                   // make sure SIPP 0 Rx is not connected to MIPI ctrl 1
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX1_TO_SIPP0));
                   DPRINTF1("CIF0\n");
                }
                else // connect to SIPP RX 0 
                {
                   // make sure is not connected to CIF0 
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX1_TO_CIF0));                  
                   
                   // connect to SIPP RX 0, make sure it's the only one connected to SIPP 0
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX1_TO_SIPP0 | (reg & (~(D_DRV_MSS_MIPI_RX0_TO_SIPP0 | D_DRV_MSS_MIPI_RX1_TO_SIPP0))));
                   DPRINTF1("SIPP RX0\n");
                }        
                break;
                
        case 2:                 
                if (device == DRV_MSS_CIF) 
                {
                    result = 0; //connection not possible
                    DPRINTF1("nothing (connection not possible)\n");
                }
                else // connect to SIPP RX 1
                {
                    // enable ctrl 2 as RX
                    reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                    SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C2_RX | D_DRV_MSS_MIPI_CFG_EN_2 | reg);        

                    reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX2_TO_SIPP1 | reg);
                    DPRINTF1("SIPP RX1\n");
                }                        
                break;
                
                
        case 3: 
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C3_RX | D_DRV_MSS_MIPI_CFG_EN_3 | (reg & (~D_DRV_MSS_MIPI_CFG_C3_TX)));           
        
                if (device == DRV_MSS_CIF) 
                {
                    reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, D_DRV_MSS_MIPI_RX3_TO_CIF1 | (reg & (~(D_DRV_MSS_MIPI_RX3_TO_CIF1 | D_DRV_MSS_MIPI_RX5_TO_CIF1))));                   
                   
                    reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX3_TO_SIPP2));                   

                    reg = GET_REG_WORD_VAL(MSS_GPIO_CFG_ADR);
                    SET_REG_WORD(MSS_GPIO_CFG_ADR, D_DRV_MSS_CIF_TO_MIPI | reg); //only for CIF1
                    DPRINTF1("CIF1\n");
                }
                else 
                {
                   // make sure is not connected to CIF1 
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX3_TO_CIF1));                  
                   
                   // connect to SIPP RX 2, make sure it's the only one connected to SIPP 2
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX3_TO_SIPP2 | (reg & (~(D_DRV_MSS_MIPI_RX3_TO_SIPP2 | D_DRV_MSS_MIPI_RX5_TO_SIPP2))));
                   DPRINTF1("SIPP RX2\n");
                }
                break;
                
                
        case 4: 
                if (device == DRV_MSS_CIF) 
                {
                    result = 0; //connection not possible
                    DPRINTF1("nothing (connection not possible)\n");
                }
                else 
                {
                    // enable ctrl 4 as RX
                    reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                    SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C4_RX | D_DRV_MSS_MIPI_CFG_EN_4 | reg);        

                    reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX4_TO_SIPP3 | reg);                     
                    DPRINTF1("SIPP RX3\n");
                }        
                break;
                
                
        case 5: 
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C5_RX | D_DRV_MSS_MIPI_CFG_EN_5 | (reg & (~D_DRV_MSS_MIPI_CFG_C5_TX)));                
        
                if (device == DRV_MSS_CIF) 
                {
                    
                    reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, D_DRV_MSS_MIPI_RX5_TO_CIF1 | (reg & (~(D_DRV_MSS_MIPI_RX3_TO_CIF1 | D_DRV_MSS_MIPI_RX5_TO_CIF1))));                   
                   
                    reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                    SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX5_TO_SIPP2));
                    DPRINTF1("CIF1\n");
        
                }
                else 
                {
                   // make sure is not connected to CIF1 
                   reg = GET_REG_WORD_VAL(MSS_MIPI_CIF_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_CIF_CFG_ADR, reg & (~D_DRV_MSS_MIPI_RX5_TO_CIF1));                  
                   
                   // connect to SIPP RX 2, make sure it's the only one connected to SIPP 2
                   reg = GET_REG_WORD_VAL(MSS_MIPI_SIPP_CFG_ADR);                   
                   SET_REG_WORD(MSS_MIPI_SIPP_CFG_ADR, D_DRV_MSS_MIPI_RX5_TO_SIPP2 | (reg & (~(D_DRV_MSS_MIPI_RX3_TO_SIPP2 | D_DRV_MSS_MIPI_RX5_TO_SIPP2))));
                   DPRINTF1("SIPP RX2\n");
                }        
                break;
        default :
            result = 0;        
        }   
        
        DPRINTF2("<DrvMss> MSS_MIPI_SIPP_CFG_ADR = 0x%x\n", *(u32*)MSS_MIPI_SIPP_CFG_ADR);
        DPRINTF2("<DrvMss> MSS_MIPI_CIF_CFG_ADR  = 0x%x\n", *(u32*)MSS_MIPI_CIF_CFG_ADR);

     }
     else
     // Set device as Tx if connected to LCD or SIPP Tx
     if ((device == DRV_MSS_LCD) || (device == DRV_MSS_SIPP_TX))
     {
          switch (MipiCtrlNo)     
          {
          case 3:
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C3_TX | D_DRV_MSS_MIPI_CFG_EN_3 | reg);        
               
                if (device == DRV_MSS_LCD) 
                {
                       SET_REG_WORD(MSS_LCD_MIPI_CFG_ADR, D_DRV_MSS_LCD_TO_MIPI_TX3);

                       reg = GET_REG_WORD_VAL(MSS_GPIO_CFG_ADR);                       
                       SET_REG_WORD(MSS_GPIO_CFG_ADR, reg & (~D_DRV_MSS_LCD_TO_MIPI));
                       DPRINTF1("LCD\n");
                }
                else 
                {
                       reg  = GET_REG_WORD_VAL(MSS_SIPP_MIPI_CFG_ADR);
                       SET_REG_WORD(MSS_SIPP_MIPI_CFG_ADR, reg | D_DRV_MSS_SIPP0_TO_MIPI_TX3);
                       DPRINTF1("SIPP TX0\n");
                }        
 
                break;          
          
          case 5:
                reg = GET_REG_WORD_VAL(MSS_MIPI_CFG_ADR);
                SET_REG_WORD(MSS_MIPI_CFG_ADR,     D_DRV_MSS_MIPI_CFG_C5_TX | D_DRV_MSS_MIPI_CFG_EN_5 | reg);        
               
                if (device == DRV_MSS_LCD) 
                {
                       SET_REG_WORD(MSS_LCD_MIPI_CFG_ADR, D_DRV_MSS_LCD_TO_MIPI_TX5);

                       reg = GET_REG_WORD_VAL(MSS_GPIO_CFG_ADR);                       
                       SET_REG_WORD(MSS_GPIO_CFG_ADR, reg & (~D_DRV_MSS_LCD_TO_MIPI));
                       DPRINTF1("LCD\n");
                }
                else 
                {
                       reg  = GET_REG_WORD_VAL(MSS_SIPP_MIPI_CFG_ADR);
                       SET_REG_WORD(MSS_SIPP_MIPI_CFG_ADR, reg | D_DRV_MSS_SIPP1_TO_MIPI_TX5);
                       DPRINTF1("SIPP TX1\n");
                }        
 
                break;          
          
          default:  
              result =0; break;
          }

          DPRINTF2("<DrvMss> MSS_SIPP_MIPI_CFG_ADR = 0x%x\n", *(u32*)MSS_SIPP_MIPI_CFG_ADR);
          DPRINTF2("<DrvMss> MSS_LCD_MIPI_CFG_ADR  = 0x%x\n", *(u32*)MSS_LCD_MIPI_CFG_ADR);
     }    
     
     DPRINTF2("<DrvMss> MSS_GPIO_CFG_ADR      = 0x%x\n", *(u32*)MSS_GPIO_CFG_ADR);
     DPRINTF2("<DrvMss> MSS_MIPI_CFG_ADR      = 0x%x\n", *(u32*)MSS_MIPI_CFG_ADR);

     return result;     
}

void DrvMssConnectCif1ToGpio(void)
{
    u32 reg;

    reg  = GET_REG_WORD_VAL(MSS_GPIO_CFG_ADR);
    SET_REG_WORD(MSS_GPIO_CFG_ADR, D_DRV_MSS_CIF_TO_GPIO | reg);  //Enable CIF1->GPIO connection

    DPRINTF1("<DrvMss> Connected CIF1 to GPIO\n");
}

void DrvMssConnectLcdToGpio(void)
{
    u32 reg;

    reg  = GET_REG_WORD_VAL(MSS_GPIO_CFG_ADR);
    SET_REG_WORD(MSS_GPIO_CFG_ADR, D_DRV_MSS_LCD_TO_GPIO | reg);  //Enable LCD->GPIO connection

    DPRINTF1("<DrvMss> Connected LCD to GPIO\n");
}

/// @}


