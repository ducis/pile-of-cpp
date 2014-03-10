#pragma once

#include "IPickable.h"
#include "IDraggable.h"

class CViewPanner:public IPickable,public IDraggable{
public:
	//实现鼠标拖动画面的类,注意这个是直接调用OpenGL的,不存储状态,与CTargetCamera不相容
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const{ return 0.0f; }
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj);
	virtual void VisualizePick() const{}
};
