/*
**********************************************************************
*	Copyright (C) 2016.
*	All Rights Reserved.
**********************************************************************
*
*	@file		Track.h
*	@brief		�켣�������������������ɵ���й켣���(��С���˷�)�õ����
*
*	Date        Name        Description
*	31/01/16	TB			Creation.
*   17/02/16	TB			�����˶Ը���������ų�
*
*********************************************************************/
#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class Track
{
public:
	/**
	*	\brief	���췽��
	*/
	Track();

	/**
	*	\brief	���췽�����ع���
	*
	*	\param	len ����г�������
	*/
	Track(int len);

	/**
	*	\brief	���켣���������һ����ά�㣬�������������޳̶��򶥵���һ����
	*
	*	\param	p ����ĵ�����
	*/
	void AddPoint(cv::Point2f p);

	/**
	*	\brief	��ȡ���ֱ����x�ύ�����꣨һά���꣬��ȡ�ɹ�����Ϊ���г��ȴﵽ���ޣ�
	*
	*	\param	xValue ��x�ύ�㣨һά���꣩
	*
	*	\return	��ȡ�Ƿ�ɹ�
	*/
	bool GetTarget(float& xValue);

	/**
	*	\brief	��յ�ǰ�����
	*/
	void Clear();

private:
	const int m_Len;					/*����г������ޣ���Ϊ��ȡ�㹻����Ϣ��������*/
	std::vector<cv::Point2f> m_Points;	/*�����*/
};