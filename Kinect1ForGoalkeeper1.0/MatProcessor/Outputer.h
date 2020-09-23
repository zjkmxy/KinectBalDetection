#ifndef _OUTPUTER_H
#define _OUTPUTER_H

#include "Processor.h"

/*
* ͼ���ʽת����
* ��ͨ������������(-1.0~1.0)ת��ΪRGB8λ����
* �����뽫�����Ϊ���һ��ͨ��
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
	* ��-1.0~1.0��������ת��Ϊ0~255���
	* src	: ����ͼ��
	* ����ֵ: ���ͼ��
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