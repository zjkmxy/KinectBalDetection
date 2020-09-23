#ifndef _OUTPUTER_H
#define _OUTPUTER_H

#include "Processor.h"

/*
* 图像格式转换器
* 三通道浮点型数据(-1.0~1.0)转化为RGB8位数据
* ！！请将这个作为最后一个通道
*/
class Outputer : public IProcessor{
private:
	IProcessor *prev;

public:
	Outputer(_In_opt_ IProcessor *prev);
	~Outputer() override;

	IProcessor* GetPrev() override;

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) override;
	cv::Mat Process(_In_ const cv::Mat& src) override;

	/*
	* 将-1.0~1.0浮点输入转化为0~255输出
	* src	: 输入图像
	* 返回值: 输出图像
	*/
	static cv::Mat FloatToUbyte(_In_ const cv::Mat src);

private:
	static void ThreadMain(
		_In_ const cv::Mat& src,
		_In_ int strow,
		_In_ int edrow,
		_Out_ cv::Mat& dst);
};

#endif