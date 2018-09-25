//nonMax3x3_fp32 kernel test

//Asm function prototype:
//	void mvcvNonMax3x3_fp32_asm(u16* candPos, u32 candCount, fp32* corners1, fp32* corners2,
//								fp32* corners3, u16* candOutPos, u32 *candOutCount)

//Asm test function prototype:
//	void mvcvNonMax3x3_fp32_asm_test(u16* candPos, u32 candCount, fp32* corners1, fp32* corners2,
//									fp32* corners3, u16* candOutPos, u32 *candOutCount)

//C function prototype:
//	void mvcvNonMax3x3_fp32(u16* candPos, u32 candCount, fp32* corners1, fp32* corners2,
//							fp32* corners3, u16* candOutPos, u32 *candOutCount)

#include "gtest/gtest.h"
#include "nonMax3x3_fp32_asm_test.h"
#include "nonMax3x3_fp32.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <float.h>


class NonMax3x3_fp32Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u32 width;
	u16* candPos;
	u32 candCount;
	fp32* corners1;
	fp32* corners2;
	fp32* corners3;

	u32 candOutCount_C;
	u32 candOutCount_Asm;
	u16* candOutPos_C;
	u16* candOutPos_Asm;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(NonMax3x3_fp32Test, TestDummy)
{
	width = 32;
	candCount = 20;

	candOutPos_C = 0;
	candOutPos_Asm = 0;

	inputGen.SelectGenerator("random");
	candPos = inputGen.GetLineU16(candCount, 1, width-1);

	corners1 = inputGen.GetLineFloat(width, 0, 25);
	corners2 = inputGen.GetLineFloat(width, 0, 50);
	corners3 = inputGen.GetLineFloat(width, 0, 25);


corners1[0]  = 10.026761  ;     	corners2[0]  = 44.486603 ;    corners3[0]  = 22.241936 ;
corners1[1]  = 18.020969  ;     	corners2[1]  = 22.355558 ;    corners3[1]  = 11.422438 ;
corners1[2]  = 16.668379  ;     	corners2[2]  = 40.910557 ;    corners3[2]  =  5.168005 ;
corners1[3]  = 13.425743  ;     	corners2[3]  = 12.967913 ;    corners3[3]  =  9.354506 ;
corners1[4]  =  9.240654  ;     	corners2[4]  = 24.225140 ;    corners3[4]  =  7.925204 ;
corners1[5]  =  0.934791  ;     	corners2[5]  = 43.005535 ;    corners3[5]  =  3.645235 ;
corners1[6]  =  1.047489  ;     	corners2[6]  = 46.954460 ;    corners3[6]  = 16.447220 ;
corners1[7]  = 16.993273  ;     	corners2[7]  = 14.185426 ;    corners3[7]  =  1.221229 ;
corners1[8]  = 19.980143  ;     	corners2[8]  = 36.592052 ;    corners3[8]  = 23.894043 ;
corners1[9]  = 21.793259  ;     	corners2[9]  = 40.497620 ;    corners3[9]  =  9.049639 ;
corners1[10] =  21.771580 ;     	corners2[10] =  35.204838;    corners3[10] =   2.120312;
corners1[11] =  10.509706 ;     	corners2[11] =   1.798165;    corners3[11] =  16.949106;
corners1[12] =   7.603057 ;     	corners2[12] =  36.110123;    corners3[12] =  17.835901;
corners1[13] =  22.806253 ;     	corners2[13] =  17.572529;    corners3[13] =  12.701680;
corners1[14] =  16.183846 ;     	corners2[14] =  21.162737;    corners3[14] =   7.091782;
corners1[15] =   9.682285 ;     	corners2[15] =  30.285353;    corners3[15] =   1.016053;
corners1[16] =  22.087612 ;     	corners2[16] =  16.360300;    corners3[16] =  21.726318;
corners1[17] =  24.393887 ;     	corners2[17] =  18.049274;    corners3[17] =   2.112776;
corners1[18] =  23.443268 ;     	corners2[18] =  40.041985;    corners3[18] =   5.958200;
corners1[19] =   4.878317 ;     	corners2[19] =   9.884295;    corners3[19] =  22.566002;
corners1[20] =  21.761997 ;     	corners2[20] =   1.679369;    corners3[20] =  22.058687;
corners1[21] =  16.815046 ;     	corners2[21] =  39.891823;    corners3[21] =  24.339502;
corners1[22] =  24.924917 ;     	corners2[22] =  36.762608;    corners3[22] =  21.643782;
corners1[23] =  13.439157 ;     	corners2[23] =  48.155560;    corners3[23] =   8.993539;
corners1[24] =  23.238096 ;     	corners2[24] =  23.869701;    corners3[24] =  21.557917;
corners1[25] =  16.988939 ;     	corners2[25] =  44.436829;    corners3[25] =  13.198540;
corners1[26] =   3.837111 ;     	corners2[26] =  33.109516;    corners3[26] =  17.931164;
corners1[27] =  17.476978 ;     	corners2[27] =  17.875248;    corners3[27] =  22.998663;
corners1[28] =  22.002773 ;     	corners2[28] =   2.881492;    corners3[28] =  24.262697;
corners1[29] =   4.222332 ;     	corners2[29] =  22.128315;    corners3[29] =   8.898322;
corners1[30] =  19.509399 ;     	corners2[30] =  31.934319;    corners3[30] =  21.682711;
corners1[31] =   7.029535 ;     	corners2[31] =  47.368095;    corners3[31] =  21.504633;


	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

	for(u32 i = 0; i < candCount; i++)
		candPos[i] = i+1;

	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);

	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_fp32Test, TestMinCandCount)
{
	width = 649;
	candCount = 1;
	candOutPos_C = 0;
	candOutPos_Asm = 0;

	inputGen.SelectGenerator("random");
	candPos = inputGen.GetLineU16(candCount, 1, width-1);
	corners1 = inputGen.GetLineFloat(width, -255, 255);
	corners2 = inputGen.GetLineFloat(width, -255, 255);
	corners3 = inputGen.GetLineFloat(width, -255, 255);

	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);

	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_fp32Test, TestMaxWidthMaxCandCount)
{
	width = 1920;
	candCount = 1920;
	candOutPos_C = 0;
	candOutPos_Asm = 0;

	inputGen.SelectGenerator("random");
	candPos = inputGen.GetLineU16(candCount, 1, width-1);
	corners1 = inputGen.GetLineFloat(width, -255, 255);
	corners2 = inputGen.GetLineFloat(width, -255, 255);
	corners3 = inputGen.GetLineFloat(width, -255, 255);

	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);

	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_fp32Test, TestLastCandIsMax)
{
	width = 1880;
	candCount = 80;

	inputGen.SelectGenerator("random");
	candPos = inputGen.GetLineU16(candCount, 1, width-1);
	corners1 = inputGen.GetLineFloat(width, -255, 255);
	corners2 = inputGen.GetLineFloat(width, -255, 255);
	corners3 = inputGen.GetLineFloat(width, -255, 255);

	corners2[78] = 3000;
	candPos[candCount-1] = 78;

	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}


