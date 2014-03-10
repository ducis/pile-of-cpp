#pragma once

#include "ISceneNodeBoxy.h"

class CSceneNodeBoxyDecorator: public ISceneNodeBoxy{//Decorator模式的基类
public:
	virtual void Render() const{ m_pDecorated->Render();}
	virtual void OnInsertion() { m_pDecorated->OnInsertion();}
	virtual void EnlargeAABB(AABB &aabb)const{ m_pDecorated->EnlargeAABB(aabb); }
	virtual void UpdateTransform(const matrix44 &parentTransform){ m_pDecorated->UpdateTransform(parentTransform); }
	virtual void SetAABB(const AABB &aabb){ m_pDecorated->SetAABB(aabb); }
	virtual const AABB GetAABB() const{ return m_pDecorated->GetAABB(); }
	CSceneNodeBoxyDecorator(ISceneNodeBoxy *pDecorated):m_pDecorated(pDecorated){}

private:
	std::auto_ptr<ISceneNodeBoxy> m_pDecorated;
};
