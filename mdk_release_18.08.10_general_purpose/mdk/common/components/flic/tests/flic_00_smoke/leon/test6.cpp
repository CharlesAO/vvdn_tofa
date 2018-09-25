///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Master-Send -> Slave-Receive (multi-instance)
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

class MA { public: int dA;};
class MB { public: int dB;};
class MC { public: int dC;};
class MD { public: int dD;};
class ME { public: int dE;};
class MF { public: int dF;};
class MG { public: int dG;};
class MH { public: int dH;};
class MI { public: int dI;};
class MJ { public: int dJ;};
class MK { public: int dK;};
class ML { public: int dL;};
class MM { public: int dM;};
class MN { public: int dN;};
class MO { public: int dO;};

//Check err code
#define C(x) unitTestAssert(OK==x)

//###############################################################
//Multiple instances
//###############################################################
void test6()
{
    MA mA; MSender<MA> sA; SReceiver<MA> rA; rA.Create(4); sA.Link(&rA); mA.dA = 10; sA.Send(&mA); C(rA.Receive(&mA));  printf("Rec %d\n", mA.dA);  rA.Destroy(); sA.Delete(); rA.Delete();
    MB mB; MSender<MB> sB; SReceiver<MB> rB; rB.Create(4); sB.Link(&rB); mB.dB = 11; sB.Send(&mB); C(rB.Receive(&mB));  printf("Rec %d\n", mB.dB);  rB.Destroy(); sB.Delete(); rB.Delete();
    MC mC; MSender<MC> sC; SReceiver<MC> rC; rC.Create(4); sC.Link(&rC); mC.dC = 12; sC.Send(&mC); C(rC.Receive(&mC));  printf("Rec %d\n", mC.dC);  rC.Destroy(); sC.Delete(); rC.Delete();
    MD mD; MSender<MD> sD; SReceiver<MD> rD; rD.Create(4); sD.Link(&rD); mD.dD = 13; sD.Send(&mD); C(rD.Receive(&mD));  printf("Rec %d\n", mD.dD);  rD.Destroy(); sD.Delete(); rD.Delete();
    ME mE; MSender<ME> sE; SReceiver<ME> rE; rE.Create(4); sE.Link(&rE); mE.dE = 14; sE.Send(&mE); C(rE.Receive(&mE));  printf("Rec %d\n", mE.dE);  rE.Destroy(); sE.Delete(); rE.Delete();
    MF mF; MSender<MF> sF; SReceiver<MF> rF; rF.Create(4); sF.Link(&rF); mF.dF = 15; sF.Send(&mF); C(rF.Receive(&mF));  printf("Rec %d\n", mF.dF);  rF.Destroy(); sF.Delete(); rF.Delete();
    MG mG; MSender<MG> sG; SReceiver<MG> rG; rG.Create(4); sG.Link(&rG); mG.dG = 16; sG.Send(&mG); C(rG.Receive(&mG));  printf("Rec %d\n", mG.dG);  rG.Destroy(); sG.Delete(); rG.Delete();
    MH mH; MSender<MH> sH; SReceiver<MH> rH; rH.Create(4); sH.Link(&rH); mH.dH = 17; sH.Send(&mH); C(rH.Receive(&mH));  printf("Rec %d\n", mH.dH);  rH.Destroy(); sH.Delete(); rH.Delete();
    MI mI; MSender<MI> sI; SReceiver<MI> rI; rI.Create(4); sI.Link(&rI); mI.dI = 18; sI.Send(&mI); C(rI.Receive(&mI));  printf("Rec %d\n", mI.dI);  rI.Destroy(); sI.Delete(); rI.Delete();
    MJ mJ; MSender<MJ> sJ; SReceiver<MJ> rJ; rJ.Create(4); sJ.Link(&rJ); mJ.dJ = 19; sJ.Send(&mJ); C(rJ.Receive(&mJ));  printf("Rec %d\n", mJ.dJ);  rJ.Destroy(); sJ.Delete(); rJ.Delete();
    MK mK; MSender<MK> sK; SReceiver<MK> rK; rK.Create(4); sK.Link(&rK); mK.dK = 20; sK.Send(&mK); C(rK.Receive(&mK));  printf("Rec %d\n", mK.dK);  rK.Destroy(); sK.Delete(); rK.Delete();
    ML mL; MSender<ML> sL; SReceiver<ML> rL; rL.Create(4); sL.Link(&rL); mL.dL = 21; sL.Send(&mL); C(rL.Receive(&mL));  printf("Rec %d\n", mL.dL);  rL.Destroy(); sL.Delete(); rL.Delete();
    MM mM; MSender<MM> sM; SReceiver<MM> rM; rM.Create(4); sM.Link(&rM); mM.dM = 22; sM.Send(&mM); C(rM.Receive(&mM));  printf("Rec %d\n", mM.dM);  rM.Destroy(); sM.Delete(); rM.Delete();
    MN mN; MSender<MN> sN; SReceiver<MN> rN; rN.Create(4); sN.Link(&rN); mN.dN = 23; sN.Send(&mN); C(rN.Receive(&mN));  printf("Rec %d\n", mN.dN);  rN.Destroy(); sN.Delete(); rN.Delete();
    MO mO; MSender<MO> sO; SReceiver<MO> rO; rO.Create(4); sO.Link(&rO); mO.dO = 24; sO.Send(&mO); C(rO.Receive(&mO));  printf("Rec %d\n", mO.dO);  rO.Destroy(); sO.Delete(); rO.Delete();
}