#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<opencv2/opencv.hpp>
#include<iostream>
 
// Namespace to nullify use of cv::function(); syntax
using namespace std;
using namespace cv;
extern "C" {
 
  #include <libavcodec/avcodec.h>
  #include <libswscale/swscale.h>
  #include <libavutil/opt.h>
  #include <libavutil/imgutils.h>
  #include <libavformat/avformat.h>
}
static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
                   FILE *outfile)
{
    int ret;

    /* send the frame to the encoder */
    if (frame)
        printf("Send frame %3"PRId64"\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return;
        else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }

        printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

int main(int argc, char **argv)
{
    const char *filename, *codec_name;
    const AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret, x, y;
    FILE *f;
    AVFrame *frame;
    AVPacket *pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    if (argc <= -1) {
        fprintf(stderr, "Usage: %s <output file> <codec name>\n", "test");
        exit(0);
    }
    filename = "output.avi";
    codec_name = "libx265";

    /* find the mpeg1video encoder */
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) {
        fprintf(stderr, "Codec '%s' not found\n", codec_name);
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    pkt = av_packet_alloc();
    if (!pkt)
        exit(1);

    /* put sample parameters */
    c->bit_rate = 400000;
    /* resolution must be a multiple of two */
    c->width = 640;
    c->height = 360;
    /* frames per second */
    c->time_base = (AVRational){1, 30};
    c->framerate = (AVRational){30, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "fast", 0);

    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec: %s\n", "error incomprésenible");
        exit(1);
    }

    f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate the video frame data\n");
        exit(1);
    }
    
    VideoCapture vid_capture("videoplayback.mp4");
    // VideoCapture vid_capture(0);
    Mat opencv_frame;
    /* encode 1 second of video */
    for (i = 0; i < 200; i++) {
      
        fflush(stdout);
        
        
        /* Make sure the frame data is writable.
           On the first round, the frame is fresh from av_frame_get_buffer()
           and therefore we know it is writable.
           But on the next rounds, encode() will have called
           avcodec_send_frame(), and the codec may have kept a reference to
           the frame in its internal structures, that makes the frame
           unwritable.
           av_frame_make_writable() checks that and allocates a new buffer
           for the frame only if necessary.
         */
        
        
        ret = av_frame_make_writable(frame);
        if (ret < 0)
            exit(1);






            // Allocate memory for the RGB image
    uint8_t* rgbData = new uint8_t[640 * 360 * 3]; // 3 bytes per pixel (RGB)

   
        


        /* Prepare a dummy image.
           In real code, this is where you would have your own logic for
           filling the frame. FFmpeg does not care what you put in the
           frame.
         */
        /* Y */
      /*
        vid_capture.read(opencv_frame);
        for (y = 0; y < c->height; y++) {
            for (x = 0; x < c->width; x++) {
                frame->data[y][x] = opencv_frame.data[c->height * y + x];
            }
        }
*/
        vid_capture.read(opencv_frame); 
        Mat newframe;
        cvtColor(opencv_frame, newframe, COLOR_BGR2YUV_I420, 0);
        // waitKey(20);
        uint8_t* pixelPtr = (uint8_t*)opencv_frame.data;
        int cn = opencv_frame.channels();
        // imshow("Frame", opencv_frame);
        



        for (y = 0; y < 360; y++) { // width 640
            for (x = 0; x < 640; x++) { // height 360  
    
                Vec3b v = opencv_frame.at<Vec3b>(y, x);

                int r = v[0];
                int g = v[1];
                int b = v[2];
                frame->data[0][y * frame->linesize[0] + x] = g;
                frame->data[1][y/2 * frame->linesize[1] + x/2] = b;
                frame->data[2][y/2 * frame->linesize[2] + x/2] = r;

                // frame->data[0][y * frame->linesize[0] + x] = y;
                // frame->data[1][y * frame->linesize[0] + x] = u;
                // frame->data[2][y * frame->linesize[2] + x] = v_d;
                // frame->data[1][y * frame->linesize[1] + x] = pixelPtr[x*opencv_frame.cols*cn + y*cn + 1];
                // frame->data[2][y * frame->linesize[2] + x] = pixelPtr[x*opencv_frame.cols*cn + y*cn + 2];
            }
        }
        // printf("finished image\n");
        // TODO here frame -> data request YUV420 pixel format, not RGB reduce it to this type







        /* Cb and Cr */
        for (y = 0; y < c->height/2; y++) {
            for (x = 0; x < c->width/2; x++) {
                //frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                // frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

    
        frame->pts = i;

        /* encode the image */
        encode(c, frame, pkt, f);

    }

    /* flush the encoder */
    encode(c, NULL, pkt, f);

    /* Add sequence end code to have a real MPEG file.
       It makes only sense because this tiny examples writes packets
       directly. This is called "elementary stream" and only works for some
       codecs. To create a valid file, you usually need to write packets
       into a proper file format or protocol; see mux.c.
     */
    if (codec->id == AV_CODEC_ID_MPEG1VIDEO || codec->id == AV_CODEC_ID_MPEG2VIDEO)
        fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_free_context(&c);
    av_frame_free(&frame);
    av_packet_free(&pkt);
    

 // Release allocated resources
   


    vid_capture.release();
    return 0;
}