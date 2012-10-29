#ifndef _INCLUDES_
#define _INCLUDE_


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <windows.h>
//#include "MedianFlowTracker.h"


using namespace cv;
using namespace std;
//using namespace tld;

//Fern Forest Constants
#define N_FERNS 10
#define N_FEATURES 5
#define MIN_SCALE 0.1f
#define MAX_SCALE 0.5f
#define DECISION_THRESHOLD 0.5f

//P-N Criteria of deciding if a bb is positive or not.
#define MIN_POSITIVE_OVERLAP 0.6f
#define MAX_NEGATIVE_OVERLAP 0.2f

/*<NOT USED YET>*/

//Threshold defining the validity of the tracker
#define THRESHOLD_NN_VALID 0.7f

//Threshold defining if some patch is the same as the already existing inside the positive/negative set
#define THE_SAME 0.95f

/*</NOT USED YET>*/

//Clustering overlapping condition
#define CUTOFF 0.5f

//Patch dimension
 #define NORMALIZED_PATCH_DIM 15


 #define FRAME_W 448
 #define FRAME_H 336

/*detector constants*/
 #define MIN_BB 24
 #define MIN_WINDOW_SCALE -4.0f
 #define MAX_WINDOW_SCALE 9.0f
 #define BASE_WINDOW_SCALE 1.2f
 #define SHIFT 0.1f

 /*1-NN Classifier Constants*/
 #define THETA_TP 0.65f
 #define THETA_FP 0.5f

 #define TLD_CONFIG_FILE "tldConfig.txt"





#endif
