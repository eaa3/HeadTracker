#include "ffclassifier.h"

namespace tld
{

FFClassifier::FFClassifier(int nFerns, int nNodes, float minScale, float maxScale, float threshold)
{
    this->nFerns = nFerns;
    this->nNodes = nNodes;
    this->minScale = minScale;
    this->maxScale = maxScale;
    this->threshold = threshold;

    this->init(nFerns, nNodes, minScale, maxScale);

}

FFClassifier::~FFClassifier()
{
    this->release();
}


void FFClassifier::init(int nFerns, int nNodes, float minScale, float maxScale)
{
    this->ferns.resize(nFerns);

    for(int i = 0; i < this->ferns.size(); i++) this->ferns[i] = new Fern(nNodes, minScale, maxScale);
}

void FFClassifier::release()
{
    for(int i = 0; i < this->ferns.size(); i++) delete ferns[i];

    this->ferns.clear();

}



void FFClassifier::train( Mat& integralImage,  BoundingBox& patch, int positive)
{
    float post = 0.0f;

    post = classify(integralImage, patch);

    if( (positive && post <= this->threshold) || (!positive && post >= threshold))
    {
        for(int i = 0; i < this->ferns.size(); i++)
        {
            this->ferns[i]->train(integralImage, patch, positive);
        }
    }

}

float FFClassifier::classify( Mat& integralImage,  BoundingBox& patch)
{
    float sum = 0.0f;

    for(int i = 0; i < this->ferns.size(); i++)
    {
        sum += this->ferns[i]->classify(integralImage, patch);
    }

    return sum/float(this->ferns.size());
}

bool FFClassifier::filter(Mat& integralImage,  BoundingBox& patch)
{
    float post = classify(integralImage, patch);

    patch.post = post;

    return post >= this->threshold;
}

}
