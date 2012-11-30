#include "videohandler.h"



VideoHandler::VideoHandler(int w, int h, string source, bool frameBound, int maxFPS)
{
	this->vcap.open(source);

	this->config(w,h, frameBound, maxFPS);
}

VideoHandler::VideoHandler(int w, int h, int source, bool frameBound, int maxFPS)
{
	this->vcap.open(source);

	this->config(w,h, frameBound, maxFPS);
}

VideoHandler::~VideoHandler()
{
	this->vcap.release();
}


void VideoHandler::config(int w, int h, bool frameBound, int maxFPS)
{
	this->vcap.set(CV_CAP_PROP_FRAME_WIDTH, w);
    this->vcap.set(CV_CAP_PROP_FRAME_HEIGHT, h);

	this->maxFPS = maxFPS;
	
	this->t0 = GetTickCount()/getTickFrequency();
	this->tf = 0.0;
	this->frameCounter = 0;

	this->w = w;
	this->h = h;
}

bool VideoHandler::acquire()
{
	if( !this->vcap.isOpened() ) return false;

	this->vcap >> this->currentFrame;

	if( this->currentFrame.empty() ) return false;


	Mat temp;
	resize(this->currentFrame, temp, Size(this->w, this->h));

	this->currentFrame = temp;

	//this->frameControl();

	return true;
}

bool VideoHandler::acquire(Mat& img)
{
	bool success = this->acquire();

	img = this->currentFrame;

	return success;
}

void VideoHandler::frameControl()
{
	if( !this->frameBound ) return;

	this->tf = getTickCount()*1000/getTickFrequency();

	double diff = (tf - t0);

	double sleepTime = (1000.0/this->maxFPS - diff );
	if( sleepTime > 0.0 ) Sleep(int(sleepTime));

	this->frameCounter++;

	double tdiff = getTickCount()*1000.0/getTickFrequency() - this->t;
	if( tdiff > 1000.0 )
	{

		this->fps = this->frameCounter;
		this->t = getTickCount()*1000.0/getTickFrequency();
	}

	this->t0 = getTickCount()*1000/getTickFrequency();
}




