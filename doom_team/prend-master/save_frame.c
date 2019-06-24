//#include "player.h"
//
//SDL_Surface* SaveFrame1(SDL_Surface* surface)
//{
//    return;
//    char Buf[512];
//    sprintf(Buf, "ffmpeg -an -f rawvideo -pix_fmt bgr0 -s %ux%u -r 60 -i - -aspect %u/%u -c:v h264 -crf 2 -preset fast -y file1.avi", W2,H, W2,H);
//
//    static FILE* fp = NULL;
//    if(!fp) {
//        fp = /*fopen("file1.bin", "wb");*/ popen(Buf, "w");
//    }
//    fwrite(surface->pixels, W2*H, 4, fp);
//    fflush(fp);
//}
//
//SDL_Surface* SaveFrame2(SDL_Surface* surface)
//{
//    return;
//    //static unsigned skip=0;
//    //if(++skip>=3) { skip=0; } else return;
//
//    char Buf[512];
//    sprintf(Buf, "ffmpeg -an -f rawvideo -pix_fmt bgr0 -s %ux%u -r 60 -i - -aspect %u/%u -c:v h264 -crf 2 -preset fast -y file2.avi", W2,H, W2,H);
//
//    static FILE* fp = NULL;
//    if(!fp) { fp = /*fopen("file2.bin", "wb");*/ popen(Buf, "w"); }
//    fwrite(surface->pixels, W2*H, 4, fp);
//    fflush(fp);
//}