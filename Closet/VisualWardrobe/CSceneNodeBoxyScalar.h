#pragma once

#include "ISceneNodeBoxy.h"
#include <memory>

class CSceneNodeBoxyScalar: public ISceneNodeBoxy{//用于通过缩放将ISceneNode包装为ISceneNodeBoxy的类
public:
	CSceneNodeBoxyScalar( ISceneNode *pSN, const AABB &aabb );
	virtual void Render() const;
	virtual void OnInsertion();
	virtual void UpdateTransform(const matrix44 &parentTransform);
	
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
private:
	matrix44 m_parentTransform;
	std::auto_ptr<ISceneNode> m_pSN;
	AABB m_aabb;
};
