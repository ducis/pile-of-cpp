#pragma once 
#include "ISceneNode.h"
#include <memory>
class CSceneNodeDecorator:public ISceneNode{//应用Decorator模式的类的基类
public:
	void Render() const{ m_pDecorated->Render();}
	void OnInsertion() { m_pDecorated->OnInsertion();}
	void EnlargeAABB(AABB &aabb)const{ m_pDecorated->EnlargeAABB(aabb); }
	void UpdateTransform(const matrix44 &parentTransform){ m_pDecorated->UpdateTransform(parentTransform); }
	CSceneNodeDecorator(ISceneNode *pDecorated):m_pDecorated(pDecorated){}

private:
	std::auto_ptr<ISceneNode> m_pDecorated;
};
