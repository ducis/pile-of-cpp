#pragma once

#include "IPickable.h"
#include "ITranslate.h"

class IDraggable{//可以拖动的物体的接口
public:
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj)=0;
	//响应拖动的函数,displacementHS是鼠标在齐次空间中的位移,
	//viewProj是view矩阵变换后再projection矩阵变换,根据OpenGL的规则就是projection*view
};