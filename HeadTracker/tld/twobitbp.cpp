#include "twobitbp.h"

namespace tld {

TwoBitBP::TwoBitBP(float minScale, float maxScale) : Feature(minScale, maxScale)
{
    //ctor
}

TwoBitBP::~TwoBitBP()
{
    //dtor
}


int TwoBitBP::evaluate(Mat& integralImage, BoundingBox patch)
{
    int eval = 0;

    /*******************************************

            A------G------B
            |      |      |
            |      |      |
            E------|------F
            |      |      |
            |      |      |
            C------H------D


            AreaABCD = D - C - B + A

    ******************************************/

    int x = (int)(xp * float(patch.w)) + patch.x;
    int y = (int)(yp * float(patch.h)) + patch.y;
    int w = (int)((wp * float(patch.w)) * 0.5f);
    int h = (int)((hp * float(patch.h)) * 0.5f);
    int xf = x + w;
    int yf = y + h;


    int A = integralImage.at<int>(y, x);
    int B = integralImage.at<int>(y, xf);
    int C = integralImage.at<int>(yf, x);
    int D = integralImage.at<int>(yf, xf);
    int E = integralImage.at<int>(y + h/2, x);
    int F = integralImage.at<int>(y + h/2, xf);
    int G = integralImage.at<int>(y, x + w/2);
    int H = integralImage.at<int>(yf, x + w/2);

    int A1 = D - C - F + E;
    int A2 = F - E - B + A;

    if( A1 > A2 ) eval |= 1;

    eval <<= 1;

    int B1 = H - C - G + A;
    int B2 = D - H - B + G;

    if( B1 > B2 ) eval |= 1;

    return eval;
}

}

