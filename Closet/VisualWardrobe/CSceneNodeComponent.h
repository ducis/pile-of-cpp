#pragma once 
#include "CSceneNodeDecorator.h"
#include "mtxlib.h"
class CSceneNodeComponent: public CSceneNodeDecorator{//带有位置和旋转的Decorator
public:
	virtual void Render() const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	CSceneNodeComponent(ISceneNode *pObj, std::wistream &arg);
private:
	int m_posInMillimeter[3];
	int m_rotInDegree;
};
