

#include "imx214_2L_HdrDefines.h"
#include "DrvI2cMaster.h"
#include "swcLeonUtils.h"
#include <stdio.h>


void setHdrMode(I2CM_Device * dev,u8 i2cAddr,u8 * protocol,tyImx214_2L_HdrSettings HdrSetting){

	u8 temp;


	temp= HdrSetting.ST_ANA_GLOBAL >> 8;
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_ANA_GAIN_GLOBAL_8  ,protocol,(u8*)&temp,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_ANA_GAIN_GLOBAL    ,protocol,(u8*)&HdrSetting.ST_ANA_GLOBAL,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_MODE					 ,protocol,(u8*)&HdrSetting.HDR_MODE,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_RESO_REDU			 ,protocol,(u8*)&HdrSetting.HDR_RESO_REDU,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_EXPO_RATIO			 ,protocol,(u8*)&HdrSetting.EXPO_RATIO,1);

	temp= HdrSetting.ST_COARSE_INTEG_TIME >> 8;
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_COARSE_INTEG_TIME_H,protocol,(u8*)&temp,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_COARSE_INTEG_TIME_L,protocol,(u8*)&HdrSetting.ST_COARSE_INTEG_TIME,1);

}

tyImx214_2L_HdrSettings getHdrMode(I2CM_Device * dev,u8 i2cAddr,u8 * protocol){

	tyImx214_2L_HdrSettings HdrSettingTemp;
	u8 temp;

	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_ANA_GAIN_GLOBAL_8  ,protocol,(u8*)&HdrSettingTemp.ST_ANA_GLOBAL,1);
	HdrSettingTemp.ST_ANA_GLOBAL <<= 8;
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_ANA_GAIN_GLOBAL    ,protocol,(u8*)&temp,1);
	HdrSettingTemp.ST_ANA_GLOBAL |= temp;

	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_MODE			   	     ,protocol,(u8*)&HdrSettingTemp.HDR_MODE,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_RESO_REDU		  	 ,protocol,(u8*)&HdrSettingTemp.HDR_RESO_REDU,1);
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_EXPO_RATIO	  		 ,protocol,(u8*)&HdrSettingTemp.EXPO_RATIO,1);

	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_COARSE_INTEG_TIME_H,protocol,(u8*)&HdrSettingTemp.ST_COARSE_INTEG_TIME,1);
	HdrSettingTemp.ST_COARSE_INTEG_TIME <<= 8;
	DrvI2cMTransaction(dev,i2cAddr,HDR_REGADDR_ST_COARSE_INTEG_TIME_L,protocol,(u8*)&temp,1);
	HdrSettingTemp.ST_COARSE_INTEG_TIME |=  temp;

	return HdrSettingTemp;
}

void printHdrCababilities(tyImx214_2L_HdrCapabilities data){
	int i;
	printf("HDR_CAPA: 0x%x\n",data.HDR_CAPABILITIES);
	printf("HDR_BIT_DEPTH: 0x%x\n",data.MIN_HDR_BIT_DEPTH);
	printf("HDR_RESO_SUBTYPE: 0x%x\n",data.HDR_RESO_SUBTYPE);

	for(i=0;i<8;i++){

		printf("HDR_RESO_TYPE_%d: 0x%x\n",i+1,data.HDR_RESO_TYPE[i]);
	}

}

tyImx214_2L_HdrCapabilities getHdrCapabilities(I2CM_Device * dev,u8 i2cAddr,u8 * protocol){

	tyImx214_2L_HdrCapabilities data;
	u16 i;
	DrvI2cMTransaction(dev, i2cAddr,0x1210,protocol, (u8*)&data.HDR_CAPABILITIES,1);
	DrvI2cMTransaction(dev, i2cAddr,0x1211,protocol, (u8*)&data.MIN_HDR_BIT_DEPTH,1);
	DrvI2cMTransaction(dev, i2cAddr,0x1212,protocol, (u8*)&data.HDR_RESO_SUBTYPE,1);

	for(i=0;i<8;i++){
		DrvI2cMTransaction(dev, i2cAddr,0x1213+i,protocol, (u8*)&data.HDR_RESO_TYPE[i],1);

	}
	return data;

}

void printHdrSettings(tyImx214_2L_HdrSettings HdrSetting){

	printf("\nHDR settings : \nST_ANA %x \nMODE %x \nRESO_REDU %x\nEXPO %x \nCOARSE %x\n",HdrSetting.ST_ANA_GLOBAL,HdrSetting.HDR_MODE,HdrSetting.HDR_RESO_REDU,HdrSetting.EXPO_RATIO,HdrSetting.ST_COARSE_INTEG_TIME);


}
