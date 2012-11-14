#ifndef _VIDEO_HANDLER_
#define _VIDEO_HANDLER_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>


using namespace cv;

class VideoHandler
{
public:
	
	VideoCapture vcap;
	int w,h;
	Mat currentFrame;
	int maxFPS;
	int frameCounter;
	int fps;
	
	bool frameBound;

	double t0, tf, t;


	VideoHandler(int w, int h, string source, bool frameBound = false, int maxFPS = 30);
	VideoHandler(int w, int h, int source = 0, bool frameBound = false, int maxFPS = 30);

	~VideoHandler();


	bool acquire(Mat& img);
	bool acquire();

	void frameControl();

private:

	void config(int w, int h, bool frameBound = false, int maxFPS = 30);


};



#endif