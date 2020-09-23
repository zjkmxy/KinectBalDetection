#include "Converter.h"

#include <algorithm>
#include <cmath>
#include <thread>
using namespace cv;
using namespace std;

#pragma region 数学常数
static const float PI = 3.1415926535f;
static const float eps = 0.001f;
#pragma endregion

Converter::Converter(_In_ ColorSpace type) :
type(type),
havg(0)
{
}

Converter::~Converter(){
}

IProcessor* Converter::GetPrev(){
	return nullptr;
}

void Converter::ThreadMain(
	_In_ const cv::Mat& src,
	_In_ int strow,
	_In_ int edrow,
	_In_ float havg,
	_Out_ cv::Mat& dst)
{
	int i, j;
	float rv, gv, bv;
	int ri, gi, bi;
	float hv, sv, iv, minv;

	for (i = strow; i < edrow; i++){
		const uchar* datIn = src.ptr<const uchar>(i);
		float* datOut = dst.ptr<float>(i);
		for (j = 0; j < src.cols; j++){
			//转化成0.0 ~ 1.0
			ri = *(datIn++);
			gi = *(datIn++);
			bi = *(datIn++);

			rv = ri / 255.0f;
			gv = gi / 255.0f;
			bv = bi / 255.0f;


			minv = min(rv, min(gv, bv));
			iv = (rv + gv + bv) / 3.0f;
			sv = 1.0f - minv / (iv + eps);
			//这里h采用近似算法
			if (ri > gi){
				if (ri > bi){
					hv = (gv - bv) / (rv - minv + eps);
				}
				else{
					hv = (rv - gv) / (bv - minv + eps) + 4.0f;
				}
			}
			else{
				if (gi > bi){
					hv = (bv - rv) / (gv - minv + eps) + 2.0f;
				}
				else{
					hv = (rv - gv) / (bv - minv + eps) + 4.0f;
				}
			}
			if (hv < 0)
				hv = hv + 6.0f;

			//归一化到-1.0 ~ 1.0
			iv = iv * 2.0f - 1.0f;
			sv = sv * 2.0f - 1.0f;
			hv = hv / 3.0f - 1.0f;

			//H值的中心化
			hv = hv - havg;
			if (hv < -1.0f)
				hv = hv + 2.0f;

			//输出
			*(datOut++) = hv;
			*(datOut++) = sv;
			*(datOut++) = iv;
		}
	}
}

cv::Mat Converter::Process(_In_ const cv::Mat& src){

	Mat_<Vec3f> dst(src.rows, src.cols);
	int mid = src.rows / 2;

	//不接受prev
	if (type == Converter::ColorSpace::CS_HSI){
		thread th1(ThreadMain, src, 0, mid, havg, dst);
		thread th2(ThreadMain, src, mid, src.rows, havg, dst);
		th1.join();
		th2.join();
	}
	else{
		for (int i = 0; i < src.rows; i++){
			const uchar* datIn = src.ptr<const uchar>(i);
			float* datOut = dst.ptr<float>(i);
			for (int j = 0; j < src.cols; j++){
				*(datOut++) = *(datIn++) / 127.5f - 1.0f;
			}
		}
	}
	

	return dst;
}

Mat Converter::Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour){
	int i = 0, cnt = (int)contour.size();

	//不接受prev
	if (type == ColorSpace::CS_HSI){
		float avg1 = 0.0f;
		float avg2 = 0.0f;
		float var1 = 0.0f;
		float var2 = 0.0f;

		for (i = 0; i < cnt; i++){
			const uchar* data = src.ptr<const uchar>(contour[i].y);
			data += contour[i].x * 3;

			//计算当前点的H值
			float rv = *(data++) / 255.0f;
			float gv = *(data++) / 255.0f;
			float bv = *(data++) / 255.0f;
			float minv = min(rv, min(gv, bv));
			float hv = 0.0f;
			if (rv > gv && rv > bv){
				hv = (gv - bv) / (rv - minv + eps);
			}
			else if (gv > rv && gv > bv){
				hv = (bv - rv) / (gv - minv + eps) + 2.0f;
			}
			else{
				hv = (rv - gv) / (bv - minv + eps) + 4.0f;
			}
			if (hv < 0)
				hv = hv + 6.0f;
			hv = hv / 3.0f - 1.0f;

			//计算均值方差
			avg1 += hv;
			var1 += hv * hv;

			//旋转
			hv = hv + 1.0f;
			if (hv > 1.0f)
				hv -= 2.0f;

			//计算均值方差
			avg2 += hv;
			var2 += hv * hv;
		}

		avg1 /= cnt;
		avg2 /= cnt;
		var1 = var1 / cnt - avg1 * avg1;
		var2 = var2 / cnt - avg2 * avg2;

		//旋转回来
		avg2 = avg2 - 1.0f;
		if (avg2 < -1.0f)
			avg2 += 2.0f;

		//选择方差小的作为合理值
		if (var1 < var2){
			havg = avg1;
		}
		else{
			havg = avg2;
		}
	}
	return Process(src);
}