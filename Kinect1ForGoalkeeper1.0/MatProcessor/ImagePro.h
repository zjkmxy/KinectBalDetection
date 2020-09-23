#pragma once

#include <opencv2/opencv.hpp>

class ImagePro
{
public:
	/**
	*	\brief	����������ӵ�ʹ����ˮ����㷨��ȡ��������
	*
	*	\param	src ����ͼ��
	*
	*	\param	seed ��ˮ���ʹ�õ����ӵ�
	*
	*	\param	useEdge �Ƿ�ʹ�ñ�Ե��Ĥ 
	*
	*	\param	contour ���ջ�õ�����
	*/
	static bool FloodGetContour(const cv::Mat& src, cv::Point seed, bool useEdge, std::vector<cv::Point>& contour);

	/**
	*	\brief	��ȡ����Ҫ�������������ĵ�����
	*
	*	\param	src ����ͼ�񣨶�ֵͼ��
	*
	*	\param	dst Ŀ������
	*
	*	\return	�Ƿ��ҵ�����Ҫ���Ŀ���
	*/
	static bool GetMaxDomainCenter(const cv::Mat src, cv::Point2i& dst);
};