TEST_F(NonMax3x3_fp32Test, TestZero)
{
	width = 500;
	candCount = 29;

	inputGen.SelectGenerator("uniform");
	candPos = inputGen.GetLineU16(candCount, 0);
	corners1 = inputGen.GetLineFloat(width,0);
	corners2 = inputGen.GetLineFloat(width,0);
	corners3 = inputGen.GetLineFloat(width, 0);

	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

int i = 0;
candPos[i++] = 206;
candPos[i++] = 207;
candPos[i++] = 208;
candPos[i++] = 209;
candPos[i++] = 210;
candPos[i++] = 211;
candPos[i++] = 212;
candPos[i++] = 213;
candPos[i++] = 214;
candPos[i++] = 215;
candPos[i++] = 216;
candPos[i++] = 217;
candPos[i++] = 283;
candPos[i++] = 284;
candPos[i++] = 285;
candPos[i++] = 286;
candPos[i++] = 287;
candPos[i++] = 288;
candPos[i++] = 289;
candPos[i++] = 290;
candPos[i++] = 291;
candPos[i++] = 292;
candPos[i++] = 293;
candPos[i++] = 294;
candPos[i++] = 295;
candPos[i++] = 316;
candPos[i++] = 317;
candPos[i++] = 318;
candPos[i++] = 319;
candCount = i;

corners1[199] =         14400.000000 ; corners2[199] =         7440.000000  ; 	corners3[199] =               2052.000000      ;
corners1[200] =        211784.000000 ; corners2[200] =       106100.000000  ; 	corners3[200] =              33464.000000      ;
corners1[201] =        407252.000000 ; corners2[201] =       166576.000000  ; 	corners3[201] =             103152.000000      ;
corners1[202] =        422496.000000 ; corners2[202] =       318276.000000  ; 	corners3[202] =             394164.000000      ;
corners1[203] =        430352.000000 ; corners2[203] =       233504.000000  ; 	corners3[203] =           32313600.000000      ;
corners1[204] =       1522324.000000 ; corners2[204] =     40989952.000000  ; 	corners3[204] =          452919296.000000      ;
corners1[205] =     233826048.000000 ; corners2[205] =    983498752.000000  ; 	corners3[205] =         3293970432.000000      ;
corners1[206] = 4294967295.4294967295; corners2[206] = 4294967295.4294967295; 	corners3[206] =        4294967295.4294967295   ;
corners1[207] = 4294967295.4294967295; corners2[207] = 4294967295.4294967295; 	corners3[207] =        4294967295.4294967295   ;
corners1[208] = 4294967295.4294967295; corners2[208] = 4294967295.4294967295; 	corners3[208] =        4294967295.4294967295   ;
corners1[209] = 4294967295.4294967295; corners2[209] = 4294967295.4294967295; 	corners3[209] =        4294967295.4294967295   ;
corners1[210] = 4294967295.4294967295; corners2[210] = 4294967295.4294967295; 	corners3[210] =        4294967295.4294967295   ;
corners1[211] = 4294967295.4294967295; corners2[211] = 4294967295.4294967295; 	corners3[211] =        4294967295.4294967295   ;
corners1[212] = 4294967295.4294967295; corners2[212] = 4294967295.4294967295; 	corners3[212] =        4294967295.4294967295   ;
corners1[213] = 4294967295.4294967295; corners2[213] = 4294967295.4294967295; 	corners3[213] =        4294967295.4294967295   ;
corners1[214] = 4294967295.4294967295; corners2[214] = 4294967295.4294967295; 	corners3[214] =        4294967295.4294967295   ;
corners1[215] = 4294967295.4294967295; corners2[215] = 4294967295.4294967295; 	corners3[215] =        4294967295.4294967295   ;
corners1[216] = 4294967295.4294967295; corners2[216] = 4294967295.4294967295; 	corners3[216] =        4294967295.4294967295   ;
corners1[217] =     779423744.000000 ; corners2[217] = 4294967295.4294967295 ; corners3[217] =       4294967295.4294967295    ;
corners1[218] =      34679552.000000 ; corners2[218] =    509186048.000000  ; 	corners3[218] =         3333160960.000000      ;
corners1[219] =       1549528.000000 ; corners2[219] =     32301872.000000  ; 	corners3[219] =          471175168.000000      ;
corners1[220] =        476276.000000 ; corners2[220] =       916076.000000  ; 	corners3[220] =           18802824.000000      ;
corners1[221] =         10784.000000 ; corners2[221] =        20160.000000  ; 	corners3[221] =              15516.000000      ;
corners1[222] =             8.000000 ; corners2[222] =            8.000000  ; 	corners3[222] =                 44.000000      ;
corners1[278] =             0.000000 ; corners2[278] =            0.000000  ; 	corners3[278] =                  4.000000      ;
corners1[279] =             0.000000 ; corners2[279] =            0.000000  ; 	corners3[279] =               3772.000000      ;
corners1[280] =           272.000000 ; corners2[280] =         1344.000000  ; 	corners3[280] =            2475328.000000      ;
corners1[281] =         35148.000000 ; corners2[281] =      1956800.000000  ; 	corners3[281] =           84762624.000000      ;
corners1[282] =       5758976.000000 ; corners2[282] =    125517824.000000  ; 	corners3[282] =         1462239232.000000      ;
corners1[283] =    2845634560.000000 ; corners2[283] = 4294967295.4294967295 ; corners3[283] =       4294967295.4294967295    ;
corners1[284] = 4294967295.4294967295; corners2[284] = 4294967295.4294967295; 	corners3[284] = 		4294967295.4294967295  ;
corners1[285] = 4294967295.4294967295; corners2[285] = 4294967295.4294967295; 	corners3[285] = 		4294967295.4294967295  ;
corners1[286] = 4294967295.4294967295; corners2[286] = 4294967295.4294967295; 	corners3[286] = 		4294967295.4294967295  ;
corners1[287] = 4294967295.4294967295; corners2[287] = 4294967295.4294967295; 	corners3[287] = 		4294967295.4294967295  ;
corners1[288] = 4294967295.4294967295; corners2[288] = 4294967295.4294967295; 	corners3[288] = 		4294967295.4294967295  ;
corners1[289] = 4294967295.4294967295; corners2[289] = 4294967295.4294967295; 	corners3[289] = 		4294967295.4294967295  ;
corners1[290] = 4294967295.4294967295; corners2[290] = 4294967295.4294967295; 	corners3[290] = 		4294967295.4294967295  ;
corners1[291] = 4294967295.4294967295; corners2[291] = 4294967295.4294967295; 	corners3[291] = 		4294967295.4294967295  ;
corners1[292] = 4294967295.4294967295; corners2[292] = 4294967295.4294967295; 	corners3[292] = 		4294967295.4294967295  ;
corners1[293] = 4294967295.4294967295; corners2[293] = 4294967295.4294967295; 	corners3[293] = 		4294967295.4294967295  ;
corners1[294] =    1677479936.000000 ; corners2[294] = 4294967295.4294967295 ; corners3[294] = 		4294967295.4294967295  ;
corners1[295] =     152357248.000000 ; corners2[295] =   1738915840.000000  ; 	corners3[295] = 		4294967295.4294967295  ;
corners1[296] =       2723156.000000 ; corners2[296] =    143660288.000000  ; 	corners3[296] = 		  1123983360.000000    ;
corners1[297] =       1159724.000000 ; corners2[297] =      1107468.000000  ; 	corners3[297] = 		    45072192.000000    ;
corners1[298] =        802724.000000 ; corners2[298] =       193312.000000  ; 	corners3[298] = 		      144768.000000    ;
corners1[299] =        749504.000000 ; corners2[299] =       170388.000000  ; 	corners3[299] = 		      197056.000000    ;
corners1[300] =        747672.000000 ; corners2[300] =       241248.000000  ; 	corners3[300] = 		      252076.000000    ;
corners1[301] =        319152.000000 ; corners2[301] =       172692.000000  ; 	corners3[301] = 		      179672.000000    ;
corners1[302] =         13920.000000 ; corners2[302] =        21304.000000  ; 	corners3[302] = 		       28112.000000    ;
corners1[303] =             0.000000 ; corners2[303] =            8.000000  ; 	corners3[303] = 		          20.000000    ;
corners1[311] =           288.000000 ; corners2[311] =          288.000000  ; 	corners3[311] = 		         416.000000    ;
corners1[312] =         32256.000000 ; corners2[312] =        80720.000000  ; 	corners3[312] = 		      536088.000000    ;
corners1[313] =        525920.000000 ; corners2[313] =      1185852.000000  ; 	corners3[313] = 		    80239616.000000    ;
corners1[314] =       2214216.000000 ; corners2[314] =    110258432.000000  ; 	corners3[314] = 		  1144029184.000000    ;
corners1[315] =     108896256.000000 ; corners2[315] =    614858752.000000  ; 	corners3[315] = 		  2535849984.000000    ;
corners1[316] =    2973483008.000000 ; corners2[316] = 4294967295.4294967295 ;	corners3[316] = 		4294967295.4294967295  ;
corners1[317] = 4294967295.4294967295; corners2[317] = 4294967295.4294967295; 	corners3[317] = 		 4294967295.4294967295 ;
corners1[318] = 4294967295.4294967295; corners2[318] = 4294967295.4294967295; 	corners3[318] = 		 4294967295.4294967295 ;
corners1[319] = 4294967295.4294967295; corners2[319] = 4294967295.4294967295; 	corners3[319] = 		 4294967295.4294967295 ;
corners1[320] = 4294967295.4294967295; corners2[320] = 4294967295.4294967295; 	corners3[320] = 		 4294967295.4294967295 ;
corners1[321] = 4294967295.4294967295; corners2[321] = 4294967295.4294967295; 	corners3[321] = 		 4294967295.4294967295 ;
corners1[322] = 4294967295.4294967295; corners2[322] = 4294967295.4294967295; 	corners3[322] = 		 4294967295.4294967295 ;
corners1[323] = 4294967295.4294967295; corners2[323] = 4294967295.4294967295; 	corners3[323] = 		 4294967295.4294967295 ;
corners1[324] = 4294967295.4294967295; corners2[324] = 4294967295.4294967295; 	corners3[324] = 		 4294967295.4294967295 ;
corners1[325] = 4294967295.4294967295; corners2[325] = 4294967295.4294967295; 	corners3[325] = 		 4294967295.4294967295 ;
corners1[326] = 4294967295.4294967295; corners2[326] = 4294967295.4294967295; 	corners3[326] = 		 4294967295.4294967295 ;
corners1[327] =    2015825920.000000 ; corners2[327] = 4294967295.4294967295 ;	corners3[327] = 		4294967295.4294967295  ;
corners1[328] =      61343328.000000 ; corners2[328] =    491700224.000000  ; 	corners3[328] = 		4294967295.4294967295  ;
corners1[329] =        412948.000000 ; corners2[329] =      8680672.000000  ; 	corners3[329] = 		   261967872.000000    ;
corners1[330] =        320396.000000 ; corners2[330] =       872528.000000  ; 	corners3[330] = 		    19036800.000000    ;
corners1[331] =        443236.000000 ; corners2[331] =       858372.000000  ; 	corners3[331] = 		     1893296.000000    ;
corners1[332] =        209500.000000 ; corners2[332] =       284304.000000  ; 	corners3[332] = 		      215792.000000    ;
corners1[333] =         46884.000000 ; corners2[333] =        57620.000000  ; 	corners3[333] = 		        3540.000000    ;
corners1[334] =           500.000000 ; corners2[334] =          660.000000  ; 	corners3[334] = 		          36.000000    ;
corners1[398] =             0.000000 ; corners2[398] =           64.000000  ; 	corners3[398] = 		         192.000000    ;
corners1[399] =          4376.000000 ; corners2[399] =        42532.000000  ; 	corners3[399] = 		       69732.000000    ;
corners1[400] =        646336.000000 ; corners2[400] =      1615764.000000  ; 	corners3[400] = 		      699164.000000    ;
corners1[401] =       3621536.000000 ; corners2[401] =      6365776.000000  ; 	corners3[401] = 		     1690724.000000    ;
corners1[402] =       4946400.000000 ; corners2[402] =      7145452.000000  ; 	corners3[402] = 		     1326396.000000    ;
corners1[403] =       1739120.000000 ; corners2[403] =      2622780.000000  ; 	corners3[403] = 		    21669264.000000    ;
corners1[404] =       1547292.000000 ; corners2[404] =      1380668.000000  ; 	corners3[404] = 		   624696320.000000    ;
corners1[405] =        894776.000000 ; corners2[405] =     39538464.000000  ; 	corners3[405] = 		4294967295.4294967295  ;
corners1[406] =        500660.000000 ; corners2[406] =   1007084544.000000  ; 	corners3[406] = 		4294967295.4294967295  ;
corners1[407] =        176648.000000 ; corners2[407] = 4294967295.4294967295 ;	corners3[407] = 		4294967295.4294967295  ;
corners1[408] =        231692.000000 ; corners2[408] = 4294967295.4294967295 ;	corners3[408] = 		4294967295.4294967295  ;
corners1[409] =        295316.000000 ; corners2[409] =   3756383232.000000  ; 	corners3[409] = 		4294967295.4294967295  ;
corners1[410] =        207212.000000 ; corners2[410] = 4294967295.4294967295 ;	corners3[410] = 		4294967295.4294967295  ;
corners1[411] =        543836.000000 ; corners2[411] = 4294967295.4294967295 ;	corners3[411] = 		4294967295.4294967295  ;
corners1[412] =       1343336.000000 ; corners2[412] =    910368256.000000  ; 	corners3[412] = 		4294967295.4294967295  ;
corners1[413] =       1212592.000000 ; corners2[413] =     31304384.000000  ; 	corners3[413] = 		4294967295.4294967295  ;
corners1[414] =        849056.000000 ; corners2[414] =      1502296.000000  ; 	corners3[414] = 		   919136256.000000    ;
corners1[415] =        512580.000000 ; corners2[415] =      1110688.000000  ; 	corners3[415] = 		    42325168.000000    ;
corners1[416] =        119180.000000 ; corners2[416] =       738416.000000  ; 	corners3[416] = 		     2671708.000000    ;
corners1[417] =        371504.000000 ; corners2[417] =       345536.000000  ; 	corners3[417] = 		     1503916.000000    ;
corners1[418] =       1494044.000000 ; corners2[418] =       719688.000000  ; 	corners3[418] = 		      438224.000000    ;
corners1[419] =        857584.000000 ; corners2[419] =       701044.000000  ; 	corners3[419] = 		      172508.000000    ;
corners1[420] =        268880.000000 ; corners2[420] =       428656.000000  ; 	corners3[420] = 		       53636.000000    ;
corners1[421] =          7256.000000 ; corners2[421] =        16920.000000  ; 	corners3[421] = 		        5252.000000    ;
corners1[499] =             4.000000 ; corners2[499] =            0.000000  ; 	corners3[499] = 		           0.000000    ;


	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}


TEST_F(NonMax3x3_fp32Test, TestRandomData)
{
	width = randGen->GenerateUInt(8, 1920, 8);	
	candCount = randGen->GenerateUInt(1, width, 1);

	inputGen.SelectGenerator("random");
	candPos = inputGen.GetLineU16(candCount, 1, width-1);
	corners1 = inputGen.GetLineFloat(width, -255, 255);
	corners2 = inputGen.GetLineFloat(width, -255, 255);
	corners3 = inputGen.GetLineFloat(width, -255, 255);

	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(candCount, 0);
	candOutPos_Asm = inputGen.GetLineU16(candCount, 0);

	mvcvNonMax3x3_fp32(candPos, candCount, corners1, corners2,
						corners3, candOutPos_C, &candOutCount_C);
	nonMax3x3_fp32_asm_test(candPos, candCount, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);

	RecordProperty("CyclePerPixel", nonMax3x3_fp32CycleCount / candCount);

	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}
