#include "detector.h"


namespace tld {

Detector::Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier, float minScale, float maxScale, float baseScale,  float shift, int min_bb, float cutoff)
{
    this->frameW = frameW;
    this->frameH = frameH;



    this->ffClassifier = ffClassifier;
    this->nnClassifier = nnClassifier;

    this->clusterer = new Clusterer(cutoff);

    this->minScale = minScale;
    this->maxScale = maxScale;
    this->baseScale = baseScale;

    this->shift = shift;
    this->min_bb = min_bb;

}

Detector::~Detector()
{
    this->release();
}


void Detector::release()
{
    delete this->clusterer;

    this->initialized = false;

}

void Detector::init()
{
    this->initWindows();

    this->initialized = true;
}



void Detector::initWindows()
{

    float currentScale = 0.0f;

    int shiftW = 0;
    float shiftH = 0;

    BoundingBox windowFrame(1,1, frameW - 1, frameH - 1);
    BoundingBox currentBB;



    //Initializing windows and scales
    for(int i = minScale; i <= maxScale; i++ )
    {
        currentScale = powf(baseScale, i);

        currentBB.setWH(this->objW*currentScale, this->objH*currentScale);

        if( currentBB.w < min_bb || currentBB.h < min_bb ) continue;

        //float s1 = (windowFrame.w - currentBB.w)/20.0f;
        //float s2 = (windowFrame.h - currentBB.h)/20.0f;

        //shiftW = max(int(ceil(s1)), 5);
        //shiftH = max(int(ceil(s2)), 5);

        shiftW = floor(shift*min(currentBB.w, currentBB.h) + 0.5f);
        shiftH = floor(shift*currentBB.h + 0.5f);



        for(int j = windowFrame.x; (j + currentBB.w) < windowFrame.xf; j += shiftW )
        {
            for(int k = windowFrame.y; (k + currentBB.h) < windowFrame.yf; k += shiftH)
            {

                currentBB.setXY(j,k);

                if( !currentBB.isInside(windowFrame))
                {
                    continue;
                }

                this->windows.push_back(currentBB);


            }
        }

    }

    printf("Windiws has been generated! sizeWindows %d\n", this->windows.size());
    waitKey(1000);




}



BoundingBox Detector::detect(Mat& img, Mat& integralImage)
{

    if( !initialized ) return BoundingBox();

    vector<BoundingBox*> confidentWindows;

    for(int i = 0; i < this->windows.size(); i++)
    {

        if( !this->ffClassifier->filter(integralImage, this->windows[i]) )
        {
            continue;
        }

        NormalizedPatch npatch = this->windows[i].extractNormalizedPatch(img);

        if( !this->nnClassifier->filter(npatch) )
        {
            continue;
        }


        confidentWindows.push_back(&this->windows[i]);

    }

    return this->clusterer->cluster(confidentWindows);
}


}
