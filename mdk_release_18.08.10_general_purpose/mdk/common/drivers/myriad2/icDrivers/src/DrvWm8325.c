

#include "mv_types.h"
#include "DrvWm8325.h"
#include "stdio.h"



// in mili Volt
#define DRV_WM8325_LDO1_6_START_VAL   (900)
#define DRV_WM8325_LDO1_6_MIDDLE_VAL  (1700)
#define DRV_WM8325_LDO1_6_END_VAL     (3300)

#define DRV_WM8325_LDO7_10_START_VAL  (1000)
#define DRV_WM8325_LDO7_10_MIDDLE_VAL (1700)
#define DRV_WM8325_LDO7_10_END_VAL    (3500)

// in micro Volt
#define DRV_WM8325_DCDC1_2_START_VAL  (600000)
#define DRV_WM8325_DCDC1_2_END_VAL    (1800000)

#define DRV_WM8325_DCDC3_4_START_VAL   (850000)
#define DRV_WM8325_DCDC3_4_END_VAL     (3400000)

#define DEBUG_MSG_EN

#ifdef DEBUG_MSG_EN
#define	DEBUG_MSG(x) printf(x)
#else
#define	DEBUG_MSG(x)
#endif

// use LDO 1 to 10

// 50mV steps to 1.7V
// 100mV steps to 3.3V / 3.5V
void DrvWm8325SetLdoVoltage(u8 Ldo, u32 voltageInMiliVolt, tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
     u32 oldVal, bitField, write, data;
     bitField = 0;

     // usual checks:
     //should not set inexistent LDOs
     if ((Ldo < 1)  || (Ldo > 10))
       return;
     
     write = 0;
     
     // over and under voltage for LDO 1 to 6
     if (Ldo <= 6) {
         if ((voltageInMiliVolt > DRV_WM8325_LDO1_6_END_VAL) || (voltageInMiliVolt < DRV_WM8325_LDO1_6_START_VAL))
              return;
         else
         {
                write = 1;
                if (voltageInMiliVolt < DRV_WM8325_LDO1_6_MIDDLE_VAL)
                    bitField = (voltageInMiliVolt - DRV_WM8325_LDO1_6_START_VAL)/50;
                else
                    bitField = ((voltageInMiliVolt - DRV_WM8325_LDO1_6_MIDDLE_VAL)/100) + 15;
         }
     }

     // over and under voltage for LDO 7 to 10
     if (Ldo >  6) {
         if ((voltageInMiliVolt > DRV_WM8325_LDO7_10_END_VAL) || (voltageInMiliVolt < DRV_WM8325_LDO7_10_START_VAL))
              return;
         else     
         {
                write = 1;
                if (voltageInMiliVolt < DRV_WM8325_LDO7_10_MIDDLE_VAL)
                    bitField = (voltageInMiliVolt - DRV_WM8325_LDO7_10_START_VAL)/50;
                else
                    bitField = ((voltageInMiliVolt - DRV_WM8325_LDO7_10_MIDDLE_VAL)/100) + 13;
         }
     }
     
     if (write == 1)
     {
           oldVal = readFunc(WM8325_LDO1_ON_CONTROL +  (Ldo - 1)*3);
           data = (oldVal & ~(WM8325_LDO1_ON_VSEL_MASK)) | (WM8325_LDO1_ON_VSEL_MASK & bitField);
           writeFunc(WM8325_LDO1_ON_CONTROL +  (Ldo - 1)*3, data);
     }
}

// return: s32 voltageInMiliVolt
s32 DrvWm8325GetLdoVoltage(u8 Ldo, tDrvPmicI2cRead readFunc)
{
    s32 voltageInMiliVolt = 0;
    u32 bitField, regData;

    // usual checks:
    //should not set inexistent LDOs
    if ((Ldo < 1)  || (Ldo > 10))
        return -1;

    regData = readFunc(WM8325_LDO1_ON_CONTROL + (Ldo - 1) * 3);
    bitField = regData & WM8325_LDO1_ON_VSEL_MASK;

    if (Ldo <= 6)
    {
        if (bitField < (DRV_WM8325_LDO1_6_MIDDLE_VAL - DRV_WM8325_LDO1_6_START_VAL) / 50)
            voltageInMiliVolt = bitField * 50 + DRV_WM8325_LDO1_6_START_VAL;
        else
            voltageInMiliVolt = (bitField - 15) * 100 + DRV_WM8325_LDO1_6_MIDDLE_VAL;
    }
    else
    {
        if (bitField < (DRV_WM8325_LDO7_10_MIDDLE_VAL - DRV_WM8325_LDO7_10_START_VAL) / 50)
            voltageInMiliVolt = bitField * 50 + DRV_WM8325_LDO7_10_START_VAL;
        else
            voltageInMiliVolt = (bitField - 13) * 100 + DRV_WM8325_LDO7_10_MIDDLE_VAL;
    }

    return voltageInMiliVolt;
}



