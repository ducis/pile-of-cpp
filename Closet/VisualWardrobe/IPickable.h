#pragma once

#include "mtxlib.h"

class IPickable{//��ѡȡ�Ķ���Ľӿ�
public:
	virtual float Pick(const vector2 & posHS, const matrix44 &viewProj) const=0;//ѡȡ�ж�
	virtual void VisualizePick() const=0;//��ʾѡȡ״̬
};
