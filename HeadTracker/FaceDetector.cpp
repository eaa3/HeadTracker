#include "FaceDetector.h"


FaceDetector::FaceDetector( string filename )
{
	this->init(filename);
}

FaceDetector::FaceDetector( )
{
	this->isOk = false;
}

void FaceDetector::init( string filename )
{
	this->isOk = face_cascade.load(filename);
}

void FaceDetector::detect( Mat& img )
{
	Mat frame_gray;

	cvtColor( img, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	//-- Detect faces
	this->detectionResult.clear();
	face_cascade.detectMultiScale( frame_gray, this->detectionResult, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );		
}
