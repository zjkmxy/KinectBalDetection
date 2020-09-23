#pragma once

#include <opencv2/opencv.hpp>

class ImagePro
{
public:
	/**
	*	\brief	从输入的种子点使用漫水填充算法获取填充的轮廓
	*
	*	\param	src 输入图像
	*
	*	\param	seed 漫水填充使用的种子点
	*
	*	\param	useEdge 是否使用边缘掩膜 
	*
	*	\param	contour 最终获得的轮廓
	*/
	static bool FloodGetContour(const cv::Mat& src, cv::Point seed, bool useEdge, std::vector<cv::Point>& contour);

	/**
	*	\brief	获取符合要求的最大轮廓中心点坐标
	*
	*	\param	src 输入图像（二值图）
	*
	*	\param	dst 目标坐标
	*
	*	\return	是否找到符合要求的目标点
	*/
	static bool GetMaxDomainCenter(const cv::Mat src, cv::Point2i& dst);
};

