#pragma once

#include "3d.h"

class ISceneNode{//场景图节点接口
public:
	virtual ~ISceneNode(){}

	virtual void EnlargeAABB(AABB &aabb) const{}//不使用
	virtual void Render() const = 0;//显示
	virtual void OnInsertion(){};//插入场景后需要做的一些动作
	virtual void UpdateTransform(const matrix44 &parentTransform){}//某节点相对于世界坐标的transform变动后需要调用子树的这个函数将子树上每个节点的transform更新(因为有些节点需要存储自己的世界坐标)
};
