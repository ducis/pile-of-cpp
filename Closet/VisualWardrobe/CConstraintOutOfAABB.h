#pragma once

#include "CConstraintAABB.h"

class CConstraintOutOfAABB:public CConstraintAABB{//���Ƶ���ĳ��AABB�ⲿ��Լ��
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const;
	virtual bool Violated(const vector3 &x, const AABB &aabb) const;
	CConstraintOutOfAABB(const AABB &aabb):CConstraintAABB(aabb){}
};
