#include <windows.h>

#include "KCamera.h"

int KCamera::DEPTH_IMG_SIZE = 640 * 480;

KCamera::KCamera()
{
	m_pDepth = new DepthPixel[DEPTH_IMG_SIZE];
}

KCamera::~KCamera()
{
	if (!m_pDepth)
		delete m_pDepth;
	oniDepthStream.destroy();
	oniColorStream.destroy();
	device.close();
	OpenNI::shutdown();
}

bool KCamera::
OpenKinect(const char * deviceURL)
{
	Status rc = STATUS_OK;
	
	rc = device.open(deviceURL);

	if (STATUS_OK != rc){
		std::cerr << "无法打开设备：" << OpenNI::getExtendedError() << std::endl;
		return false;
	}

	rc = oniDepthStream.create(device, SENSOR_DEPTH);
	if (STATUS_OK == rc){
		VideoMode modeDepth;
		modeDepth.setResolution(640, 480);
		modeDepth.setFps(30);
		modeDepth.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);
		oniDepthStream.setVideoMode(modeDepth);

		oniDepthStream.start();
		if (STATUS_OK != rc){
			std::cerr << "无法打开深度数据流：" << OpenNI::getExtendedError() << std::endl;
			oniDepthStream.destroy();
			return false;
		}
	}
	else{
		std::cerr << "无法创建深度数据流：" << OpenNI::getExtendedError() << std::endl;
		return false;
	}


	rc = oniColorStream.create(device, SENSOR_COLOR);
	if (STATUS_OK == rc){
		VideoMode modeColor;
		modeColor.setResolution(640, 480);
		modeColor.setFps(30);
		modeColor.setPixelFormat(PIXEL_FORMAT_RGB888);

		if (device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR)){
			device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR);
		}

		rc = oniColorStream.start();
		if (STATUS_OK != rc){
			std::cerr << "无法打开彩色数据流：" << OpenNI::getExtendedError() << std::endl;
			oniColorStream.destroy();
			return false;
		}
	}
	else{
		std::cerr << "无法创建彩色数据流：" << OpenNI::getExtendedError() << std::endl;
		return false;
	}

	if (!oniDepthStream.isValid() || !oniColorStream.isValid()){
		std::cerr << "彩色或深度数据流不合法" << std::endl;
		OpenNI::shutdown();
		return false;
	}

	return true;
}

void KCamera::UpdateDepthMsg()
{
	VideoFrameRef oniDepthImg;

	if (STATUS_OK == oniDepthStream.readFrame(&oniDepthImg)){
		int size = oniDepthImg.getDataSize();
		DepthPixel* tmp = (DepthPixel *)oniDepthImg.getData();
		DepthPixel* depthTmp = m_pDepth;
		for (int i = 0; i < DEPTH_IMG_SIZE; i++){
			*depthTmp++ = *tmp++;
		}
	}
}

bool KCamera::
GetDepthImg(Mat& depthImg)
{
	VideoFrameRef oniDepthImg;

	if (STATUS_OK == oniDepthStream.readFrame(&oniDepthImg)){
		depthImg = Mat(oniDepthImg.getHeight(), oniDepthImg.getWidth(), CV_16UC1, (char*)oniDepthImg.getData());
		double DepthCoe = 255.0 / oniDepthStream.getMaxPixelValue();
		depthImg.convertTo(depthImg, CV_8UC1, DepthCoe);
		return true;
	}
	return false;
}

bool KCamera::
GetBGRImg(Mat& colorImg)
{
	VideoFrameRef oniColorImg;
	cv::Mat tmp;
	
	if (STATUS_OK == oniColorStream.readFrame(&oniColorImg))
	{
		tmp = Mat(oniColorImg.getHeight(), oniColorImg.getWidth(), CV_8UC3, (void*)oniColorImg.getData());
		tmp.copyTo(colorImg);
		return true;
	}
	return false;
}

Point KCamera::DepthToColor(CvPoint depthPos)
{
	VideoFrameRef oniDepthImg;

	int ColorX = 0, ColorY = 0, idx = depthPos.y * 640 + depthPos.x;

	CoordinateConverter::convertDepthToColor(
		oniDepthStream, 
		oniColorStream, 
		depthPos.x, 
		depthPos.y, 
		m_pDepth[idx], 
		&ColorX, 
		&ColorY);

	return Point(ColorX, ColorY);
}

DepthPixel KCamera::GetDepthMsg(int idx)
{
	if (idx >= DEPTH_IMG_SIZE)
		return 0;
	return m_pDepth[idx];
}

void KCamera::ColorToWorld(cv::Point imgPos, cv::Point3f& worldPos)
{
	DepthPixel depthValue = m_pDepth[imgPos.y * 640 + imgPos.x];
	CoordinateConverter::convertDepthToWorld(oniDepthStream, 
											 imgPos.x, 
											 imgPos.y, 
											 depthValue,
											 &worldPos.x,
											 &worldPos.y,
											 &worldPos.z);
}