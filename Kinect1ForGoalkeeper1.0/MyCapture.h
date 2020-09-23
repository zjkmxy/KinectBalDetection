#pragma once

#include <opencv2/opencv.hpp>

class MyCapture
{
public:
	MyCapture();
	~MyCapture();

	cv::Mat GetFrame();

private:
	cv::VideoCapture cap;
};

