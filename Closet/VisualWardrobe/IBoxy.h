#pragma once

#include "3d.h"

class IBoxy{//��һ���Ŀɱ��С�ķ�������(�����,����)�Ľӿ�
public:
	virtual void SetAABB(const AABB &aabb)=0;
	virtual const AABB GetAABB() const =0;
	//ע��SetAABBֻ�Ǹ��߶���Ҫ��Ϊĳ����С,������󲢲�һ���ܹ���ı�Ϊ�����С,
	//����֮�����ڵ���SetAABB(x)�����ϵ���GetAABB()���ص�ֵҲ��һ������x
protected:
	~IBoxy(){}
};
