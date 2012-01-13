//FrameInfoPlus.cpp
//Joshua Suggs
//January 2012
#include "JoshuaMath.h"
#include "FrameInfoPlus.h"
#include <stdio.h>

FrameInfoPlus::FrameInfoPlus(){}
uint32_t FrameInfoPlus::get_filesz(){return this->filesz;}
uint16_t FrameInfoPlus::get_creator1(){return this->creator1;}
uint16_t FrameInfoPlus::get_creator2(){return this->creator2;}
uint32_t FrameInfoPlus::get_bmp_offset(){return this->bmp_offset;}
uint32_t FrameInfoPlus::get_header_sz(){return this->header_sz;}
int32_t FrameInfoPlus::get_width(){return this->width;}
int32_t FrameInfoPlus::get_height(){return this->height;}
uint16_t FrameInfoPlus::get_nplanes(){return this->nplanes;}
uint16_t FrameInfoPlus::get_bitspp(){return this->bitspp;}
uint32_t FrameInfoPlus::get_compress_type(){return this->compress_type;}
uint32_t FrameInfoPlus::get_bmp_bytesz(){return this->bmp_bytesz;}
int32_t FrameInfoPlus::get_hres(){return this->hres;}
int32_t FrameInfoPlus::get_vres(){return this->vres;}
uint32_t FrameInfoPlus::get_ncolors(){return this-> ncolors;}
uint32_t FrameInfoPlus::get_nimpcolors(){return this->nimpcolors;}
int FrameInfoPlus::getSegWidth(){return this->segWidth;}
int FrameInfoPlus::getSegHeight(){return this->segHeight;}
int FrameInfoPlus::getHeight(){return (this->height);}
void FrameInfoPlus::setSegmentHW(){
	this->segWidth=roundUpDiv(this->width*this->segPercent,100);
    this->segHeight=roundUpDiv(this->height*this->segPercent,100);
}
void FrameInfoPlus::pixelsSegPrepInit(){//takes information about the pixel data and sets information in the header
		this->cbyte=(this->width*this->bitspp/32);
		if(!(this->width * this-> bitspp % 32==0))
			this-> cbyte++;
		this-> cbyte*=1;//32 bit pixel representer for now
	}
int FrameInfoPlus::segsInit(){
		this->printStuff();
		int h=roundUpDiv(this-> height,this-> segHeight);
		int w=roundUpDiv(this-> cbyte,this-> segWidth);
        return(h*w);
}		
void FrameInfoPlus::loadFrameInfo(FILE * fin,int flag){
      struct bmpfile_magic mgnum;
      struct bmpfile_header bmphead;
      BITMAPINFOHEADER bminfoh;
      if(fin!=NULL){                            
        fread(&mgnum,sizeof(bmpfile_magic),1,fin);
        fread(&bmphead, sizeof(struct bmpfile_header), 1, fin);
        fread(&bminfoh, sizeof(BITMAPINFOHEADER),1,fin);
      }
      this->compileIn(mgnum,bmphead,bminfoh);      
      if(flag==1){
          fread(&(this->segWidth), sizeof(int),1,fin);
          fread(&(this->segHeight), sizeof(int),1,fin);
      }    
}
void FrameInfoPlus::setSegPercent(int in){this->segPercent=in;}
void FrameInfoPlus::compileIn(struct bmpfile_magic mnm,
                                     struct bmpfile_header bhd,
                                     BITMAPINFOHEADER bfh){
      this->magic[0]=mnm.magic[0];
      this->magic[1]=mnm.magic[1];
      this->filesz=bhd.filesz;
      this->creator1=bhd.creator1;
      this->creator2=bhd.creator2;
      this->bmp_offset=bhd.bmp_offset;
      this->header_sz=bfh.header_sz;
      this->width=bfh.width;
      this->height=bfh.height;
      this->nplanes=bfh.nplanes;
      this->bitspp=bfh.bitspp;
      this->compress_type=bfh.compress_type;
      this->bmp_bytesz=bfh.bmp_bytesz;
      this->hres=bfh.hres;
      this->vres=bfh.vres;
      this->ncolors=bfh.ncolors;
      this->nimpcolors=bfh.nimpcolors;
}

void FrameInfoPlus::writeout(FILE * o){
     frameInfo out_struct;
      out_struct.magic[0]=this->magic[0];
      out_struct.magic[1]=this->magic[1];
      out_struct.filesz=this->filesz;
      out_struct.creator1=this->creator1;
      out_struct.creator2=this->creator2;
      out_struct.bmp_offset=this->bmp_offset;
      out_struct.header_sz=this->header_sz;
      out_struct.width=this->width;
      out_struct.height=this->height;
      out_struct.nplanes=this->nplanes;
      out_struct.bitspp=this->bitspp;
      out_struct.compress_type=this->compress_type;
      out_struct.bmp_bytesz=this->bmp_bytesz;
      out_struct.hres=this->hres;
      out_struct.vres=this->vres;
      out_struct.ncolors=this->ncolors;
      out_struct.nimpcolors=this->nimpcolors;   
     fwrite((frameInfo *)(&(out_struct)), sizeof( frameInfo), 1,o);
	}
void FrameInfoPlus::printStuff(){
//only the stuff directly from the bitmap
			printf("magic:[ %d , %d ]\n",this-> magic[0],this-> magic[1]);
			printf("file size:%d\n",this-> filesz);
			printf("creator 1:%d\n",this-> creator1);
			printf("creator 1:%d\n",this-> creator2);
			printf("offset:%d\n",this-> bmp_offset);
			printf("header_sz:%d\n",this-> header_sz);
			printf("width:%d\n",this-> width);
			printf("height:%d\n",this-> height);
			printf("nplanes:%d\n",this-> nplanes);
			printf("bitspp:%d\n",this-> bitspp);
			printf("compress_type:%d\n",this-> compress_type);
			printf("bmp_bytesz:%d\n",this-> bmp_bytesz);
			printf("hres:%d\n",this-> hres);
			printf("vres:%d\n",this-> vres);
			printf("ncolors:%d\n",this-> ncolors);
			printf("nimpcolors:%d\n",this-> nimpcolors);
}
int FrameInfoPlus::number_of_segments(){
     return roundUpDiv((int)this->height,(int)this->segHeight)*roundUpDiv((int)this->width,(int)this->segWidth); 
}
