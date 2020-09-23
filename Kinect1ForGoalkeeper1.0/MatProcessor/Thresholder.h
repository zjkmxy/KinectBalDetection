#ifndef _THRESHOLDER_H
#define _THRESHOLDER_H

#include "Processor.h"
#include <vector>

/*
* ͼ���ֵ����
* ����ΪRGB8ͼ������
* ���Ϊ��ֵͼ
*/
class Thresholder{
private:
	//��ֵ�½�
	std::vector<int> lowerb;

	//��ֵ�Ͻ�
	std::vector<int> upperb;

public:
	Thresholder();
	~Thresholder();

	cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour, _Out_ std::vector<int>& upp, _Out_ std::vector<int>& low);
	cv::Mat Process(_In_ const cv::Mat& src);

	void ProcessPart(_In_ const cv::Mat& src, _Out_ std::vector<cv::Mat>& dst);

	/*
	* �˹��޸Ĳ���
	*/
	void InputParameter(_In_ const std::vector<int>& lower, _In_ const std::vector<int>& upper);

private:

	/**
	* ��һ����ͨ��ͼģ���˹�����ֵ�����ѵ�ͨ����ֵ
	* src		: ����ͼ�񣨵�ͨ���Ҷ�ͼ��
	* contour	: ����Ŀ����Ȥ����
	* inPix		: Ĭ�ϳ���Ϊ256���洢��Ȥ���������طֲ�
	* outPix	: Ĭ�ϳ���Ϊ256���洢��Ȥ���������طֲ�
	* min		: ����õ�������ֵ
	* max		: ����õ�������ֵ
	*/
	static void SimpleThreshold(
		_In_ const cv::Mat& src,
		_In_ const std::vector<cv::Point>& contour,
		_In_reads_(256) int* inPix,
		_In_reads_(256) int* outPix,
		_Out_ int& min,
		_Out_ int& max);

	/**
	* ͳ��ѧ��ֵ���м���ĳ����ֵ��Ӧ��
	* src		: ����ͼ��
	* contour	: ����Ŀ����Ȥ����
	* inPix		: Ĭ�ϳ���Ϊ256���洢��Ȥ���������طֲ�
	* outPix	: Ĭ�ϳ���Ϊ256���洢��Ȥ���������طֲ�
	* bottom	: ����ֵ
	* top		: ����ֵ
	* ����ֵ	: ��Ӧ��
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