///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple instances (code size estimation for MsgBuf)
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>

class MsgA { public: int dA;};
class MsgB { public: int dB;};
class MsgC { public: int dC;};
class MsgD { public: int dD;};
class MsgE { public: int dE;};
class MsgF { public: int dF;};
class MsgG { public: int dG;};
class MsgH { public: int dH;};
class MsgI { public: int dI;};
class MsgJ { public: int dJ;};
class MsgK { public: int dK;};
class MsgL { public: int dL;};
class MsgM { public: int dM;};
class MsgN { public: int dN;};
class MsgO { public: int dO;};

//###############################################################
//Multiple instances
//###############################################################
void test2()
{
    int c = 0;
    MsgBuf<MsgA> bA; bA.Create(8); MsgA mA;mA.dA = 101; bA.Push(NULL, &mA); bA.Pop(NULL, &mA);  bA.TryPush(NULL, &mA); bA.TryPop(NULL, &mA); c+=bA.Count()+1; bA.Destroy(); bA.Delete();
    MsgBuf<MsgB> bB; bB.Create(8); MsgB mB;mB.dB = 101; bB.Push(NULL, &mB); bB.Pop(NULL, &mB);  bB.TryPush(NULL, &mB); bB.TryPop(NULL, &mB); c+=bB.Count()+1; bB.Destroy(); bB.Delete();
    MsgBuf<MsgC> bC; bC.Create(8); MsgC mC;mC.dC = 101; bC.Push(NULL, &mC); bC.Pop(NULL, &mC);  bC.TryPush(NULL, &mC); bC.TryPop(NULL, &mC); c+=bC.Count()+1; bC.Destroy(); bC.Delete();
    MsgBuf<MsgD> bD; bD.Create(8); MsgD mD;mD.dD = 101; bD.Push(NULL, &mD); bD.Pop(NULL, &mD);  bD.TryPush(NULL, &mD); bD.TryPop(NULL, &mD); c+=bD.Count()+1; bD.Destroy(); bD.Delete();
    MsgBuf<MsgE> bE; bE.Create(8); MsgE mE;mE.dE = 101; bE.Push(NULL, &mE); bE.Pop(NULL, &mE);  bE.TryPush(NULL, &mE); bE.TryPop(NULL, &mE); c+=bE.Count()+1; bE.Destroy(); bE.Delete();
    MsgBuf<MsgF> bF; bF.Create(8); MsgF mF;mF.dF = 101; bF.Push(NULL, &mF); bF.Pop(NULL, &mF);  bF.TryPush(NULL, &mF); bF.TryPop(NULL, &mF); c+=bF.Count()+1; bF.Destroy(); bF.Delete();
    MsgBuf<MsgG> bG; bG.Create(8); MsgG mG;mG.dG = 101; bG.Push(NULL, &mG); bG.Pop(NULL, &mG);  bG.TryPush(NULL, &mG); bG.TryPop(NULL, &mG); c+=bG.Count()+1; bG.Destroy(); bG.Delete();
    MsgBuf<MsgH> bH; bH.Create(8); MsgH mH;mH.dH = 101; bH.Push(NULL, &mH); bH.Pop(NULL, &mH);  bH.TryPush(NULL, &mH); bH.TryPop(NULL, &mH); c+=bH.Count()+1; bH.Destroy(); bH.Delete();
    MsgBuf<MsgI> bI; bI.Create(8); MsgI mI;mI.dI = 101; bI.Push(NULL, &mI); bI.Pop(NULL, &mI);  bI.TryPush(NULL, &mI); bI.TryPop(NULL, &mI); c+=bI.Count()+1; bI.Destroy(); bI.Delete();
    MsgBuf<MsgJ> bJ; bJ.Create(8); MsgJ mJ;mJ.dJ = 101; bJ.Push(NULL, &mJ); bJ.Pop(NULL, &mJ);  bJ.TryPush(NULL, &mJ); bJ.TryPop(NULL, &mJ); c+=bJ.Count()+1; bJ.Destroy(); bJ.Delete();
    MsgBuf<MsgK> bK; bK.Create(8); MsgK mK;mK.dK = 101; bK.Push(NULL, &mK); bK.Pop(NULL, &mK);  bK.TryPush(NULL, &mK); bK.TryPop(NULL, &mK); c+=bK.Count()+1; bK.Destroy(); bK.Delete();
    MsgBuf<MsgL> bL; bL.Create(8); MsgL mL;mL.dL = 101; bL.Push(NULL, &mL); bL.Pop(NULL, &mL);  bL.TryPush(NULL, &mL); bL.TryPop(NULL, &mL); c+=bL.Count()+1; bL.Destroy(); bL.Delete();
    MsgBuf<MsgM> bM; bM.Create(8); MsgM mM;mM.dM = 101; bM.Push(NULL, &mM); bM.Pop(NULL, &mM);  bM.TryPush(NULL, &mM); bM.TryPop(NULL, &mM); c+=bM.Count()+1; bM.Destroy(); bM.Delete();
    MsgBuf<MsgN> bN; bN.Create(8); MsgN mN;mN.dN = 101; bN.Push(NULL, &mN); bN.Pop(NULL, &mN);  bN.TryPush(NULL, &mN); bN.TryPop(NULL, &mN); c+=bN.Count()+1; bN.Destroy(); bN.Delete();
    MsgBuf<MsgO> bO; bO.Create(8); MsgO mO;mO.dO = 101; bO.Push(NULL, &mO); bO.Pop(NULL, &mO);  bO.TryPush(NULL, &mO); bO.TryPop(NULL, &mO); c+=bO.Count()+1; bO.Destroy(); bO.Delete();
    printf("Count = %d\n", c);
}