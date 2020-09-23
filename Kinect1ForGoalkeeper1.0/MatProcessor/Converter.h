#ifndef _CONVERTER_H
#define _CONVERTER_H

#include "Processor.h"

/*
* ͼ���ʽת����
* RGB8λ����(0~255)ת��Ϊ��ͨ������������(-1.0~1.0)
* ������֧��ǰ�����뽫�����Ϊ��һ��ͨ��
* ���ѡ��ת��Ϊ��H��ͨ��������Զ������ȵ�����ľ�ֵΪ0
*/
class Converter : public IProcessor{
public:
	enum class ColorSpace{
		CS_RGB = 0,
		CS_HSI = 1,
	};

private:
	ColorSpace type; //Ŀ��ͼ���ʽ
	float havg; //H������ƽ��ֵ

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