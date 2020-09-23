#ifndef _CONVERTER_H
#define _CONVERTER_H

#include "Processor.h"

/*
* 图像格式转换器
* RGB8位数据(0~255)转化为三通道浮点型数据(-1.0~1.0)
* ！！不支持前驱，请将这个作为第一个通道
* 如果选择转化为带H的通道，则会自动调整热点区域的均值为0
*/
class Converter : public IProcessor{
public:
	enum class ColorSpace{
		CS_RGB = 0,
		CS_HSI = 1,
	};

private:
	ColorSpace type; //目标图像格式
	float havg; //H分量的平均值

public:
	Converter(_In_ ColorSpace type);
	~Converter() override;

	IProcessor* GetPrev() override;

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) override;
	cv::Mat Process(_In_ const cv::Mat& src) override;

private:
	static void ThreadMain(
		_In_ const cv::Mat& src,
		_In_ int strow,
		_In_ int edrow,
		_In_ float havg,
		_Out_ cv::Mat& dst);
};

#endif