#pragma once
#include "ISceneNode.h"

class CSceneNodeGLUTWireCube:public ISceneNode{//������ʾ��
public:
	void Render() const;
};

class CSceneNodeGLUTSolidCube:public ISceneNode{//������ʾ��
public:
	void Render() const;
};

