//JoshuaMath.cpp
//January 2012
//no stealing!

#include "JoshuaMath.h"

int roundUpDiv(int n, int d){
		if (n%d!=0) return n/d+1;
		return n/d;
}