//For DC DC 1 & 2  :     12.5mV steps from 0.6V to 1.8V
//For DC DC 3 & 4  :     25mv steps from 0.850V to 3.4V
// voltage in microvolts, to solve the 0.5mV issue
void DrvWm8325SetDcDcVoltage(u8 Dc, u32 voltageInMicroVolt, tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
     u32 oldVal, bitField, write, data;
     bitField = 0;
     // checks 
     // right number for the DC DC
     if ((Dc<1) || (Dc>4))
        return;
        
        
     if ((Dc>=1) && (Dc<=2)) {
        // check the voltage range and if OK calculate the bit field
        if ((voltageInMicroVolt < DRV_WM8325_DCDC1_2_START_VAL)  ||  (voltageInMicroVolt > DRV_WM8325_DCDC1_2_END_VAL))
            return;
        else
        {
            write = 1;
            bitField = ((voltageInMicroVolt - DRV_WM8325_DCDC1_2_START_VAL) / 12500) + 8;
        }
     }
     
     if ((Dc>=3) && (Dc<=4)) {
        // check the voltage range and if OK calculate the bit field
        if ((voltageInMicroVolt < DRV_WM8325_DCDC3_4_START_VAL)  ||  (voltageInMicroVolt > DRV_WM8325_DCDC3_4_END_VAL))
            return;
        else
        {
            write = 1;
            bitField = ((voltageInMicroVolt - DRV_WM8325_DCDC3_4_START_VAL) / 25000);
        }
     }

        if (write == 1)
        {
            oldVal = readFunc(WM8325_DC1_ON_CONFIG +  (Dc - 1)*5 - (Dc==4?1:0));
            data = (oldVal & ~(0x7F)) | (0x7F & bitField);
            writeFunc(WM8325_DC1_ON_CONFIG +  (Dc - 1)*5 - (Dc==4?1:0),  data);
        }
}

// gpio is from 1 to 12
// use defines in DrvWm8325Defines.h to set value 
// WM8325_GPIO_NORMAL_IN_MODE
// WM8325_GPIO_NORMAL_OUT_MODE


void DrvWm8325SetGpioMode(u8 gpio, u16 value,  tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
    UNUSED(readFunc); // hush the compiler warning.
     if ((gpio>0) && (gpio<13))
     {
         // overwrite is deliberate 
         writeFunc(WM8325_GPIO1_CONTROL +  (gpio - 1),  value);
     }
}


// level is 0 or 1
// gpio is from 1 to 12
void DrvWm8325SetGpioLevel(u8 gpio, u8 level,  tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
     u32 oldVal, value;
     
     if ((gpio>0) && (gpio<13))
     {
         oldVal = readFunc(WM8325_GPIO_LEVEL);
         value = (~(1<<(gpio-1)) & oldVal) | ((level&1)<<(gpio-1)); // delete the old value and add the new one 
         writeFunc(WM8325_GPIO_LEVEL,  value);
     }
}


// gpio is from 1 to 12
u8 DrvWm8325GetGpioLevel(u8 gpio, tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
    UNUSED(writeFunc); // hush the compiler warning.
     u32 Val;
     Val = 0;
     if ((gpio>0) && (gpio<13))
     {
         Val = readFunc(WM8325_GPIO_LEVEL);
         Val &= 1<<(gpio-1);
         Val >>=gpio-1;
     }
     return Val;
}

// Read PMIC internal temperature using built-in AUXADC converter
float DrvWm8325GetPmicTemp(tDrvPmicI2cRead readFunc,  tDrvPmicI2cWrite writeFunc)
{
   u32 tempIntStat;	
   u32 AuxADCData;
   u8  AuxADCData_src;
   float PmicTemp;

   // configure AuxADC source to AUX_CHIP_TEMP_SEL
   writeFunc(WM8325_AUXADC_SOURCE, WM8325_AUX_CHIP_TEMP_SEL);

   // configure Interrupt Status 1 Mask register. Unmask AUXADC_DATA_EINT interrupt.
   writeFunc(WM8325_INTERRUPT_STATUS_1_MASK, !WM8325_IM_AUXADC_DATA_EINT);

   // configure AuxADC Control. Enable AuxADC converter.
   writeFunc(WM8325_AUXADC_CONTROL, WM8325_AUX_ENA);

   // configure AuxADC Control. Trigger an AuxADC conversion on AUX_CHIP_TEMP channel.
   writeFunc(WM8325_AUXADC_CONTROL, WM8325_AUX_ENA | WM8325_AUX_CVT_ENA);
  
   // poll the status of the AUXADC_DATA_EINT interrupt
   do
   { 
	tempIntStat = readFunc(WM8325_INTERRUPT_STATUS_1);
	tempIntStat &= 0x100; // extract bit8 - (AUXADC_DATA_EINT) from WM8325_INTERRUPT_STATUS_1 register
		
   } while(!tempIntStat);  
   
   // read the data returned by AuxADC converter
   AuxADCData = readFunc(WM8325_AUXADC_DATA);

   // check if the AuxADC source is the correct one - AUX_CHIP_TEMP.

   AuxADCData_src = (AuxADCData & 0xF000) >> 12;
  

   if( AuxADCData_src != 5 ) {

	return 0.00;
    }

    PmicTemp = (498 - (AuxADCData & 0xFFF)) / 1.09; // formula from WM8325 datasheet

    return PmicTemp;
}

