#include "Whitener.h"

using namespace cv;

#pragma region 数学常数
static const float eps = 0.01f;
#pragma endregion

Whitener::Whitener(_In_ bool ZCA, _In_opt_ IProcessor *prev) :
prev(prev),
ZCA(ZCA),
PCACoe(Mat_<float>::eye(3, 3))
{
	PCAAvg[0] = PCAAvg[1] = PCAAvg[2] = 0.0f;
}

Whitener::~Whitener(){
	if(prev)
		delete prev;
}

IProcessor* Whitener::GetPrev(){
	return prev;
}

cv::Mat Whitener::Process(_In_ const cv::Mat& src){
	if(prev){
		return DoProcess(prev->Process(src));
	} else{
		return DoProcess(src);
	}
}

cv::Mat Whitener::DoProcess(_In_ const cv::Mat& src){
	int i, j, k, p;
	Mat_<Vec3f> dst(src.rows, src.cols);
	float val[3], arg[3][3];

	for(k = 0; k < 3; k++){
		for(p = 0; p < 3; p++){
			arg[k][p] = PCACoe.at<float>(k, p);
		}
	}

	for(i = 0; i < src.rows; i++){
		const float* datIn = src.ptr<const float>(i);
		float* datOut = dst.ptr<float>(i);
		for(j = 0; j < src.cols; j++){
			//获取输入
			for(k = 0; k < 3; k++){
				val[k] = *(datIn++) - PCAAvg[k];
			}
			//计算输出
			for(k = 0; k < 3; k++){
				float nv = 0.0f;
				for(p = 0; p < 3; p++){
					nv += arg[k][p] * val[p];
				}
				*(datOut++) = nv;
			}
		}
	}

	return dst;
}

Mat Whitener::Setup(_In_ const cv::Mat& src, _In_ const std::vector<cv::Point>& contour){
	int i, k, p;
	int cnt = (int)contour.size();
	Mat mid;
	float mvar[3][3] = {}, val[3];

	//前驱运算
	if(prev){
		mid = prev->Setup(src, contour);
	} else{
		mid = src;
	}

	//计算均值方差
	PCAAvg[0] = PCAAvg[1] = PCAAvg[2] = 0.0f;
	for(i = 0; i < cnt; i++){
		const float* data = mid.ptr<const float>(contour[i].y);
		data += contour[i].x * 3;
		for(k = 0; k < 3; k++){
			val[k] = *(data ++);
			PCAAvg[k] += val[k];
		}
		for(k = 0; k < 3; k++){
			for(p = 0; p < 3; p++){
				mvar[k][p] += val[k] * val[p];
			}
		}
	}

	for(k = 0; k < 3; k++){
		PCAAvg[k] /= cnt;
	}
	for(k = 0; k < 3; k++){
		for(p = 0; p < 3; p++){
			mvar[k][p] /= cnt;
			mvar[k][p] -= PCAAvg[k] * PCAAvg[p];
		}
	}

	//计算系数
	cv::Mat_<float> tmpvar(3, 3, &mvar[0][0]);
	PCACoe = GetPCAArg(tmpvar, ZCA);

	return DoProcess(mid);
}

Mat Whitener::GetPCAArg(_In_ const cv::Mat& var, _In_ bool ZCA){
	cv::Mat s, u, vt;
	cv::Mat_<float> sdiag(3, 3, 0.0f);
	cv::SVDecomp(var, s, u, vt);
	cv::transpose(u, vt);

	for(int i = 0; i < 3; i++){
		sdiag.at<float>(i, i) = sqrt(eps + s.at<float>(i));
	}
	if(ZCA){
		return u * sdiag * vt;
	}else{
		return sdiag * vt;
	}
}