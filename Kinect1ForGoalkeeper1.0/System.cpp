#include "System.h"
#include "MatProcessor\ImagePro.h"
#include "MatProcessor\Processor.h"
#include "MatProcessor\Thresholder.h"
#include "Track.h"

//#define CAPTURE_MODE

std::string System::_thresholdFileName = "ThresholdValue.rcm";
cv::Point System::_click = cv::Point();
bool System::_clickFlag = false;

System::System()
{
	proc = IProcessor::GetDefaultProcessor();
	thres = new Thresholder();
}

System::~System()
{
	if (_hMapFile)
		CloseHandle(_hMapFile);
	if (_msg)
		delete _msg;

	if (proc)
		delete proc;
	if (thres)
		delete thres;
}

bool System::Init()
{
#ifndef CAPTURE_MODE
	/*openni��Kinect��ʼ��*/
	Status rc = STATUS_OK;
	rc = OpenNI::initialize();

	Array<DeviceInfo> KinectList;
	OpenNI::enumerateDevices(&KinectList);

	if (!_camera.OpenKinect(KinectList[0].getUri())){
		while (true){
			if (_camera.OpenKinect(KinectList[0].getUri()))
				break;
			std::cout << "Kinect��ʼ��ʧ�ܣ����ڳ����ٴγ�ʼ������" << std::endl;
		}
	}
#endif // CAPTURE_MODE

	/*��ȡ��ֵ�ļ�*/
	HANDLE hFile = CreateFileA(_thresholdFileName.c_str(), GENERIC_WRITE | GENERIC_READ, 0,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	vector<int> lowerb(3), upperb(3);

	if (!hFile){
		std::cout << "��ȡ��ֵ�ļ�ʧ�ܣ�ֱ�ӽ�����ֵ���ԣ�" << std::endl;
	}
	else{
		char buffer[sizeof(ThresholdValue)];
		pThresholdValue hdr = reinterpret_cast<pThresholdValue>(buffer);
		DWORD dwRead;				/**< ��ȡ�ֽ� */

		ReadFile(hFile, buffer, sizeof(ThresholdValue), &dwRead, NULL);

		if (dwRead < sizeof(ThresholdValue)){
			for (auto& temp : lowerb){
				temp = 0;
			}
			for (auto& temp : upperb){
				temp = 255;
			}
		}
		else{
			for (int i = 0; i < lowerb.size(); i++){
				lowerb[i] = hdr->lower[i];
			}
			for (int i = 0; i < upperb.size(); i++){
				upperb[i] = hdr->upper[i];
			}
		}

		CloseHandle(hFile);
	}

	thres->InputParameter(lowerb, upperb);

	/*��ֵ����*/
	InitThreshold();

	/*������ֵ�ļ���ɾ��ԭ���ģ��½�һ������ֵд�룩*/
	DeleteFileA(_thresholdFileName.c_str());

	hFile = CreateFileA(_thresholdFileName.c_str(), GENERIC_WRITE | GENERIC_READ, 0,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	char buffer[sizeof(ThresholdValue)] = { 0 };
	pThresholdValue hdr = reinterpret_cast<pThresholdValue>(buffer);
	for (int i = 0; i < 3; i++){
		hdr->lower[i] = lowerb[i];
	}
	for (int i = 0; i < 3; i++){
		hdr->upper[i] = upperb[i];
	}

	DWORD dwWrite;				/**< д�ֽ� */

	WriteFile(hFile, buffer, sizeof(ThresholdValue), &dwWrite, NULL);

	CloseHandle(hFile);

	_hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // �����ļ����
		NULL,                    // Ĭ�ϰ�ȫ����
		PAGE_READWRITE,          // �ɶ���д
		0,                       // ��λ�ļ���С
		4,					     // ��λ�ļ���С
		"SharedMemory"           // �����ڴ�����
		);

	_msg = (int *)MapViewOfFile(
		_hMapFile,            // �����ڴ�ľ��
		FILE_MAP_ALL_ACCESS, // �ɶ�д���
		0,					 // �ļ�ӳ����ʼƫ�Ƶĸ�32λ
		0,				     // �ļ�ӳ����ʼƫ�Ƶĵ�32λ
		4					 // ӳ���ļ��ֽ���
		);

	return true;
}

void System::InitThreshold()
{
	bool done = false;
	cv::Mat src;
	cv::Mat mid;
	cv::Mat dst;
	vector<int> lowerb(3), upperb(3);

	cv::namedWindow("Src", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("Res", CV_WINDOW_AUTOSIZE);

	cv::setMouseCallback("Src", onMouse);

	/*ˮ���Զ���ֵ��*/
	while (!done){
		if (cvWaitKey(1) != -1)
			break;
#ifndef CAPTURE_MODE
		if (!_camera.GetBGRImg(src)){
			continue;
		}
#else
		src = _cap.GetFrame();
#endif
		mid = proc->Process(src);

		//cv::inRange(mid, _lowerb, _upperb, dst);
		dst = thres->Process(mid);
		
		if (GetClick()){
			std::vector<cv::Point> contour;

			if (ImagePro::FloodGetContour(src, _click, false, contour)){
				mid = proc->Setup(src, contour);
				thres->Setup(mid, contour, upperb, lowerb);
			}

			std::cout << lowerb[0] << " " << lowerb[1] << " " << lowerb[2] << std::endl;
			std::cout << upperb[0] << " " << upperb[1] << " " << upperb[2] << std::endl;
		}

		cv::imshow("Src", src);
		cv::imshow("Res", dst);
	}

	cv::destroyWindow("Src");

	cv::namedWindow("HChannel", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("SChannel", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("IChannel", CV_WINDOW_AUTOSIZE);

	cv::createTrackbar("Hmin", "HChannel", &lowerb[0], 255);
	cv::createTrackbar("Hmax", "HChannel", &upperb[0], 255);
	cv::createTrackbar("Smin", "SChannel", &lowerb[1], 255);
	cv::createTrackbar("Smax", "SChannel", &upperb[1], 255);
	cv::createTrackbar("Imin", "IChannel", &lowerb[2], 255);
	cv::createTrackbar("Imax", "IChannel", &upperb[2], 255);

	while (true){
		if (cvWaitKey(1) != -1)
			break;

		std::vector<cv::Mat> channels(3);
		std::vector<cv::Mat> res(3);

		
#ifndef CAPTURE_MODE
		if (!_camera.GetBGRImg(src)){
			continue;
		}
#else
		src = _cap.GetFrame();
#endif
		thres->InputParameter(lowerb, upperb);

		mid = proc->Process(src);
		thres->ProcessPart(mid, res);
		dst = thres->Process(mid);
		
		cv::imshow("HChannel", res[0]);
		cv::imshow("SChannel", res[1]);
		cv::imshow("IChannel", res[2]);
		cv::imshow("Res", dst);
	}

	cv::destroyAllWindows();
}

bool System::Run()
{
	bool done = false;
	cv::Mat src;
	cv::Mat mid;
	cv::Mat dst;
	cv::Point contourCenter;
	cv::Point3f worldPos;
	Track track;
	float moveLen = 0.f;

	int time = clock();
	int count = 0;

	while (!done){
		if (clock() - time > 1000){
			std::cout << "Effective FPS = " << count << std::endl;
			count = 0;
			time = clock();
		}

#ifndef CAPTURE_MODE
		if (!_camera.GetBGRImg(src)){
			continue;
		}
#else
		src = _cap.GetFrame();
#endif
		count++;

		mid = proc->Process(src);
		dst = thres->Process(mid);

		if (ImagePro::GetMaxDomainCenter(dst, contourCenter)){
			_camera.UpdateDepthMsg();
			_camera.ColorToWorld(contourCenter, worldPos);

			//std::cout << worldPos.x << " " << worldPos.y << " " << worldPos.z << std::endl;

			if (worldPos.z > 0.f){
				track.AddPoint(cv::Point2f(worldPos.x, worldPos.z));
			}
			else{
				track.Clear();
			}
		}

		if (track.GetTarget(moveLen)){
			track.Clear();
			_msg[0] = 1;
			_msg[1] = (int)(moveLen / 10);

			std::cout << "move:" << _msg[1] << std::endl;
		}
	}

	return true;
}

void System::onMouse(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
	case CV_EVENT_LBUTTONDOWN:
		_click.x = x;
		_click.y = y;
		_clickFlag = true;
		break;
	case CV_EVENT_MOUSEMOVE:
		break;
	case CV_EVENT_LBUTTONUP:
		break;
	}
}

inline bool System::GetClick()
{
	if (!_clickFlag)
		return false;
	_clickFlag = false;
	return true;
}