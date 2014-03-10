#pragma once

#include "3d.h"
#include <algorithm>
#include "CSceneNodeBoard.h"

class CCutter{//���ڴ洢ǽ��֮�����ȥ����һ���ֵ��������
public:
	const vector2 &Min() const{ return m_min; }
	const vector2 &Max() const{ return m_max; }
	//void SetIntersectingVolume(const AABB &aabb);
	//void CancelIntersectingVolume();
	CCutter(const vector2 &a, const vector2 &b): m_min(std::min(a.x,b.x), std::min(a.y,b.y)),m_max( std::max(a.x,b.x), std::max(a.y,b.y) ){}
	//Cutter�Ƕ�ά��,ֻ��x,y�����ϲ��з�Χ,z�����������޳���
	struct GuardData{//guardָ���ǹ��屻�к��³��ֵİ����ռ������
		float minZ;
		float maxZ;
		vector2 thickness;
		AABB volumeOfEffect;
		GuardData():minZ(-100),maxZ(-50),thickness(0,0),volumeOfEffect(MinimumAABB()){}
	};

	const GuardData &GetGuard(bool positiveX,bool positiveY) const{//һ��cutter���ĸ����Ͽ��ܻ���guard(������һ��ǽ���ж����������),positive?����ָ��ĳ�������������һ�滹�Ǹ������һ��.
		return m_guards[positiveX][positiveY];
	}

	void SetGuard(bool positiveX, bool positiveY, float minZ, float maxZ, const vector2 &thickness, const AABB &volumeOfEffect){
		GuardData &guard( m_guards[positiveX][positiveY] );
		guard.minZ = minZ;
		guard.maxZ = maxZ;
		guard.thickness = thickness;
		guard.volumeOfEffect = volumeOfEffect;
	}

private:
	GuardData m_guards[2][2];
	vector2 m_min,m_max;
	//std::auto_ptr<CSceneNodeBoard> m_boards[2][2];//[x,y][min,max]
};
