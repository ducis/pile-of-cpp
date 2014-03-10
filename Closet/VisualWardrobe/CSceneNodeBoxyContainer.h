#pragma once

#include "IBoxy.h"
#include "CSceneNodeDecorator.h"

class CSceneNodeBoxyContainer:public CSceneNodeDecorator,public IBoxy{
public:
	virtual void Render() const;
	virtual void SetAABB(const AABB &aabb);
	virtual const AABB GetAABB() const;
	CSceneNodeBoxyContainer(const AABB &aabb);
private:
	AABB m_aabb;
};