#include "CAxialAABBAnnotation.h"
#include <algorithm>
#include <functional>
#include "singleton.h"
#include "CAnnotationStyle.h"

using namespace std;

CAxialAABBAnnotation::CAxialAABBAnnotation(const vector3 &axis, const std::pair<float,float> &range, const vector3 &offset)
:m_axis(axis),m_range(range),m_offset(offset)
{
}



void CAxialAABBAnnotation::RegisterPoint( IObjectSource<float> *pSource ){
	m_points.insert(pSource);
}



void CAxialAABBAnnotation::UnregisterPoint( IObjectSource<float> *pSource ){
	m_points.erase(pSource);
}



void CAxialAABBAnnotation::ShowAnnotation() const{
	float pts[1024];
	//vector<float> pts;
	transform( m_points.begin(), m_points.end(), pts, mem_fun(&IObjectSource<float>::Get) );
	pts[m_points.size()] = m_range.first;
	pts[m_points.size()+1] = m_range.second;
	int n(m_points.size()+2);
	sort(pts,pts+n);
	GetSingleton<CAnnotationStyle>()->DrawAnnotationEnd(m_offset+m_axis*pts[0]);
	for(int i=1;i<n;++i){
		//if(pts[i]-pts[i-1]>=100){
		int num(pts[i]-pts[i-1]+0.5f);
		if(num>0){
			GetSingleton<CAnnotationStyle>()->DrawAnnotation(
				m_offset+m_axis*pts[i-1],
				m_offset+m_axis*pts[i],
				num,//pts[i]-pts[i-1]+0.5f,
				CAnnotationStyle::DIGITS_BIT|CAnnotationStyle::LINE_BIT|CAnnotationStyle::ENDS_BIT
				);
		}
	}
}



