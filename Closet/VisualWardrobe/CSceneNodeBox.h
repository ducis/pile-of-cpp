#pragma once

#include "ISceneNode.h"

class CSceneNodeBox: public ISceneNode{
public:
	//�ڵ�ǰ����ԭ����ʾ���������
	void Render() const;
	explicit CSceneNodeBox(const vector3 &size);
private:
	vector3 m_size;
};
