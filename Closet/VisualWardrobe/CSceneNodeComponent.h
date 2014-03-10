#pragma once 
#include "CSceneNodeDecorator.h"
#include "mtxlib.h"
class CSceneNodeComponent: public CSceneNodeDecorator{//����λ�ú���ת��Decorator
public:
	virtual void Render() const;
	virtual void UpdateTransform(const matrix44 &parentTransform);
	CSceneNodeComponent(ISceneNode *pObj, std::wistream &arg);
private:
	int m_posInMillimeter[3];
	int m_rotInDegree;
};
