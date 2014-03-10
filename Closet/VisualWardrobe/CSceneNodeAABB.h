#pragma once

#include "ISceneNodeBoxy.h"
#include "array.h"

class CSceneNodeAABB:public ISceneNodeBoxy{
public:
	//显示长方体的类
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	CSceneNodeAABB(const AABB &aabb);
private:
	AABB m_aabb;
	void DrawFace(const vector3 &from, const vector3 &d0,const vector3 &d1, int div0, int div1) const;
	void DrawFace(const FixedArray<vector3,4> &r,unsigned int divA,unsigned int divB) const;
};
