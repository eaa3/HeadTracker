#ifndef PREDATOR_H
#define PREDATOR_H

#include "includes.h"
#include "MedianFlowTracker.h"
#include "detector.h"
#include "tld_util.h"
#include "params.h"


namespace tld
{

class Predator
{

protected:

    Params* params;
public:

    MedianFlowTracker* tracker;
    FFClassifier* ffClassifier;
    NNClassifier* nnClassifier;
    Detector* detector;

    Mat currImg;
    Mat prevImg;
    Mat integralImg;

    BoundingBox prevBB;
    BoundingBox currBB;

    BoundingBox detectedBB;
    BoundingBox trackedBB;

    void storeData(); //done
    void fuseHypotheses();
    void learn();
    void initialLearning();


    bool initialized;

    int frameW, frameH;

public:

    bool trackerEnabled;
    bool detectorEnabled;
    bool learningEnabled;

    vector<BoundingBox> detectionHistory;



    Predator(string paramsFile); //done
    Predator(Params* params); //done
    virtual ~Predator(); //done

    void init(); //done
    void release(); // done
    void reset(); // done

    void selectObject(Mat& img, BoundingBox& bb); //done
    void processFrame(Mat& img); // done


};

}

#endif // PREDATOR_H
