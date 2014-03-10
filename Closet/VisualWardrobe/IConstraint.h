#pragma once

#include "3d.h"

class IConstraint{//约束的接口
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const=0;
		//欲将一个aabb从from移动到to,返回在符合此约束的条件下能够移动到什么地方.
	virtual bool Violated(const vector3 &x, const AABB &aabb) const=0;
		//返回在x处的aabb是否违反了此约束
};

