#include "CConstraintInsideOfAABB.h"


const vector3 CConstraintInsideOfAABB::Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const{
	AABB constrainer(GetAABB());
	constrainer.first-=aabb.first;
	constrainer.second-=aabb.second;
	if(!IsValid(constrainer)){
		assert(0);
		return from;
	}
	return Clamped(constrainer,to);
}


bool CConstraintInsideOfAABB::Violated(const vector3 &x, const AABB &aabb) const{
	AABB constrainer(GetAABB());
	constrainer.first-=aabb.first;
	constrainer.second-=aabb.second;
	if(!IsValid(constrainer)){
		assert(0);
		return true;
	}
	return !ClosedAABBContains(constrainer,x);
}