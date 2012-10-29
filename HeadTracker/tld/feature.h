#ifndef FEATURE_H
#define FEATURE_H

#include "includes.h"
#include "boundingbox.h"
#include "tld_util.h"

namespace tld {

class Feature
{
    public:
        float xp, yp, wp, hp;

        Feature();
        Feature(float minScale, float maxScale);
        virtual ~Feature();




        virtual int evaluate(Mat& integralImage, BoundingBox patch) = 0;

    protected:
    private:
};

}
#endif // FEATURE_H
