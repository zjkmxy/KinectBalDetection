/*
**********************************************************************
*	Copyright (C) 2016.
*	All Rights Reserved.
**********************************************************************
*
*	@file		Track.h
*	@brief		轨迹拟合器，根据输入的若干点进行轨迹拟合(最小二乘法)得到落点
*
*	Date        Name        Description
*	31/01/16	TB			Creation.
*   17/02/16	TB			增加了对干扰情况的排除
*
*********************************************************************/
#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class Track
{
public:
	/**
	*	\brief	构造方法
	*/
	Track();

	/**
	*	\brief	构造方法（重构）
	*
	*	\param	len 点队列长度上限
	*/
	Track(int len);

	/**
	*	\brief	给轨迹拟合器输入一个二维点，若超过队列上限程度则顶掉第一个点
	*
	*	\param	p 输入的点坐标
	*/
	void AddPoint(cv::Point2f p);

	/**
	*	\brief	获取拟合直线与x轴交点坐标（一维坐标，获取成功条件为队列长度达到上限）
	*
	*	\param	xValue 与x轴交点（一维坐标）
	*
	*	\return	获取是否成功
	*/
	bool GetTarget(float& xValue);

	/**
	*	\brief	清空当前点队列
	*/
	void Clear();

private:
	const int m_Len;					/*点队列长度上限（作为获取足够点信息的条件）*/
	std::vector<cv::Point2f> m_Points;	/*点队列*/
};