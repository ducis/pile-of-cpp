#pragma once

#include "3d.h"
#include <utility>
#include "IAnnotated.h"
#include "IObjectSource.h"
#include <set>

class CAxialAABBAnnotation:public IAnnotated{
	//������ʾĳ�������᷽���ϵ�������ע����,��������,����Щ�������,��ʾ����֮��ľ���
public:
	class CAxialAABBAnnotation( const vector3 &axis, const std::pair<float,float> &range, const vector3 &offset = vector3(0,0,0) );
	void RegisterPoint( IObjectSource<float> *pSource );
	void UnregisterPoint( IObjectSource<float> *pSource );
	virtual void ShowAnnotation() const;
	const vector3 &GetNormal(){ return m_axis; }//���ط�������
private:
	std::set< IObjectSource<float>* > m_points;
	vector3 m_axis;
	vector3 m_offset;
	std::pair<float,float> m_range;
};
