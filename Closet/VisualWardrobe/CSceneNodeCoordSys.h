#pragma once 
#include "ISceneNode.h"


class CSceneNodeCoordSys:public ISceneNode{
public://用于显示坐标系的类
	void Render() const;
	CSceneNodeCoordSys(float sz = 1000):m_size(sz){}
private:
	float m_size;
};
