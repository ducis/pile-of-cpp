#pragma once 
#include "ISceneNode.h"


class CSceneNodeCoordSys:public ISceneNode{
public://������ʾ����ϵ����
	void Render() const;
	CSceneNodeCoordSys(float sz = 1000):m_size(sz){}
private:
	float m_size;
};
