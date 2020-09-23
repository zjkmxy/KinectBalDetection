#include "Track.h"

Track::Track():
m_Len(5)
{

}

Track::Track(int len):
m_Len(len)
{

}

void Track::AddPoint(cv::Point2f p)
{
	m_Points.push_back(p);
	if (m_Points.size() > m_Len){
		m_Points.erase(m_Points.begin());
		std::cout << m_Points.size() << std::endl;
	}

	//测试用
	/*for (int i = 0; i < m_Points.size(); i++){
		std::cout << m_Points[i].x << "," << m_Points[i].y << " ";
	}
	std::cout << std::endl;*/
}

bool Track::GetTarget(float& xValue)
{
	
	if (m_Points.size() < m_Len)
		return false;
	
	/*球向远离方向飞行或数帧位置基本没有变化*/
	if (m_Points[m_Points.size() - 1].y - m_Points[0].y > 0 || 
		fabs(m_Points[m_Points.size() - 1].x - m_Points[0].x) + fabs(m_Points[m_Points.size() - 1].y - m_Points[0].y) < 100){
		
		Clear();
		return false;
	}

	float aveX = 0, aveY = 0;
	float a = 0, b = 0;

	for (int i = 0; i < m_Len; i++){
		aveX += m_Points[i].x;
		aveY += m_Points[i].y;
	}
	aveX /= m_Len;
	aveY /= m_Len;

	for (int i = 0; i < m_Len; i++){
		b += m_Points[i].x * m_Points[i].y;
	}
	b -= m_Len * aveX * aveY;

	float temp = 0;
	for (int i = 0; i < m_Len; i++){
		temp += m_Points[i].x * m_Points[i].x;
	}
	temp -= m_Len * aveX * aveX;
	
	if (temp == 0){		//垂直于x轴
		xValue = aveX;
		return true;
	}
	b /= temp;
	a = aveY - b * aveX;

	if (b == 0)
		return false;
	xValue = -a / b;
	
	return true;
}

void Track::Clear()
{
	m_Points.clear();
}