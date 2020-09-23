#ifndef _WHITENER_H
#define _WHITENER_H

#include "Processor.h"

/*
* ͼ������׻���
* ���������Ϊ��ͨ��������(-1.0~1.0)����
*/
class Whitener : public IProcessor{
private:
	IProcessor *prev;

	bool ZCA;	//1->ZCA 0->PCA

	cv::Mat PCACoe; //PCAϵ������

	float PCAAvg[3]; //PCAƽ��ֵ

public:
	Whitener(_In_ bool ZCA, _In_opt_ IProcessor *prev);
	~Whitener() override;

	IProcessor* GetPrev() override;

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) override;
	cv::Mat Process(_In_ const cv::Mat& src) override;

private:
	//����var����������PCA��������
	static cv::Mat GetPCAArg(_In_ const cv::Mat& var, _In_ bool ZCA);

	//�������д���
	cv::Mat DoProcess(_In_ const cv::Mat& src);
};

#endif