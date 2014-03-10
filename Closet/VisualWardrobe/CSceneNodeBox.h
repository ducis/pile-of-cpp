#pragma once

#include "ISceneNode.h"

class CSceneNodeBox: public ISceneNode{
public:
	//在当前坐标原点显示长方体的类
	void Render() const;
	explicit CSceneNodeBox(const vector3 &size);
private:
	vector3 m_size;
};
