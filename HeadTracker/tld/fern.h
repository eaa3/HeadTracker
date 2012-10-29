#ifndef FERN_H
#define FERN_H

#include "includes.h"
#include "twobitbp.h"


namespace tld {

class Fern
{
    protected:
    private:
    public:
        vector<TwoBitBP*> node;

        //Number of leafs of this fern
        int nLeafs;

        vector<int> p;
        vector<int> n;

        vector<float> posterior;


        Fern(int nNodes, float minScale, float maxScale);
        virtual ~Fern();


        int leafIndex(Mat& integralImage, BoundingBox patch );

        void train( Mat& integralImage, BoundingBox& patch, int patchLabel);

        float classify( Mat& integralImage,  BoundingBox& patch);

};

}

#endif // FERN_H
