#pragma once

#include "IConstraint.h"

class CConstraintAABB:public IConstraint{//≤ª π”√
public:
	const AABB &GetAABB() const{ return m_aabb; }
	void SetAABB( const AABB &aabb ) { m_aabb = aabb; }
protected:
	CConstraintAABB(const AABB &aabb):m_aabb(aabb){}
private:
	AABB m_aabb;
};