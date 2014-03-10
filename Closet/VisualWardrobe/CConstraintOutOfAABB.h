#pragma once

#include "CConstraintAABB.h"

class CConstraintOutOfAABB:public CConstraintAABB{//限制点在某个AABB外部的约束
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const;
	virtual bool Violated(const vector3 &x, const AABB &aabb) const;
	CConstraintOutOfAABB(const AABB &aabb):CConstraintAABB(aabb){}
};
