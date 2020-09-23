#ifndef _THRESHOLDER_H
#define _THRESHOLDER_H

#include "Processor.h"
#include <vector>

/*
* 图像二值化器
* 输入为RGB8图像数据
* 输出为二值图
*/
class Thresholder{
private:
	//阈值下界
	std::vector<int> lowerb;

	//阈值上界
	std::vector<int> upperb;

public:
	Thresholder();
	~Thresholder();

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour, _Out_ std::vector<int>& upp, _Out_ std::vector<int>& low);
	cv::Mat Process(_In_ const cv::Mat& src);

	void ProcessPart(_In_ const cv::Mat& src, _Out_ std::vector<cv::Mat>& dst);

	/*
	* 人工修改参数
	*/
	void InputParameter(_In_ const std::vector<int>& lower, _In_ const std::vector<int>& upper);

private:

	/**
	* 对一幅单通道图模拟人工调阈值获得最佳单通道阈值
	* src		: 输入图像（单通道灰度图）
	* contour	: 输入目标兴趣区域
	* inPix		: 默认长度为256，存储兴趣区域内像素分布
	* outPix	: 默认长度为256，存储兴趣区域外像素分布
	* min		: 输出得到的下阈值
	* max		: 输出得到的上阈值
	*/
	static void SimpleThreshold(
		_In_ const cv::Mat& src,
		_In_ const std::vector<cv::Point>& contour,
		_In_reads_(256) int* inPix,
		_In_reads_(256) int* outPix,
		_Out_ int& min,
		_Out_ int& max);

	/**
	* 统计学阈值化中极端某个阈值适应度
	* src		: 输入图像
	* contour	: 输入目标兴趣区域
	* inPix		: 默认长度为256，存储兴趣区域内像素分布
	* outPix	: 默认长度为256，存储兴趣区域外像素分布
	* bottom	: 下阈值
	* top		: 上阈值
	* 返回值	: 适应度
	*/
	static float CalcFitness(
		_In_ const cv::Mat& src,
		_In_ const std::vector<cv::Point>& contour,
		_In_reads_(256) int* inPix,
		_In_reads_(256) int* outPix,
		_In_ int bottom,
		_In_ int top);
};

#endif