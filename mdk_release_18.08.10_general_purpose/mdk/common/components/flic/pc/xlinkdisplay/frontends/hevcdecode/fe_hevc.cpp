/// =====================================================================================
///
///        @file:      fe_hevc.c
///        @brief:     h265 decode frontend based on gstreamer
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///
#include <fe_hevc.h>
/// System Includes
/// -------------------------------------------------------------------------------------

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <mqueue.h>
#include <assert.h>
#include "helper.h"
#ifdef __cplusplus
}
#endif

#include "fe_cvdisplay.h"

void *Hevc::ThreadItf(void *This)
{
   ((Hevc*)This)->Thread();
}


void *Hevc::Thread()
{
    static int count = 0;
    count ++;
    AVFormatContext *pFormatCtx;
    int             i, videoindex;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame *pFrame,*pFrameYUV;
    unsigned char *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;

    char* filepath = (char*)fifoName;

    printf ( "Fifo name: %s\n", filepath );

    FILE *fp_yuv=fopen("output.yuv","wb+");  

    mqd_t mqdesO = mq_open((const char *)mqName, O_RDWR);
    if (mqdesO == (mqd_t) - 1) {
        perror("mq_open");
        assert(0);
    }

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        return 0;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        return 0;
    }
    videoindex=-1;
    for(i=0; i<pFormatCtx->nb_streams; i++) 
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    
    if(videoindex==-1){
        printf("Didn't find a video stream.\n");
        return 0;
    }

    pCodecCtx=pFormatCtx->streams[videoindex]->codec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        return 0;
    }
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        return 0;
    }
    
    pFrame=av_frame_alloc();
    pFrameYUV=av_frame_alloc();
    out_buffer=(unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P,  pCodecCtx->width, pCodecCtx->height,1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize,out_buffer,
        AV_PIX_FMT_YUV420P,pCodecCtx->width, pCodecCtx->height,1);

    
    
    packet=(AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    av_dump_format(pFormatCtx,0,filepath,0);
    printf("-------------------------------------------------\n");
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 

    uint32_t decodecnt = 0;
    uint32_t errorcnt = 0;

    while(av_read_frame(pFormatCtx, packet)>=0){
        if(packet->stream_index==videoindex){
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if(ret < 0){
                printf("Decode Error.\n");
                //return 0;
                errorcnt++;
            }
            if(got_picture){
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
                    pFrameYUV->data, pFrameYUV->linesize);

                y_size=pCodecCtx->width*pCodecCtx->height;  
//                fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y 
//                fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
//                fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V
                printf("Succeed to decode 1 frame! #%u (Errors: #%u)\n", decodecnt++, errorcnt);

                uint8_t* dispfrm = (uint8_t*)malloc(y_size*1.5);
                memcpy(dispfrm, pFrameYUV->data[0], y_size);
                memcpy(dispfrm+y_size, pFrameYUV->data[1], y_size/4);
                memcpy(dispfrm+y_size+y_size/4, pFrameYUV->data[2], y_size/4);

                cvShowMsg_t request;
                request.data = dispfrm;
                request.windowname = this->windowName;
                request.spec.width = pCodecCtx->width;
                request.spec.height = pCodecCtx->height;
                request.spec.format = FRAME_FORMAT_YUV420P;
                request.scaling = this->scaling;
                request.mode = CV_IMAGE_DISPLAY;
                request.seqNo = decodecnt;
                int status = mq_send(mqdesO, (char *) &request, sizeof(cvShowMsg_t), 0);
                if (status) {
                    perror("mq_send");
                    assert(0);
                }

            }
        }
        av_free_packet(packet);
    }
    //flush decoder
    //FIX: Flush Frames remained in Codec
    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0)
            break;
        if (!got_picture)
            break;
        sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
            pFrameYUV->data, pFrameYUV->linesize);

        int y_size=pCodecCtx->width*pCodecCtx->height;  
        fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y 
        fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U
        fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V

        printf("Flush Decoder: Succeed to decode 1 frame!\n");
    }

    sws_freeContext(img_convert_ctx);

    fclose(fp_yuv);

    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}

void Hevc::Start()
{
    int ret = pthread_create(&thread, NULL, ThreadItf, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void Hevc::Stop()
{
    pthread_join(thread, NULL);
}
