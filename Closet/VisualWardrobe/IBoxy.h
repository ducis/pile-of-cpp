#pragma once

#include "3d.h"

class IBoxy{//有一定的可变大小的方形物体(如柜体,抽屉)的接口
public:
	virtual void SetAABB(const AABB &aabb)=0;
	virtual const AABB GetAABB() const =0;
	//注意SetAABB只是告诉对象要变为某个大小,这个对象并不一定能够真的变为这个大小,
	//换言之若是在调用SetAABB(x)后马上调用GetAABB()返回的值也不一定等于x
protected:
	~IBoxy(){}
};
