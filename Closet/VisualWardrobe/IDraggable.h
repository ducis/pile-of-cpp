#pragma once

#include "IPickable.h"
#include "ITranslate.h"

class IDraggable{//�����϶�������Ľӿ�
public:
	virtual const vector2 Drag(const vector2 &displacementHS, const matrix44 &viewProj)=0;
	//��Ӧ�϶��ĺ���,displacementHS���������οռ��е�λ��,
	//viewProj��view����任����projection����任,����OpenGL�Ĺ������projection*view
};