#ifndef MESSAGES_H__
#define MESSAGES_H__

#ifndef MAX_CAMERA_COUNT
#define MAX_CAMERA_COUNT 2
#endif
#include <stdint.h>
#include <ImgFrame.h>

const int VISION_BASE_MAX_NUM_CAMS = 2;
class StereoImgMsg /*: public IMessage*/{
  public:
    ImgFramePtr frame[VISION_BASE_MAX_NUM_CAMS]; // left first, right second
};

inline bool operator==(const StereoImgMsg& lhs, const StereoImgMsg& rhs)
{
    if(
    (lhs.frame[0]      == rhs.frame[0]  ) &&
    (lhs.frame[1]      == rhs.frame[1]  )) return true; 
    else                                return false;
}

class StereoImgAndMetaMsg /*: public IMessage*/{
  public:
    int count; // this may be different for images and metadata in future
    ImgFramePtr frame[VISION_BASE_MAX_NUM_CAMS]; // left first, right second
    ImgFramePtr meta[VISION_BASE_MAX_NUM_CAMS];  // left first, right second
};
inline bool operator==(const StereoImgAndMetaMsg& lhs, const StereoImgAndMetaMsg& rhs)
{
    bool ret = false;
    if(lhs.count == rhs.count) {
        for(int i=0;i<lhs.count;i++) {
            if(lhs.frame[i] == rhs.frame[i] &&
                lhs.meta[i] == rhs.meta[i]) {
                ret = true;
            } else {
                return false;
            }
        }
    }
    return ret;
}


#endif
