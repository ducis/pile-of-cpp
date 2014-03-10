#pragma once

#include "3d.h"

class IConstraint{//Լ���Ľӿ�
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const=0;
		//����һ��aabb��from�ƶ���to,�����ڷ��ϴ�Լ�����������ܹ��ƶ���ʲô�ط�.
	virtual bool Violated(const vector3 &x, const AABB &aabb) const=0;
		//������x����aabb�Ƿ�Υ���˴�Լ��
};

