#include "Thresholder.h"

#include <cmath>
using namespace cv;
using namespace std;

Thresholder::Thresholder()
{
	lowerb.push_back(0);
	lowerb.push_back(0);
	lowerb.push_back(0);
	upperb.push_back(255);
	upperb.push_back(255);
	upperb.push_back(255);
}

Thresholder::~Thresholder(){
}

cv::Mat Thresholder::Process(_In_ const cv::Mat& src){
	Mat dst;

	inRange(src, lowerb, upperb, dst);
	return dst;
}

void Thresholder::SimpleThreshold(
	_In_ const Mat& src,
	_In_ const vector<Point>& contour,
	_In_reads_(256) int* inPix,
	_In_reads_(256) int* outPix,
	_Out_ int& min,
	_Out_ int& max)
{
	int mid = 0;
	float maxVal = float(-INT_MAX);
	int bottom = 0, top = 255;

	//找到矩形内像素值中的众数
	for(int i = 0; i<256; i++){
		if(inPix[i] > inPix[mid])
			mid = i;
	}

	maxVal = float(-INT_MAX);
	for(int i = mid; i >= 0; i--){
		float tmp = CalcFitness(src, contour, inPix, outPix, i, mid);
		if(tmp > maxVal){
			bottom = i;
			maxVal = tmp;
		}
	}

	maxVal = float(-INT_MAX);
	for(int i = mid; i <= 255; i++){
		float tmp = CalcFitness(src, contour, inPix, outPix, bottom, i);
		if(tmp > maxVal){
			top = i;
			maxVal = tmp;
		}
	}

	min = bottom;
	max = top;
}

float Thresholder::CalcFitness(
	_In_ const cv::Mat& src,
	_In_ const std::vector<cv::Point>& contour,
	_In_reads_(256) int* inPix,
	_In_reads_(256) int* outPix,
	_In_ int bottom,
	_In_ int top)
{
	float black = 0.0f, white = 0.0f;

	//（S总 - S内） / S内 = S总 / S内 - 1
	const float rat = 1.0f * src.rows * src.cols / contour.size() - 1;

	for(int i = 0; i < 256; i++){
		if(i < bottom || i > top){		//黑
			black += outPix[i];
		} else{							//白
			white += inPix[i];
		}
	}

	return black + white * rat;
}

Mat Thresholder::Setup(_In_ const Mat& src, _In_ const vector<Point>& contour, _Out_ std::vector<int>& upp, _Out_ std::vector<int>& low){
	Mat dst;

	int inPixH[256] = {0};
	int outPixH[256] = {0};
	int inPixS[256] = {0};
	int outPixS[256] = {0};
	int inPixI[256] = {0};
	int outPixI[256] = {0};

	int width = src.cols, height = src.rows;

	for(int i = 0; i < height; i++){
		const uchar* data = src.ptr<const uchar>(i);
		for(int j = 0; j < width; j++){
			int tempH = *data++;
			int tempS = *data++;
			int tempI = *data++;

			outPixH[tempH]++;
			outPixS[tempS]++;
			outPixI[tempI]++;
		}
	}

	for(int i = 0; i < contour.size(); i++){
		const uchar* data = src.ptr<const uchar>(contour[i].y);
		data += contour[i].x * 3;
		int tempH = *data++;
		int tempS = *data++;
		int tempI = *data++;

		inPixH[tempH]++;
		inPixS[tempS]++;
		inPixI[tempI]++;
	}

	for(int i = 0; i < 256; i++){
		outPixH[i] -= inPixH[i];
		outPixS[i] -= inPixS[i];
		outPixI[i] -= inPixI[i];
	}

	SimpleThreshold(src, contour, inPixH, outPixH, lowerb[0], upperb[0]);
	SimpleThreshold(src, contour, inPixS, outPixS, lowerb[1], upperb[1]);
	SimpleThreshold(src, contour, inPixI, outPixI, lowerb[2], upperb[2]);

	low = lowerb;
	upp = upperb;

	//计算结果
	inRange(src, lowerb, upperb, dst);
	return dst;
}

void Thresholder::InputParameter(_In_ const std::vector<int>& lower, _In_ const std::vector<int>& upper){
	copy(lower.begin(), lower.end(), lowerb.begin());
	copy(upper.begin(), upper.end(), upperb.begin());
}

void Thresholder::ProcessPart(_In_ const cv::Mat& src, _Out_ vector<Mat>& dst){
	split(src, dst);
	for (int channel = 0; channel < 3; channel ++)
		inRange(dst[channel], lowerb[channel], upperb[channel], dst[channel]);
}