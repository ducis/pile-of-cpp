#pragma once

#include "3d.h"
#include <utility>
#include "IAnnotated.h"
#include "IObjectSource.h"
#include <set>

class CAxialAABBAnnotation:public IAnnotated{
	//用于显示某个坐标轴方向上的连续标注的类,储存多个点,将这些点排序后,显示它们之间的距离
public:
	class CAxialAABBAnnotation( const vector3 &axis, const std::pair<float,float> &range, const vector3 &offset = vector3(0,0,0) );
	void RegisterPoint( IObjectSource<float> *pSource );
	void UnregisterPoint( IObjectSource<float> *pSource );
	virtual void ShowAnnotation() const;
	const vector3 &GetNormal(){ return m_axis; }//返回方向向量
private:
	std::set< IObjectSource<float>* > m_points;
	vector3 m_axis;
	vector3 m_offset;
	std::pair<float,float> m_range;
};
