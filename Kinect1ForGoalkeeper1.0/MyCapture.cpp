#include "MyCapture.h"


MyCapture::MyCapture() :
cap(0)
{
	cap.isOpened();
}


MyCapture::~MyCapture()
{
}

cv::Mat MyCapture::GetFrame()
{
	cv::Mat frame;
	cap >> frame;
	return frame;
}