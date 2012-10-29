#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "includes.h"
#include "boundingbox.h"
#include "ffclassifier.h"
#include "nnclassifier.h"
#include "clusterer.h"

using namespace std;


namespace tld {


class Detector
{
    public:

        FFClassifier* ffClassifier;
        NNClassifier* nnClassifier;
        Clusterer* clusterer;

        int frameW, frameH;
        float objW, objH;
        vector<BoundingBox> windows;

        float minScale, maxScale, baseScale;
        float shift;
        int min_bb;


        bool initialized;

        void initWindows();

        Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier = NULL);

        Detector(int frameW, int frameH, FFClassifier* ffClassifier, NNClassifier* nnClassifier = NULL, float minScale = MIN_WINDOW_SCALE, float maxScale = MAX_WINDOW_SCALE, float baseScale = BASE_WINDOW_SCALE,  float shift = SHIFT, int min_bb = MIN_BB, float cutoff = CUTOFF);
        virtual ~Detector();

        void init();
        void release();


        BoundingBox detect(Mat& img, Mat& integralImage);
    protected:
    private:
};

}

#endif // DETECTOR_H
