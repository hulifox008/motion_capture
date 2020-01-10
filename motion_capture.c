#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <stdio.h>


int main()
{
    AVFormatContext *pFormatCtx;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVDictionary    *option_dict = NULL;
    int videoindex;
    int i;

    avdevice_register_all();

    pFormatCtx = avformat_alloc_context();
    fprintf(stderr, "pFormatCtx %p\n", pFormatCtx);
    
    AVInputFormat *ifmt = av_find_input_format("v4l2");
    fprintf(stderr, "ifmt %p\n", ifmt);

    av_dict_set(&option_dict, "input_format", "mjpeg", 0);
    av_dict_set(&option_dict, "video_size", "640x480", 0);
    av_dict_set(&option_dict, "framerate", "5", 0);
    if(avformat_open_input(&pFormatCtx, "/dev/video0", ifmt, &option_dict) != 0) {
        fprintf(stderr, "Could not open video device!\n");
        return -1;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        fprintf(stderr, "Couldn't find sream information!\n");
        return -1;
    }

    videoindex=-1;
    for(i=0;i<pFormatCtx->nb_streams;i++) {
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }

    if(videoindex == -1) {
        fprintf(stderr, "Couldn't find a video stream.\n");
        return -1;
    }

    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL) {
        fprintf(stderr, "Codec not found.\n");
        return -1;
    }

    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0) {
        fprintf(stderr, "Could not open codec.\n");
        return -1;
    }

    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));
    AVFrame  *pFrame = av_frame_alloc();
    int frames = 0;
    int got_picture;
    for(;;) {
        if(av_read_frame(pFormatCtx, packet) >= 0) {
            frames++;
            if(avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet)<0) {
                fprintf(stderr, "avcodec_decode_video2() failed!\n");
                break;
            }
            //fprintf(stderr, "frames %d pts:%lu dts:%lu csize:%d w:%d h:%d got:%d\n", frames, packet->pts, packet->dts, packet->size, pFrame->width, pFrame->height, got_picture);
            fprintf(stderr, "linsize %d %d %d\n", pFrame->linesize[0], pFrame->linesize[1], pFrame->linesize[2]);
            av_free_packet(packet);
        }else {
            break;
        }
    }
}

