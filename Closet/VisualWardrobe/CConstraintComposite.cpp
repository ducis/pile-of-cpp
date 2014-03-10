#include "CConstraintComposite.h"

const vector3 CConstraintComposite::Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const{
	vector3 r(to);
	for(std::set<IConstraint*>::const_iterator i(m_constraints.begin()); i!=m_constraints.end(); ++i){
		r = (*i)->Constrain(from,r,aabb);
	}
	if(Violated(r,aabb))	return from;
	return r;
}



void CConstraintComposite::AddConstraint( IConstraint *p ){
	m_constraints.insert(p);
}



void CConstraintComposite::RemoveConstraint( IConstraint *p ){
	m_constraints.erase(p);
}


bool CConstraintComposite::Violated(const vector3 &x, const AABB &aabb) const{
	for(std::set<IConstraint*>::const_iterator i(m_constraints.begin()); i!=m_constraints.end(); ++i){
		if((*i)->Violated(x,aabb)) return true;
	}
	return false;
}