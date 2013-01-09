#ifndef __ADABOOST_DETECTOR_H__
#define __ADABOOST_DETECTOR_H__

#include <opencv/cv.h>

#include "Features.h"
#include "Target.h"

using namespace std;

class AdaboostDetector
{
public:
  static AdaboostDetector* I();

  bool detect(const cv::Mat& currGray, std::vector<cv::Rect>& targets, TargetType type, double scale = 1)
  {
    bool detected = false;
    
    unsigned int i = 0;
    double t = 0;

    cv::Mat smallImg(cvRound (currGray.rows/scale), cvRound(currGray.cols/scale), CV_8UC1);
    resize(currGray, smallImg, smallImg.size(), 0, 0, CV_INTER_LINEAR);
    equalizeHist(smallImg, smallImg);
    
    if(type == HAND) cascadeHand.detectMultiScale(smallImg, targets, 1.2, 2, 0|CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(40, 40));
    if(type == FACE) cascadeFace.detectMultiScale(smallImg, targets, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
    if(type == BODY) cascadeBody.detectMultiScale(smallImg, targets, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    /*|CV_HAAR_DO_ROUGH_SEARCH
    //|CV_HAAR_DO_CANNY_PRUNING
    //|CV_HAAR_SCALE_IMAGE*/

    return detected;
  }

private:
  static AdaboostDetector* instance;

  cv::CascadeClassifier cascadeFace, cascadeHand, cascadeBody;

  AdaboostDetector()
  {
    if(!cascadeFace.load("../Resources/HAFT/Training/haarcascades/haarcascade_frontalface_alt.xml") ||
       !cascadeHand.load("../Resources/HAFT/Training/haarcascades/haarcascade_closedhand.xml") ||
       !cascadeBody.load("../Resources/HAFT/Training/haarcascades/haarcascade_upperbody.xml"))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        cerr << "Usage: facedetect [--cascade=<cascade_path>]\n"
            "   [--nested-cascade[=nested_cascade_path]]\n"
            "   [--scale[=<image scale>\n"
            "   [filename|camera_index]\n" << endl;
        exit(0);
    }
  }
  AdaboostDetector(AdaboostDetector const&);            //hiding the copy constructor
  AdaboostDetector& operator=(AdaboostDetector const&); //hiding the assignment operator
};

AdaboostDetector* AdaboostDetector::instance = 0;
AdaboostDetector* AdaboostDetector::I()
{
  if(instance == 0)
  {
    instance = new AdaboostDetector();
  }

  return instance;
}

#endif//__ADABOOST_DETECTOR_H__