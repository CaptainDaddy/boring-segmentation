//frame.h

#ifndef _FRAMEINFOPLUS_H_
#define _FRAMEINFOPLUS_H_
#include <stdio.h>
#include <stdint.h>
#include "bmp.h"
#include "debug.h"
#include <iostream>
using namespace std;

class FrameInfoPlus{
      public:
            FrameInfoPlus();
            void loadFrameInfo(FILE * fin,int flag);
	        void writeout(FILE * o);
           	void printStuff();
            int segsInit();
           	int getSegWidth();
           	int getSegHeight();
           	int getHeight();
       	    int number_of_segments();
            void setSegmentHW();
            void pixelsSegPrepInit();
            void setSegPercent(int);
            uint32_t get_filesz();
            uint16_t get_creator1();
            uint16_t get_creator2();
            uint32_t get_bmp_offset();
            uint32_t get_header_sz();
            int32_t get_width();
            int32_t get_height();
            uint16_t get_nplanes();
            uint16_t get_bitspp();
            uint32_t get_compress_type();
            uint32_t get_bmp_bytesz();
            int32_t get_hres();
            int32_t get_vres();
            uint32_t get_ncolors();
            uint32_t get_nimpcolors();
      private:
            void compileIn(struct bmpfile_magic,
                                     struct bmpfile_header,
                                     BITMAPINFOHEADER);
            unsigned char magic[2];
            uint32_t filesz;
            uint16_t creator1;
            uint16_t creator2;
            uint32_t bmp_offset;
            uint32_t header_sz;
            int32_t width;
            int32_t height;
            uint16_t nplanes;
            uint16_t bitspp;
            uint32_t compress_type;
            uint32_t bmp_bytesz;
            int32_t hres;
            int32_t vres;
            uint32_t ncolors;
            uint32_t nimpcolors;
            //$$$
            int segWidth;
            int segHeight;
            //these values below will be set by driver/methods and used for operations but not stored in a file
            int segPercent;
            int cbyte;
  //int number_of_segments; 
};
#endif
