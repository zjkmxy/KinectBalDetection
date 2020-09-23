#include "ImagePro.h"

bool ImagePro::FloodGetContour(const cv::Mat& src, cv::Point seed, bool useEdge, std::vector<cv::Point>& contour)
{
	cv::Mat dst;
	src.copyTo(dst);
	double scalar = 30.0;		/*漫水阈值*/

	if (useEdge){
		int width = src.cols, height = src.rows;
		cv::Mat edge, mask(height + 2, width + 2, CV_8U);
		src.copyTo(edge);

		cv::cvtColor(edge, edge, CV_RGB2GRAY);
		cv::Canny(edge, edge, 50, 200);

		for (int i = 0; i < height; i++){
			uchar* dataMask = mask.ptr<uchar>(i + 1);
			uchar* dataMid = edge.ptr<uchar>(i);

			for (int j = 0; j < width; j++){
				*++dataMask = *dataMid++;
			}
		}
		/*漫到的像素点像素值与种子点不能差距超过漫水阈值*/
		cv::floodFill(dst, mask, seed, cv::Scalar(255, 255, 255), NULL,
			cv::Scalar(scalar, scalar, scalar), cv::Scalar(scalar, scalar, scalar), cv::FLOODFILL_FIXED_RANGE);
		cv::imshow("edge", edge);
	}
	else{
		cv::floodFill(dst, seed, cv::Scalar(255, 255, 255), NULL,
			cv::Scalar(scalar, scalar, scalar), cv::Scalar(scalar, scalar, scalar), cv::FLOODFILL_FIXED_RANGE);
	}


	/*与原图作差即得到轮廓（理论上只有一个轮廓）*/
	dst -= src;
	cv::imshow("contour", dst);

	std::vector<cv::Mat> channels;
	cv::split(dst, channels);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(channels[0], contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	/*检查轮廓*/
	if (contours.size() == 0)
		return false;

	/*找最大的轮廓（虽然说应该只有一个）*/
	int maxC = 0;
	for (int i = 0; i < contours.size(); i++){
		if (contours[i].size() > contours[maxC].size())
			maxC = i;
	}

	contour = contours[maxC];

	return true;
}

bool ImagePro::GetMaxDomainCenter(const cv::Mat src, cv::Point2i& dst)
{
	if (src.channels() != 1)
		return false;

	int areaT = 50;		/*区域阈值（小于此阈值放弃轮廓）*/
	float rateT = 2.0;	/*长宽比比例阈值（大于此阈值放弃轮廓）*/

	std::vector<std::vector<cv::Point>> contours;

	cv::findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	double maxArea = 0;
	std::vector<cv::Point> maxContour;
	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		if (area > maxArea && area > areaT)
		{
			maxArea = area;
			maxContour = contours[i];
		}
	}
	if (maxArea < areaT)
		return false;
	cv::Rect maxRect = cv::boundingRect(maxContour);
	float rate = (float)maxRect.height / maxRect.width;
	rate = rate >= 1.0f ? rate : 1.0f / rate;
	if (rate > rateT){
		return false;
	}

	dst.x = maxRect.x + maxRect.width / 2;
	dst.y = maxRect.y + maxRect.height / 2;

	return true;
}