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
           // printf("\n%s\n",outName);
            this->pieces[a].setName(outName);
	}
}		

void Frame::setPercent(int in){this->frameHeader.setSegPercent(in);setShitUp();}
//this must be adjusted so that setPercent happens first no matter what.
void Frame::setShitUp(){
		frameHeader.pixelsSegPrepInit();
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
		int piece[segH][segW];
		for(r=0;r<(this->sizeOfPixels[0]);r+=segH)
		   for(c=0;r<(this->sizeOfPixels[1]);r+=segW){
		      for(rp=0;rp<segH;rp++){
		         for(cp=0;cp<segW;cp++){
     			    piece[rp][cp]=0;
                 }
              } 
		    for(rp=0;(rp<segH)
				&&((rp+r*segH)<this->frameHeader.getHeight());rp++){
			   for(cp=0;(cp<segW)
				   &&((cp+c*segW)<this->sizeOfPixels[1]);cp++){
					piece[rp][cp]=this->pixels[r*segH+rp][c*segW+cp];
                    stamp("piece");printf("%d,%d::%d\n",rp,cp,piece[rp][cp]);
			    }
              }
for(r=0;r<segH;r++){
for(c=0;r<segW;c++)printf("%d  ",piece[r][c]);stamp("\n");}
hold();
            (this->pieces)[r*segH+c].setData((int**)piece,seg_dim);
            scream('*');
			}
	}
void Frame::writeAllSegments(){
		int a;
		int stop =this->frameHeader.number_of_segments(); 
		for(a=0;a<stop;a++) this->pieces[a].writeSegment();
	}
void Frame::instantiate_pixels(int rows,int cols){
     int r,c;
     this->pixels=(int **)calloc(rows, sizeof(int*));
     for(r=0;r<rows;r++) (this->pixels)[r]=(int *)calloc(cols, sizeof(int));
}
	
void Frame::getItTogether(){//not good
		int r,c,rp,cp,rows,cols;
	    rows=this->frameHeader.get_height();
	    cols=this->frameHeader.get_width()*this->frameHeader.get_bitspp()/32;
	    if(this->frameHeader.get_height()*this->frameHeader.get_bitspp()%32!=0) rows++;
	    rows*=4;
	    instantiate_pixels(rows,cols);
		for(r=0;r<rows;r+=this->frameHeader.getSegHeight())
		   for(c=0;c<cols;r+=this->frameHeader.getSegWidth()){
		      for(rp=0;(rp<this->frameHeader.getSegHeight())
				&&(rp+r*this->frameHeader.getSegHeight()<this->frameHeader.get_height());rp++)
			    for(cp=0;(cp<this->frameHeader.getSegWidth())
				   &&(cp+c*this->frameHeader.getSegWidth())<cols;cp++)
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
				fwrite(&(this->pixels[i][j]),sizeof(int),1,o);
	}
void Frame::writeBMP(){
//        char * name=
		FILE * out = fopen((this->FrameName += ".bmp").c_str(),"wb");
		this->frameHeader.writeout(out);
		this->writePixels(out);
		fclose(out);
		}
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%555
//one thing I'm doing here is actually just reading one byte of pixel info
//in at a time and storing each byte int the array.  Since bitmap pixel info
//is padded to 4 byte multiples anyway this will be ok for now.
void Frame::readBMP(string fName){
		int rows,cols,r,c;
	    FILE * in = NULL;
		this->FrameName= string(fName.substr(0,fName.length()-4));
		printf("name:%s", fName.c_str());
	    in = fopen(fName.c_str(),"rb");
		this->frameHeader.loadFrameInfo(in,0);	
		rows=this->frameHeader.get_height();
		cols=this->frameHeader.get_width()*this->frameHeader.get_bitspp()/32;
		if(this->frameHeader.get_height()*this->frameHeader.get_bitspp()%32!=0) rows++;
		rows*=4;
		instantiate_pixels(rows,cols);
		this->sizeOfPixels[0]=rows;
		this->sizeOfPixels[1]=cols;
		fseek(in,frameHeader.get_bmp_offset(),SEEK_SET);
		for(c=0;c<cols;c++){
    		for(r=0;r<rows;r++){
				fread(&(this->pixels[r][c]),sizeof(int),1,in);	
            }
        }
        fclose(in);
}

void Frame::readBusted(string headName){
		FILE * in = NULL;
		//this->frameHeader=new FrameInfoPlus();//again, unneccessary and improper
	    in = fopen(headName.c_str(),"rb");
			this->frameHeader.loadFrameInfo(in,1);
      		assembleSegments();
        fclose(in);
		
}

void Frame::printHeader(){this->frameHeader.printStuff();}
