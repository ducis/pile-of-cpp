#pragma once

#include "3d.h"
#include "IConnectible.h"
#include "CSceneNodeWardrobeSelector.h"
#include "CSceneNodeCutterGuard.h"
#include "IAnnotated.h"
#include <memory>

class CSceneNodeWardrobe:public ISceneNodeBoxy, public IConnectible,public IAnnotated{
public:
	//作为场景节点的柜体类．一个柜体在场景中的表示应该是这个类
	virtual void EnlargeAABB(AABB &aabb) const{ m_pSelector->EnlargeAABB(aabb); }
	virtual void Render() const;
	virtual void OnInsertion(){ m_pSelector->OnInsertion(); m_aabb = m_pSelector->GetAABB(); }
	virtual void UpdateTransform(const matrix44 &parentTransform);
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const{ return m_aabb; }
	//CSceneNodeWardrobe(CSceneNodeWardrobeSelector &selector, CSceneNodeCutterGuard &cutterGuardA, CSceneNodeCutterGuard &cutterGuardB);//:m_pSelector(&selector){}
	virtual bool ConnectTo(const IConnectible &theOther){
		const CSceneNodeWardrobe *pOther( dynamic_cast<const CSceneNodeWardrobe*>(&theOther) );
		return pOther? m_pSelector->ConnectTo(*pOther->m_pSelector) : false;
	}
	virtual bool DisconnectFrom(const IConnectible &theOther){
		const CSceneNodeWardrobe *pOther( dynamic_cast<const CSceneNodeWardrobe*>(&theOther) );
		return pOther? m_pSelector->DisconnectFrom(*pOther->m_pSelector) : false;
	}
	virtual void ShowAnnotation() const;

	CSceneNodeWardrobe(CSceneNodeWardrobeSelector &selector);
	~CSceneNodeWardrobe();

private:
	AABB m_aabb;
	matrix44 m_worldTransform;
	std::auto_ptr<CSceneNodeWardrobeSelector> m_pSelector;
	std::auto_ptr<CSceneNodeCutterGuard> m_pCutterGuardMinY,m_pCutterGuardMaxY;

	void SetCutterGuardsAABB();
	void CheckCutterIntersection();
	void CheckCutterIntersection_impl(class CCutter *);
};
