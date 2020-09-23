#pragma once

#include <vector>

#include "KCamera.h"
#include "MyCapture.h"
#include "MatProcessor\Processor.h"
#include "MatProcessor\Thresholder.h"

class System
{
private:
	/*读取阈值文件格式*/
#pragma pack(1)

	typedef struct ThresholdValue
	{
		//阈值修改时间

		uchar lower[3];		/*三通道低阈值*/
		uchar upper[3];		/*三通道高阈值*/
	}*pThresholdValue;

#pragma pack()

public:
	System();

	~System();

	bool Init();

	bool Run();

private:
	void InitThreshold();

	/*opencv鼠标事件*/
	static void onMouse(int event, int x, int y, int flags, void* param);

	/**
	*	\brief	获取鼠标点击（鼠标事件中已存储）
	*
	*	\return	是否点击
	*/
	bool GetClick();

	static cv::Point _click;			/*标记点*/
	static bool _clickFlag;				/*标记标志*/

private:
	//std::vector<int> _lowerb, _upperb;	/*上下阈值*/
	KCamera _camera;					/*Kinect数据源*/
	static std::string _thresholdFileName;/*阈值文件名*/

	HANDLE _hMapFile;					/*文件映射句柄*/
	int*   _msg;						/*文件映射共享数据区*/

	//MyCapture _cap;

	IProcessor* proc;
	Thresholder* thres;
};