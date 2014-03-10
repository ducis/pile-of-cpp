#pragma once

#include "mtxlib.h"

class IPickable{//可选取的对象的接口
public:
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const=0;//选取判定
	virtual void VisualizePick() const=0;//显示选取状态
};
