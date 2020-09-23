#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include <sal.h>
#include <opencv2/opencv.hpp>
#include <vector>

//�ֽ׶�Ϊ������δ�ṩ��������ת���õĴ����
//��װԽ�ã��ٶ�Խ��

/*
* ͼ�����ݴ�����ӿ�
* ���д����ֻ����-1.0~+1.0��float����
* ����Ԥ�����������ǰ��prev�����һ��Զ��ͷ���ǰ��
* ÿ���������Ҫ���ض���ʽ����������
*/
struct IProcessor abstract{
	/*
	* ȡ�ô������ǰ��
	*/
	virtual IProcessor* GetPrev() = 0;

	/*
	* ����Ԥ��ͼ���������
	* src		: ����Դͼ��
	* contour	: ����Ȥ������
	* ����ֵ	: �������Դͼ��
	*/
	virtual cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) = 0;

	/*
	* �ô��������ͼ��
	* src		: ����Դͼ��
	* ����ֵ	: �������Դͼ��
	*/
	virtual cv::Mat Process(_In_ const cv::Mat& src) = 0;

	/*
	* �������������Զ�ɾ��ǰ��
	*/
	virtual ~IProcessor(){};

	/*
	* �õ�һ��Ĭ�ϵĴ����
	*/
	static IProcessor* GetDefaultProcessor();
};

#endif