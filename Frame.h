//Frame.h

#ifndef _FRAME_H_
#define _FRAME_H_
#include "FrameInfoPlus.h"
#include "Segment.h"
#include "debug.h"
#include<math.h>
#include <iostream>
using namespace std;
class Frame {

    public:
           
	Frame(string inName);
	void setPercent(int in);
	void splitToPieces();
	void readAllSegments();
	void assembleSegments();
	void writePixels(FILE *);
	void writeBMP();
    void writeBusted();
	void readBMP(string fName);
	void readBusted(string headName);
	void getItTogether();
    void printHeader();
    void setShitUp();
    void print_surface();
    private:
            
	string FrameName;
	int ** pixels;
	int sizeOfPixels[2];//rows cols  as in 'pixels'
	Segment * pieces;
	int number_of_segments;
	FrameInfoPlus frameHeader;
	void setSegHW();
	void segsInit(string);
    void instantiate_pixels(int rows,int cols);
	void writeAllSegments();
};
#endif
