//#include <cstdlib>
//#include <iostream>
//using namespace std;
//bitmap anal


#include<stdio.h>
#include<stdlib.h>
using namespace std;
#include"bmp.h"

void readbmp(char *);


int main(){
    readbmp("cylinder0000.bmp");
system("pause");
}

void readbmp(char *fname){
     FILE * bmp;
     struct bmpfile_magic mgnum;
     struct bmpfile_header bmphead;
     int segwidth;
     int segheight;
     int hsegfactor=30;
     int vsegfactor=10;
     BITMAPINFOHEADER bminfoh;
     bmp=fopen(fname,"rb");
     fread(&mgnum,sizeof(bmpfile_magic),1,bmp);
   	 fread(&bmphead, sizeof(struct bmpfile_header), 1, bmp);
     fread(&bminfoh, sizeof(BITMAPINFOHEADER),1,bmp);
     printf("width %ld\n height %ld",  bminfoh.width,bminfoh.height);
     segwidth=bminfoh.width/hsegfactor;
     segheight=bminfoh.height/vsegfactor;
     
     
     fclose(bmp);
}
