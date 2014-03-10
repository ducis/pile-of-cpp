#pragma once

#include <utility>
#include "3d.h"

class IWardrobeCell{//�����ڵĸ�û�б��еĸ���(��������Ҷ�ڵ�)�Ľӿ�
public:
	virtual void VisualizeCell()=0;
	virtual void ShowCellDimensions()=0;
	virtual float PickCell(const vector2 &posHS,const matrix44 &viewProj) const=0;//ѡȡ�ж�
	virtual void SplitCell(int iAxis)=0;//���Ѹ�Ҷ�ڵ�
	virtual void InsertContent(class ISceneNodeBoxy *p) = 0;//�ڸ����в��붫��,�������ȵ�
};
