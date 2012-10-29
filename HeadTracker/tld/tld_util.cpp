#include "tld_util.h"



float randomFloat()
{

    return ((float)rand())/((float)RAND_MAX);
}

double randomDouble()
{
    return ((double)rand())/((double)RAND_MAX);
}

int randomInt(int from, int to)
{
    return rand()%(to - from + 1) + from;
}


Mat get_roi(Mat& src, BoundingBox box, int padding)
{
    Range rowRange = Range(box.y + padding,box.yf - padding);
    Range colRange = Range(box.x + padding,box.xf - padding);

    Mat roi, temp;

    if( rowRange.size() > 0 && colRange.size() > 0 )
    {
        //temp = Mat(src, rowRange, colRange );
        //temp.copyTo(roi);
        roi = src(rowRange, colRange).clone();
        //cvtColor(roi, roi, CV_BGR2GRAY);
    }

    return roi;
}

bool gtCompf( float& a,  float& b)
{
    return a > b;
}


bool tldSortByOverlapDesc(pair<int,float> bb1 , pair<int,float> bb2) {
	return bb1.second > bb2.second;
}
