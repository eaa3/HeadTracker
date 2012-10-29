#ifndef TWOBITBP_H
#define TWOBITBP_H

#include "feature.h"

namespace tld {

class TwoBitBP : public Feature
{
    public:
        TwoBitBP(float minScale, float maxScale);
        virtual ~TwoBitBP();


        virtual int evaluate(Mat& frame, BoundingBox box);


    protected:
    private:

};

}

#endif // TWOBITBP_H
