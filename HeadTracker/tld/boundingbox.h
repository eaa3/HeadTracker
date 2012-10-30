#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "includes.h"
#include "normalizedpatch.h"
//#include "tld_util.h"

namespace tld {

class NormalizedPatch;

class BoundingBox
{
public:
    int x, y, w, h;
    int xf, yf;
    float conf;
    float post;
    bool valid;

    bool detected;

    BoundingBox(int x = 0, int y = 0, int w = 0, int h = 0, float conf = 0.0f, bool valid = false, bool detected = false, float post = 0.0f);
    BoundingBox( const BoundingBox& other);
    BoundingBox( const Rect& other );

    void setX(int x);
    void setY(int y);
    void setXY(int x, int y);

    void setW(int w);
    void setH(int h);
    void setWH(int w, int h);

    bool isOverlapping(  BoundingBox& other );
    bool isOverlapping(  BoundingBox& other, float minOverlapArea );
    bool isInside(  BoundingBox& other );
	bool isInside( int x, int y);

    float overlap(  BoundingBox& other );
    void overlap(  vector<BoundingBox>& others, float* overlaps );
    void overlap(  vector<BoundingBox*>& others, float* overlaps );

    int getArea() ;
    int getArea( Mat& integralImage) ;

    Mat extractPatch( Mat& image);
    NormalizedPatch extractNormalizedPatch( Mat& image, int dimension = NORMALIZED_PATCH_DIM);

    virtual ~BoundingBox();

    operator Rect();
protected:
private:
};

}

#endif // BOUNDINGBOX_H
