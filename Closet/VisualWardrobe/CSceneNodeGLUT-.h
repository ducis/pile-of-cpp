#pragma once
#include "ISceneNode.h"

class CSceneNodeGLUTWireCube:public ISceneNode{//测试显示用
public:
	void Render() const;
};

class CSceneNodeGLUTSolidCube:public ISceneNode{//测试显示用
public:
	void Render() const;
};

