#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include <sal.h>
#include <opencv2/opencv.hpp>
#include <vector>

//现阶段为简便起见未提供数据类型转换用的处理机
//封装越好，速度越慢

/*
* 图像数据处理机接口
* 所有处理机只处理-1.0~+1.0的float矩阵
* 所有预处理机保存其前驱prev，并且会自动释放其前驱
* 每个处理机会要求特定格式的输入和输出
*/
struct IProcessor abstract{
	/*
	* 取得处理机的前驱
	*/
	virtual IProcessor* GetPrev() = 0;

	/*
	* 根据预设图像建立处理机
	* src		: 输入源图像
	* contour	: 感兴趣的区域
	* 返回值	: 处理完的源图像
	*/
	virtual cv::Mat Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour) = 0;

	/*
	* 用处理机处理图像
	* src		: 输入源图像
	* 返回值	: 处理完的源图像
	*/
	virtual cv::Mat Process(_In_ const cv::Mat& src) = 0;

	/*
	* 析构函数，会自动删除前驱
	*/
	virtual ~IProcessor(){};

	/*
	* 得到一个默认的处理机
	*/
	static IProcessor* GetDefaultProcessor();
};

#endif