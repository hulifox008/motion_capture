#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
#include <stdio.h>


int main()
{
    AVFormatContext *pFormatCtx;

    pFormatCtx = avformat_alloc_context();
    fprintf(stderr, "pFormatCtx %p\n", pFormatCtx);
}

