#pragma once

#include "CSceneNodeDecorator.h"
#include "mtxlib.h"

template<unsigned int _iAxis>
class CSceneNodeRotationCoordSysAxis: public CSceneNodeDecorator{//带有绕某坐标轴旋转的Decorator
public:
	virtual void Render() const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	explicit CSceneNodeRotationCoordSysAxis(ISceneNode *pObj, float rotInDegree = 0.0f):CSceneNodeDecorator(pObj),m_rotInDegree(rotInDegree){}
private:
	static vector3 s_axis;
	static char s_axisChar;
	float m_rotInDegree;
};

