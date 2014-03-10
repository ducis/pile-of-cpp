#pragma once

#include "3d.h"
#include <algorithm>
#include "CSceneNodeBoard.h"

class CCutter{//用于存储墙角之类会切去柜体一部分的物体的类
public:
	const vector2 &Min() const{ return m_min; }
	const vector2 &Max() const{ return m_max; }
	//void SetIntersectingVolume(const AABB &aabb);
	//void CancelIntersectingVolume();
	CCutter(const vector2 &a, const vector2 &b): m_min(std::min(a.x,b.x), std::min(a.y,b.y)),m_max( std::max(a.x,b.x), std::max(a.y,b.y) ){}
	//Cutter是二维的,只有x,y方向上才有范围,z方向上是无限长的
	struct GuardData{//guard指的是柜体被切后新出现的板材所占的区域
		float minZ;
		float maxZ;
		vector2 thickness;
		AABB volumeOfEffect;
		GuardData():minZ(-100),maxZ(-50),thickness(0,0),volumeOfEffect(MinimumAABB()){}
	};

	const GuardData &GetGuard(bool positiveX,bool positiveY) const{//一个cutter在四个面上可能会有guard(不考虑一个墙角切多个柜体的情况),positive?就是指在某轴上是正方向的一面还是负方向的一面.
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
