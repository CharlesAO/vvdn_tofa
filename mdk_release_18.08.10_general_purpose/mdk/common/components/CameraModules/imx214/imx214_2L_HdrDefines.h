#ifndef _IMX214_2L_HDR_DEFINES_H_
#define _IMX214_2L_HDR_DEFINES_H_


#include "DrvI2cMaster.h"
#include "swcLeonUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

// Hdr mode register addresses

#define HDR_REGADDR_ST_ANA_GAIN_GLOBAL_8   0x0216 	//Analog gain register addresses
#define HDR_REGADDR_ST_ANA_GAIN_GLOBAL 	   0x0217	
#define HDR_REGADDR_MODE		   0x0220	// Hdr mode register address
#define HDR_REGADDR_RESO_REDU		   0x0221	// first 4 bits of register are for row scaling 							//	factor, last 4 are for column scaling factor
#define HDR_REGADDR_EXPO_RATIO		   0x0222	// Exposure ratio register address
#define HDR_REGADDR_ST_COARSE_INTEG_TIME_H 0x0224	// Coarse integration time register address
#define HDR_REGADDR_ST_COARSE_INTEG_TIME_L 0x0225

// Available hdr register values
#define HDR_ENABLE 		0x01	// HDR_MODE register values
#define HDR_COMBINED_GAIN 	0x00
#define HDR_SEPARATE_GAIN 	0x02
#define HDR_EXPOSURE_BY_RATIO 	0x00
#define HDR_EXPOSURE_DIRECT 	0x10


					// HDR resolutin reduction register values
#define HDR_COLUMN_FULL 	0x0	//full HDR
#define HDR_COLUMN_BINNING 	0x02	//binning HDR
#define HDR_ROW_FULL 		0x10
#define HDR_ROW_BINNING 	0x20



typedef struct {
	u8 HDR_CAPABILITIES;
	u8 MIN_HDR_BIT_DEPTH;
	u8 HDR_RESO_SUBTYPE;
	u8 HDR_RESO_TYPE[8];

}tyImx214_2L_HdrCapabilities;




typedef struct {

	u16 ST_ANA_GLOBAL;   //Analog gain 
	u8 HDR_MODE;	     
	u8 HDR_RESO_REDU;	
	u8 EXPO_RATIO;	     //Exposure ratio
	u16 ST_COARSE_INTEG_TIME;	//Coarse integration time

}tyImx214_2L_HdrSettings;

void setHdrMode(I2CM_Device * dev,u8 i2cAddr,u8 * protocol,tyImx214_2L_HdrSettings HdrSetting);
tyImx214_2L_HdrSettings getHdrMode(I2CM_Device * dev,u8 i2cAddr,u8 * protocol);

tyImx214_2L_HdrCapabilities getHdrCapabilities(I2CM_Device * dev,u8 i2cAddr,u8 * protocol);


void printHdrCababilities(tyImx214_2L_HdrCapabilities data);
void printHdrSettings(tyImx214_2L_HdrSettings HdrSetting);

#ifdef __cplusplus
}
#endif

#endif //_IMX214_2L_HDR_DEFINES_H_
