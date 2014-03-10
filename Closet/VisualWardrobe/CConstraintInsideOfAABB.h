#pragma once

#include "CConstraintAABB.h"

class CConstraintInsideOfAABB:public CConstraintAABB{//限制点在某个AABB内部的约束
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const;
	virtual bool Violated(const vector3 &x, const AABB &aabb) const;
	CConstraintInsideOfAABB(const AABB &aabb):CConstraintAABB(aabb){}
};
