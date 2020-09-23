/*
**********************************************************************
*	Copyright (C) 2015.
*	All Rights Reserved.
**********************************************************************
*
*	@file		KCamera.h
*	@brief		Kinect�������
*
*	Date        Name        Description
*	05/10/15	TB			Creation.
*	21/04/16	TB			�Ż��˲�ɫ���ݺ�������ݵĻ�ȡ
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
	*	\brief	���캯��
	*/
	KCamera();

	/**
	*	\brief	��������
	*/
	~KCamera();

	/**
	*	\brief	��kinect����ͷ
	*
	*	\param	deviceURL kinect�豸���ָ��
	*
	*	\return	�����Ƿ�ɹ�
	*/
	bool OpenKinect(const char * deviceURL);

	/**
	*	\brief	���µ�ǰ�������	ע�⣺�˲���Ӧ����ѭ����ÿ֡��������
	*/
	void UpdateDepthMsg();

	/**
	*	\brief	��ȡ�������ǰ֡���ͼ
	*
	*	\return	���ͼ
	*/
	bool GetDepthImg(Mat& depthImg);

	/**
	*  \brief ��ȡ�������ǰ֡BGRͼ
	*
	*	\return	��ɫͼ
	*/
	bool GetBGRImg(Mat& colorImg);

	/**
	*  \brief ��ȡ��Ӧ��������Ϣ
	*
	*	\param	idx ���꣨ת��Ϊһά�����ģ�
	*
	*	\return	�����Ϣ�����ֵ��
	*/
	DepthPixel GetDepthMsg(int idx);

	/**
	*  \brief �����ͼ����ת��Ϊ��ɫͼ����
	*
	*	\param	depthPos ���ͼ����
	*
	*	\return	��Ӧ��ɫͼ����
	*/
	Point DepthToColor(CvPoint depthPos);

	/**
	*  \brief ����ɫͼ����ת��Ϊ��������
	*
	*	\param	imgPos ��ɫͼ����
	*
	*	\param	worldPos ��������
	*
	*	\return	��Ӧ��������
	*/
	void ColorToWorld(cv::Point imgPos, cv::Point3f& worldPos);
private:

	Device device;				 /*�豸*/

	VideoStream oniDepthStream;  /*���������*/
	VideoStream oniColorStream;  /*RGB������*/

	DepthPixel* m_pDepth;		 /*�����Ϣָ��*/

	static int DEPTH_IMG_SIZE;	/*������ݳߴ�*/
};