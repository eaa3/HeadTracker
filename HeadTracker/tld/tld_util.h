#ifndef _UTIL_
#define _UTIL_

#include "includes.h"
#include  "boundingbox.h"

using tld::BoundingBox;

float randomFloat();
double randomDouble();
int randomInt();

bool gtCompf( float& a,  float& b);
bool tldSortByOverlapDesc(pair<int,float> bb1 , pair<int,float> bb2);

Mat get_roi(Mat& src, BoundingBox box, int padding = 0);


#endif
