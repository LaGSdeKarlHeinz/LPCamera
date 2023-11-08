extern "C" {
 
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
  #include <libavutil/opt.h>
  #include <libavutil/imgutils.h>
  #include <libavformat/avformat.h>
}

#include<opencv2/opencv.hpp>
using namespace cv;


void frame2mat(AVFrame *frame, Mat *matrix) {
    // Mat matrix = Mat(frame->height,frame->width, CV_8UC1);
 
    for (int y = 0; y < frame->height; y++) { // width 640
            for (int x = 0; x < frame->width; x++) { // height 360
                
                float Y = frame->data[0][y * frame->linesize[0] + x];
                float Cb = frame->data[1][y/2 * frame->linesize[1] + x/2];
                float Cr = frame->data[2][y/2 * frame->linesize[2] + x/2];
                // printf("color = %d", Y);
                int r = Y + 1.402*(Cr - 128);
                int g = Y - 0.34414*(Cb-128) - 0.71414*(Cr - 128);
                int b = Y + 1.772*(Cb - 128);
                Vec3b v = Vec3b(b,g,r);
                // printf("x = %d", x);
                //printf("y = %d", y);
                matrix->at<Vec3b>(y, x) = v;
        }
    }
}