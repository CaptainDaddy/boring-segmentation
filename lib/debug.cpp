//debug.cpp
#include "debug.h"
void scream(char i){int a;for(a=0;a<999;a++)printf("%c",i);printf("\n");} 
void scream_int(int i){int a;for(a=0;a<999;a++)printf("%d::",i);printf("\n");} 
void stamp(char * in){printf("%s",in);}
void stamp_int(int in){printf("%d",in);}
void hold(){system("pause");}
