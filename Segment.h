//Segment.h
//Joshua Suggs
//January 2012
//no stealing!
#ifndef _SEGMENT_H_
#define _SEGMENT_H_
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;
class Segment{
      
       public:
			Segment(string name);
			Segment(string name, int ** inD,int *);
			int getInt(int a);
			void readSegment(int numR, int numC);
			void writeSegment();
			void setName(char * inName);
			char * giveName();
			void setData(int ** inD,int *dim);//dim[0]=rows,dim[1]=cols
			int dataLength();
      private:
              
			int * data;
			char * segName;
};
#endif
