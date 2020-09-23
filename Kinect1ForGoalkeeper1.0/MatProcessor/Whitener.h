#ifndef _WHITENER_H
#define _WHITENER_H

#include "Processor.h"

/*
* 图像输入白化器
* 输入输出均为三通道浮点型(-1.0~1.0)数据
*/
class Whitener : public IProcessor{
private:
	IProcessor *prev;

	bool ZCA;	//1->ZCA 0->PCA

	cv::Mat PCACoe; //PCA系数矩阵

	float PCAAvg[3]; //PCA平均值

public:
	Whitener(_In_ bool ZCA, _In_opt_ IProcessor *prev);
	~Whitener() override;

	IProcessor* GetPrev() override;

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) override;
	cv::Mat Process(_In_ const cv::Mat& src) override;

private:
	//根据var方差矩阵计算PCA参数矩阵
	static cv::Mat GetPCAArg(_In_ const cv::Mat& var, _In_ bool ZCA);

	//真正进行处理
	cv::Mat DoProcess(_In_ const cv::Mat& src);
};

#endif