#ifndef FFCLASSIFIER_H
#define FFCLASSIFIER_H


#include "fern.h"


namespace tld {

const float DEFAULT_THRESHOLD = 0.5f;

//Fern Forest Classifier
class FFClassifier
{
    public:

        vector<Fern*> ferns;
        float threshold;
        int nFerns, nNodes;
        float minScale, maxScale;


        FFClassifier(int nFerns, int nNodes, float minScale, float maxScale, float threshold = DEFAULT_THRESHOLD);
        virtual ~FFClassifier();


        void train( Mat& integralImage, BoundingBox& patch, int positive);

        float classify( Mat& integralImage,  BoundingBox& patch);

        bool filter(Mat& integralImage,  BoundingBox& patch);

        void init(int nFerns = 10, int nNodes = 5, float minScale = 0.1f, float maxScale = 0.5f);
        void release();

    protected:
    private:
};


}
#endif // FFCLASSIFIER_H
