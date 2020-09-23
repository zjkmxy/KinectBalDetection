#include "Outputer.h"

#include <thread>
using namespace cv;
using namespace std;

void Outputer::ThreadMain(
	_In_ const cv::Mat& src,
	_In_ int strow,
	_In_ int edrow,
	_Out_ cv::Mat& dst)
{
	int i, j, k;
	float val;
	for (i = strow; i < edrow; i++){
		const float* datIn = src.ptr<const float>(i);
		uchar* datOut = dst.ptr<uchar>(i);
		for (j = 0; j < src.cols; j++){
			for (k = 0; k < 3; k++){
				val = *(datIn++);
				val = (val + 1.0f) * 127.5f;
				val = min(255.0f, max(val, 0.0f));
				//*(datOut ++) = (uchar)round(val); //round too tlow, desprecated
				*(datOut++) = (uchar)(val + 0.5f);
			}
		}
	}
}

Mat Outputer::FloatToUbyte(_In_ const Mat src){
	Mat_<Vec3b> dst(src.rows, src.cols);
	int mid = src.rows / 2;

	thread th1(ThreadMain, src, 0, mid, dst);
	thread th2(ThreadMain, src, mid, src.rows, dst);
	th1.join();
	th2.join();

	return dst;
}

Outputer::Outputer(_In_opt_ IProcessor *prev) :
prev(prev)
{
}

Outputer::~Outputer(){
	if(prev)
		delete prev;
}

IProcessor* Outputer::GetPrev(){
	return prev;
}

cv::Mat Outputer::Process(_In_ const cv::Mat& src){
	if(prev){
		return FloatToUbyte(prev->Process(src));
	} else{
		return FloatToUbyte(src);
	}
}

Mat Outputer::Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour){
	if(prev){
		return FloatToUbyte(prev->Setup(src, contour));
	} else{
		return FloatToUbyte(src);
	}
}