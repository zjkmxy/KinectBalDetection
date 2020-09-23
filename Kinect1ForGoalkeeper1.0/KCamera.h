/*
**********************************************************************
*	Copyright (C) 2015.
*	All Rights Reserved.
**********************************************************************
*
*	@file		KCamera.h
*	@brief		Kinect摄像机类
*
*	Date        Name        Description
*	05/10/15	TB			Creation.
*	21/04/16	TB			优化了彩色数据和深度数据的获取
*
*********************************************************************/

#pragma once

#include <OpenNI.h>
#include <opencv2/opencv.hpp>

using namespace openni;
using namespace cv;

class KCamera
{
public:
	/**
	*	\brief	构造函数
	*/
	KCamera();

	/**
	*	\brief	析构函数
	*/
	~KCamera();

	/**
	*	\brief	打开kinect摄像头
	*
	*	\param	deviceURL kinect设备编号指针
	*
	*	\return	操作是否成功
	*/
	bool OpenKinect(const char * deviceURL);

	/**
	*	\brief	更新当前深度数据	注意：此操作应在主循环中每帧独立处理
	*/
	void UpdateDepthMsg();

	/**
	*	\brief	获取摄像机当前帧深度图
	*
	*	\return	深度图
	*/
	bool GetDepthImg(Mat& depthImg);

	/**
	*  \brief 获取摄像机当前帧BGR图
	*
	*	\return	彩色图
	*/
	bool GetBGRImg(Mat& colorImg);

	/**
	*  \brief 获取对应点的深度信息
	*
	*	\param	idx 坐标（转化为一维矩阵后的）
	*
	*	\return	深度信息（深度值）
	*/
	DepthPixel GetDepthMsg(int idx);

	/**
	*  \brief 将深度图坐标转化为彩色图坐标
	*
	*	\param	depthPos 深度图坐标
	*
	*	\return	对应彩色图坐标
	*/
	Point DepthToColor(CvPoint depthPos);

	/**
	*  \brief 将彩色图坐标转化为世界坐标
	*
	*	\param	imgPos 彩色图坐标
	*
	*	\param	worldPos 世界坐标
	*
	*	\return	对应世界坐标
	*/
	void ColorToWorld(cv::Point imgPos, cv::Point3f& worldPos);
private:

	Device device;				 /*设备*/

	VideoStream oniDepthStream;  /*深度数据流*/
	VideoStream oniColorStream;  /*RGB数据流*/

	DepthPixel* m_pDepth;		 /*深度信息指针*/

	static int DEPTH_IMG_SIZE;	/*深度数据尺寸*/
};