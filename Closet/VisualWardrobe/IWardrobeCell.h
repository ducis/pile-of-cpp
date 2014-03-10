#pragma once

#include <utility>
#include "3d.h"

class IWardrobeCell{//柜体内的格没有被切的格子(柜体树的叶节点)的接口
public:
	virtual void VisualizeCell()=0;
	virtual void ShowCellDimensions()=0;
	virtual float PickCell(const vector2 &posHS,const matrix44 &viewProj) const=0;//选取判定
	virtual void SplitCell(int iAxis)=0;//分裂该叶节点
	virtual void InsertContent(class ISceneNodeBoxy *p) = 0;//在格子中插入东西,比如抽屉等等
};
