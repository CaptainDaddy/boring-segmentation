//Frame.cpp
//Joshua Suggs
//January 2012
//if used or altered, my name and the date must remain

#include "Frame.h"
#include <iostream>
using namespace std;

//filename format
//movieName.FrameXXXXXXXXX.bmp
//
//movieName.FrameXXXXXXXXX.head
//movieName.FrameXXXXXXXXX.SegmentXXXXXXXXX


Frame::Frame(string inName){
		if(inName[(inName.length()-1)]=='p') this->readBMP(inName);
		else readBusted(inName);
		this->frameHeader.printStuff();
}
	
void Frame::setSegHW(){
     this->frameHeader.setSegmentHW();
	}
void Frame::segsInit(string frameN){
	int a;
    char * outName = (char *)calloc(18+frameN.length(),sizeof(char));
    this->number_of_segments=this->frameHeader.segsInit();
	this->setShitUp();
	this->pieces=(Segment *)calloc(this->number_of_segments,sizeof(Segment));
	for(a=0;a<this->number_of_segments;a++){
			sprintf(outName,"%s.Segment%09d",frameN.c_str(), a);//put  the number on the end
            this->pieces[a].setName(outName);
	}
}		

void Frame::print_surface(){
     int r,c;
     for(r=0;r<this->frameHeader.get_height();r++){
        for(c=0;c<frameHeader.get_width();c++)printf("%8d ",(pixels[r][c]));stamp("\n");stamp_int(c);}
}
void Frame::setPercent(int in){this->frameHeader.setSegPercent(in);setShitUp();}
//this must be adjusted so that setPercent happens first no matter what.
void Frame::setShitUp(){
		setSegHW();
	}

void Frame::splitToPieces(){
		segsInit(this->FrameName);
		int r,c,rp,cp;
        int seg_dim[2];
		int segH=this->frameHeader.getSegHeight();
		int segW=this->frameHeader.getSegWidth();
        seg_dim[0]=segH;
        seg_dim[1]=segW;
		int **piece= new int *[segH];
        for (r=0;r<segH;r++) piece[r]=new int [segW];
        
		for(r=0;r<(this->sizeOfPixels[0]);r+=segH)
		   for(c=0;c<(this->sizeOfPixels[1]);c+=segW){
		      for(rp=0;rp<segH;rp++){
		         for(cp=0;cp<segW;cp++){
     			    piece[rp][cp]=0;
                 }
              }
            
		      for(rp=0;(rp<segH)&&((rp+r)<this->frameHeader.getHeight());rp++){
			     for(cp=0;(cp<segW)&&((cp+c)<this->sizeOfPixels[1]);cp++){
  				    piece[rp][cp]=this->pixels[r+rp][c+cp];
			    }
              }
              (this->pieces)[(r/segH)*(int)sqrt(this->number_of_segments)+c/segW].setData((int**)piece,seg_dim);
           }
	}
void Frame::writeAllSegments(){
		int a;
		int stop =this->frameHeader.number_of_segments(); 
		for(a=0;a<stop;a++)((this->pieces)[a]).writeSegment();
	}
void Frame::instantiate_pixels(int rows,int cols){
     int r,c;
     this->pixels=(int **)calloc(rows, sizeof(int*));
     for(r=0;r<rows;r++) (this->pixels)[r]=(int *)calloc(cols, sizeof(int));
}
void Frame::writeBusted(){
	    FILE * out = fopen((this->FrameName += ".head").c_str(),"wb");	
	    this->frameHeader.writeout(out,1);
        fclose(out);
        writeAllSegments();
}   
void Frame::getItTogether(){//not good?
		int r,c,rp,cp,rows,cols;
	    rows=this->frameHeader.get_height();
	    cols=this->frameHeader.get_width()*this->frameHeader.get_bitspp()/32;
	    if(this->frameHeader.get_height()*this->frameHeader.get_bitspp()%32!=0) rows++;
	    rows*=4;
	    instantiate_pixels(rows,cols);
		for(r=0;r<rows;r+=this->frameHeader.getSegHeight())
		   for(c=0;c<cols;r+=this->frameHeader.getSegWidth()){
		      for(rp=0;(rp<this->frameHeader.getSegHeight())
				&&(rp+r<this->frameHeader.get_height());rp++)
			    for(cp=0;(cp<this->frameHeader.getSegWidth())
				   &&(cp+c)<cols;cp++)
			       this->pixels[r+rp][c+cp] = (this->pieces[r*this->frameHeader.getSegHeight()+c]).getInt(rp*this->frameHeader.getSegWidth()+cp);
			}		
	}
void Frame::readAllSegments(){
		int a;
		for(a=0;a<this->number_of_segments;a++)  this->pieces[a].readSegment(this->frameHeader.getSegHeight(),this->frameHeader.getSegWidth());//this should do it
	}		
void Frame::assembleSegments(){
		this->setShitUp();
		segsInit(this->FrameName);
		readAllSegments();
	}
void Frame::writePixels(FILE * o){
		int i,j,r,c;
		r = this->sizeOfPixels[0];
		c= this->sizeOfPixels[1];
		for(i=0;i<r;i++)
			for(j=0;j<c;j++)
				fwrite(&(this->pixels[i][j]),(this->frameHeader.get_bitspp())/8,1,o);
	}
void Frame::writeBMP(){
//        char * name=
		FILE * out = fopen((this->FrameName += ".bmp").c_str(),"wb");
		this->frameHeader.writeout(out,0);
		this->writePixels(out);
		fclose(out);
		}
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%555
void Frame::readBMP(string fName){
		int rows,cols,r,c;
	    FILE * in = NULL;
		this->FrameName= string(fName.substr(0,fName.length()-4));
	    in = fopen(fName.c_str(),"rb");
		this->frameHeader.loadFrameInfo(in,0);	
		rows=this->frameHeader.get_height();
		cols=this->frameHeader.get_width();
		instantiate_pixels(rows,cols);
		this->sizeOfPixels[0]=rows;
		this->sizeOfPixels[1]=cols;
		fseek(in,frameHeader.get_bmp_offset(),SEEK_SET);
   		for(r=0;r<rows;r++){
     		for(c=0;c<cols;c++){
				fread(&(this->pixels[r][c]),(this->frameHeader.get_bitspp())/8,1,in);
            }
        }
        fclose(in);
}

void Frame::readBusted(string headName){
		FILE * in = NULL;
		//this->frameHeader=new FrameInfoPlus();//again, unneccessary and improper
	    in = fopen(headName.c_str(),"rb");
		this->frameHeader.loadFrameInfo(in,1);
scream('*');
      	assembleSegments();
        fclose(in);
		
}

void Frame::printHeader(){this->frameHeader.printStuff();}
