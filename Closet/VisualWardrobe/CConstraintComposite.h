#pragma once

#include "IConstraint.h"
#include <set>

class CConstraintComposite{//���Լ�����ɵ�����Լ��
public:
	virtual const vector3 Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const;
	virtual bool Violated(const vector3 &x, const AABB &aabb) const;
	void AddConstraint( IConstraint *p );
	void RemoveConstraint( IConstraint *p );
private:
	std::set<IConstraint*> m_constraints;
};
