#pragma once

#include "IPickable.h"
#include "IDraggable.h"

class CViewPanner:public IPickable,public IDraggable{
public:
	//ʵ������϶��������,ע�������ֱ�ӵ���OpenGL��,���洢״̬,��CTargetCamera������
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const{ return 0.0f; }
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj);
	virtual void VisualizePick() const{}
};
