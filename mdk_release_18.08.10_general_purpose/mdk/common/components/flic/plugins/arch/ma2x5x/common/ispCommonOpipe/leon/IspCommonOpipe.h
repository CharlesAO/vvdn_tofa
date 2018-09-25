///
/// @file      IspCommonOpipe.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Opipe common and serialization side Interface for the Isp flic plugins.
///

#ifndef COMPONENTS_FLIC_PLUGINS_COMMON_LEON_IspCommon_H_
#define COMPONENTS_FLIC_PLUGINS_COMMON_LEON_IspCommon_H_

#include "ImgFrame.h"
#include "OpipeApps.h"

#ifndef PLG_ISP_MAX_W
#define PLG_ISP_MAX_W (2016)
#endif

class IspCommonOpipe {
public:

    void Create() {
        if (0 == plgIspMtxInstanceCnt) {
            if (pthread_mutex_init(&lockMtx, NULL) != 0){
                printf("\n mutex init failed\n");
                assert(0);
            }
        }
        LockMtx();
        plgIspMtxInstanceCnt++;
        UnlockMtx();

    }
    void Destroy() {
        //delete sharing resources mutex. if is not already deleted
        LockMtx();
        plgIspMtxInstanceCnt--;
        UnlockMtx();
        if (0 == plgIspMtxInstanceCnt) {
            if (pthread_mutex_destroy(&lockMtx) != 0){
                printf("\n mutex destroy failed\n");
                assert(0);
            }
        }
    }
    void LockMtx() {
        int rc = pthread_mutex_lock(&(lockMtx));
        assert((rc==0) && "Not possible to unlock Opipe Serialize mutex");
    }
    void UnlockMtx() {
        int rc = pthread_mutex_unlock(&(lockMtx));
        assert((rc==0) && "Not possible to unlock Opipe Serialize mutex");
    }
    void SetCircBufsAdr(Buf *sigma, Buf *dbyr, Buf *sharp,
            Buf *lut, Buf *poly) {

        if(sigma) {sigma->base = (uint32_t)cSigma;   sigma->h = I_CBUFF_H;}
        if(dbyr) {dbyr->base  = (uint32_t)cDbyrY;   dbyr->h  = DBYR_Y_H;}
        if(sharp) {sharp->base = (uint32_t)cSharpY;  sharp->h = SHARP_Y_H;}
        if(lut) {lut->base   = (uint32_t)cLut;     lut->h   = O_CBUFF_H;}
        if(poly) {poly->base  = (uint32_t)cUpfirDn; poly->h  = O_CBUFF_H;}
    }
private:
    static pthread_mutex_t lockMtx;
    static uint32_t plgIspMtxInstanceCnt;
    static uint16_t cSigma  [];
    static uint16_t cDbyrY  [];
    static uint16_t cSharpY [];
    static uint8_t  cLut    [];
    static uint8_t  cUpfirDn[];
};


#endif /* COMPONENTS_FLIC_PLUGINS_COMMON_LEON_IspCommon_H_ */
