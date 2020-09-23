#pragma once

#include <vector>

#include "KCamera.h"
#include "MyCapture.h"
#include "MatProcessor\Processor.h"
#include "MatProcessor\Thresholder.h"

class System
{
private:
	/*��ȡ��ֵ�ļ���ʽ*/
#pragma pack(1)

	typedef struct ThresholdValue
	{
		//��ֵ�޸�ʱ��

		uchar lower[3];		/*��ͨ������ֵ*/
		uchar upper[3];		/*��ͨ������ֵ*/
	}*pThresholdValue;

#pragma pack()

public:
	System();

	~System();

	bool Init();

	bool Run();

private:
	void InitThreshold();

	/*opencv����¼�*/
	static void onMouse(int event, int x, int y, int flags, void* param);

	/**
	*	\brief	��ȡ�����������¼����Ѵ洢��
	*
	*	\return	�Ƿ���
	*/
	bool GetClick();

	static cv::Point _click;			/*��ǵ�*/
	static bool _clickFlag;				/*��Ǳ�־*/

private:
	//std::vector<int> _lowerb, _upperb;	/*������ֵ*/
	KCamera _camera;					/*Kinect����Դ*/
	static std::string _thresholdFileName;/*��ֵ�ļ���*/

	HANDLE _hMapFile;					/*�ļ�ӳ����*/
	int*   _msg;						/*�ļ�ӳ�乲��������*/

	//MyCapture _cap;

	IProcessor* proc;
	Thresholder* thres;
};