//Segment.cpp
//Joshua Suggs
//January 2012
//don't remove this header
#include "Segment.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

		Segment::Segment(string name){
            this->setName((char *)name.c_str());
		}
		
		Segment::Segment(string name, int ** inD,int * dim){
			this->setName((char *)name.c_str());
			this->setData(inD,dim);
		}
		int Segment::getInt(int a){return data[a];}
		void Segment::readSegment(int numR, int numC){
				int a,r=numR;
				int b,c=numC;
				FILE * in = NULL;
			        in = fopen(segName,"rb");
				for(a=0;a<r;a++){
					for(b=0;b<c;b++)
				                fread(&(this->data[a*c+b]),sizeof(int),1,in);
					}
			}
        int Segment::dataLength(){
            int a;
            for(a=0;this->data[a]!='\0';a++);
            return a;
        }			
		void Segment::writeSegment(){
				int a,d=this->dataLength();
				FILE * out = NULL;
		        out = fopen(segName,"wb");
				for(a=0;a<d;a++){
                        fwrite(&(this->data[a]), sizeof(int),1,out);         
				}fclose(out);
		}
		void Segment::setName(char * inName){
				this->segName = (char *)calloc(strlen(inName)+1,sizeof(char));
				strcpy(this->segName,inName);
        }
   		char * Segment::giveName(){
             char * ret= (char *)calloc(strlen(this->segName)+1,sizeof(char));
             strcpy(ret,this->segName);
             return ret;
        }
		void Segment::setData(int ** inD,int * dim){
				int r,c;
				this->data = (int *)calloc (dim[0]*dim[1],sizeof (int));
				for(r=0;r<dim[0];r++){
					for(c=0;c<dim[1];c++){
                  
						this->data[r*dim[1]+c]=inD[r][c];
                    }
                }
                this->length=dim[0]*dim[1];
		}


