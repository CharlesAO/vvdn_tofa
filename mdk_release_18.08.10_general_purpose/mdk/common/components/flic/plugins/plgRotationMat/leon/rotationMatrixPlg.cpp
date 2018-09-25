#include "rotationMatrixPlg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <rotationMatMsg.h>


#define MVLOG_UNIT_NAME rotationMatrixNode
#include <mvLog.h>


const float  PlgRotationMat::c_nsPerSecond = 1/1000000000.0;
const int PlgRotationMat::BIAS_ESTIMATE_COUNT = 100;
const float PlgRotationMat::DEG_PER_RADIAN = 180.0f / M_PI;

int PlgRotationMat::integrateImuSample(mvImuData_t* imuCurr, mvImuData_t* imuPrev){
    u64 delta_t_ns;
    float deltaTseconds;
    if (imuPrev->timestampNs < previousTs &&
                    imuCurr->timestampNs > previousTs)
    {
        delta_t_ns = imuCurr->timestampNs - previousTs;
    }
    else if (imuPrev->timestampNs >= previousTs &&
                    imuCurr->timestampNs <= currentTs)
    {
        delta_t_ns = imuCurr->timestampNs - imuPrev->timestampNs;
    }
    else if (imuPrev->timestampNs < currentTs &&
                    imuCurr->timestampNs >= currentTs)
    {
        delta_t_ns = currentTs - imuPrev->timestampNs;
    }
    else
    {
        //both timestamps bigger than the current one. Don't process any more samples
        return -1;
    }
    deltaTseconds = delta_t_ns * c_nsPerSecond;
    if (biasEstimate < BIAS_ESTIMATE_COUNT){
        bias_x += imuPrev->axisData.x;
        bias_y += imuPrev->axisData.y;
        bias_z += imuPrev->axisData.z;
        biasEstimate++;
    } else if (biasEstimate == BIAS_ESTIMATE_COUNT){
        bias_x /= biasEstimate;
        bias_y /= biasEstimate;
        bias_z /= biasEstimate;
        biasEstimate++;
    } else{
        rotMat.rotationMatrix[0] += (imuPrev->axisData.x - bias_x) * deltaTseconds * DEG_PER_RADIAN;
        rotMat.rotationMatrix[1] += (imuPrev->axisData.y - bias_y) * deltaTseconds * DEG_PER_RADIAN;
        rotMat.rotationMatrix[2] += (imuPrev->axisData.z - bias_z) * deltaTseconds * DEG_PER_RADIAN;
    }
    if (imuCurr->timestampNs > currentTs){
        // we don't need more samples as the current one is bigger than our timestamp
        return -1;
    }else{
        return 0;
    }
}

int PlgRotationMat::processImuSample(ImgFrame* msg){
    mvImuData_t* imuSample = (mvImuData_t*) msg->fb.p1;


    if(imuSample->type == IMU_TYPE_GYROSCOPE){
        if (imuSample->timestampNs >= previousTs){
            if (isPrevValid){
//                previousSample.Release();
            }
            previousSample = currentSample;
            currentSample = *imuSample;
            isPrevValid = isCurrentValid;
            isCurrentValid = 1;
            if (isPrevValid){
                return integrateImuSample(imuSample, (mvImuData_t*) &previousSample);
            }
            return 0 ;

        }
        else{
            // Not relevant. Just drop this sample
        }
    }else{
        // Not gyro. just drop this sample
    }
//    msg->Release();
    return 0;
}
void* PlgRotationMat::threadFunc(void* context){
    UNUSED(context);
    mvLogLevelSet(MVLOG_DEBUG);
    ImgFramePtr msgImu;
    ImgFramePtr msgFrame;
    while(1){
        int i;
        //wait for a new frame
        int rc1 = timeStampIn.Receive(&msgFrame);
        UNUSED(rc1);

        //then just release it until we read freqDivider frames
        for( i = 1; i < freqDivider; i++){
//            msgFrame.Release();
            int rc2 = timeStampIn.Receive(&msgFrame);
            UNUSED(rc2);
        }
        //get the timestamp of it
        ImgFrame* imgFrame = (ImgFrame*) msgFrame.ptr;
        currentTs = imgFrame->ts;
//        msgFrame.Release();
        rotMat.rotationMatrix[0] = 0;
        rotMat.rotationMatrix[1] = 0;
        rotMat.rotationMatrix[2] = 0;
        if (previousTs == 0){
            previousTs = currentTs - ONE_FRAME_DELAY_NS;
        }
        if (isPrevValid && isCurrentValid){
            // we need to add the last measurement of previous cycle to this rotation matrix
            // currImuTs > prevFrTs && prevImuTs < prevFrTs
            // we are not doing this with processImuSample, as this is the second time we are using the same IMU samples, so the required actions were performed already
            integrateImuSample((mvImuData_t*) &currentSample,
                               (mvImuData_t*) &previousSample);
        }
        // get all IMU samples and free them up as we are using them
        while( inIMU.TryReceive(&msgImu) == 0){
            if (processImuSample(msgImu.ptr))
                break;
        }
        //TODO: detect gyro version. better method would be rotation matrix in the board driver
        struct stat res;
        if (stat("/dev/i2c.BMI160", &res) == 0){
            //imuFifo
//            rotMat.rotationMatrix[0] = rotMat.rotationMatrix[0];
            rotMat.rotationMatrix[1] = -rotMat.rotationMatrix[1];
        }else{
            rotMat.rotationMatrix[0] = -rotMat.rotationMatrix[0];
//            rotMat.rotationMatrix[1] = rotMat.rotationMatrix[1];
        }
        rotMat.rotationMatrix[2] = -rotMat.rotationMatrix[2];
        dbgTotalRotMat.rotationMatrix[0] += rotMat.rotationMatrix[0];
        dbgTotalRotMat.rotationMatrix[1] += rotMat.rotationMatrix[1];
        dbgTotalRotMat.rotationMatrix[2] += rotMat.rotationMatrix[2];
        previousTs = currentTs;
        rotMat.timeStamp = currentTs;
        // send rotation matrix
        out.Send(&rotMat);
    }
    return NULL;
}


