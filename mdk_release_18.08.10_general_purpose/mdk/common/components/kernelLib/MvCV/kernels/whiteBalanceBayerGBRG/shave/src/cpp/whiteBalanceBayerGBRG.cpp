 #include "whiteBalanceBayerGBRG.h"

 void mvcvWhiteBalanceBayerGBRG(u16** inGBRG, u16 awbCoef[3], u16** outGBRG, u16 clamp[1], u32 width, u8 line)
{
	int i;
	u32 o1, o2, o3, o4;
	
	for (i = 0; i < (int)width; i+=2) {
		if (line % 2 == 0) { //line with GBGBGB...pixels
		o1 = ((u32)inGBRG[0][0+i] * (u32)awbCoef[1]) >> 15; //Gb
		o2 = ((u32)inGBRG[0][1+i] * (u32)awbCoef[2]) >> 15; //G
			outGBRG[0][0+i] = (u16)(o1 > clamp[0] ? clamp[0] : o1);
			outGBRG[0][1+i] = (u16)(o2 > clamp[0] ? clamp[0] : o2);
		}
			else { //line with RGRGRG...pixels
			o3 = ((u32)inGBRG[0][0+i] * (u32)awbCoef[0]) >> 15; //R
			o4 = ((u32)inGBRG[0][1+i] * (u32)awbCoef[1]) >> 15; //Gr
				outGBRG[0][0+i] = (u16)(o3 > clamp[0] ? clamp[0] : o3);
				outGBRG[0][1+i] = (u16)(o4 > clamp[0] ? clamp[0] : o4);
				}
	}
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvWhiteBalanceBayerGBRG_opt(unsigned short ** inGBRG, unsigned short awbCoef[3], unsigned short ** outGBRG, 
                                               unsigned short clamp[1], unsigned int width, unsigned char line) {
  int i;
  unsigned int o1, o2, o3, o4;

  for (i = 0; i < (int)width; i+=2) {
    if (line % 2 == 0) { //line with GBGBGB...pixels
      o1 = ((unsigned int)inGBRG[0][0+i] * (unsigned int)awbCoef[1]) >> 15; //Gb
      o2 = ((unsigned int)inGBRG[0][1+i] * (unsigned int)awbCoef[2]) >> 15; //G
      outGBRG[0][0+i] = (unsigned short)(o1 > clamp[0] ? clamp[0] : o1);
      outGBRG[0][1+i] = (unsigned short)(o2 > clamp[0] ? clamp[0] : o2);
    }
    else { //line with RGRGRG...pixels
      o3 = ((unsigned int)inGBRG[0][0+i] * (unsigned int)awbCoef[0]) >> 15; //R
      o4 = ((unsigned int)inGBRG[0][1+i] * (unsigned int)awbCoef[1]) >> 15; //Gr
      outGBRG[0][0+i] = (unsigned short)(o3 > clamp[0] ? clamp[0] : o3);
      outGBRG[0][1+i] = (unsigned short)(o4 > clamp[0] ? clamp[0] : o4);
    }
  }
}
#endif
