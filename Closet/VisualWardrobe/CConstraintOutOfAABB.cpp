#include "CConstraintOutOfAABB.h"


const vector3 CConstraintOutOfAABB::Constrain(const vector3 &from, const vector3 &to, const AABB &aabb) const{
	AABB constrainer(GetAABB());
	constrainer.first-=aabb.second;
	constrainer.second-=aabb.first;
	if(!IsValid(constrainer)){
		assert(0);
		return from;
	}
	//if(!Contains(constrainer,to)){
	if(!OpenAABBContains(constrainer,to)){
		return to;
	}else if(OpenAABBContains(constrainer,from)){
		return from;
	}else{
		for(int iAxis(0); iAxis<3; ++iAxis){
			//if(MyUtilities::IsInOpenRange(to[iAxis],constrainer.first[iAxis],constrainer.second[iAxis])) continue;
			if(from[iAxis]>=constrainer.second[iAxis]){
				bool go(true);
				for(int dAxis(1) ; go && dAxis<=2 ; ++dAxis){
					const int j((iAxis+dAxis)%3);
					vector2 lp(constrainer.first[j],constrainer.second[iAxis]);
					vector2 mp(from[j],from[iAxis]);
					vector2 rp(constrainer.second[j],constrainer.second[iAxis]);
					vector2 o(to[j],to[iAxis]);
					if(!MyUtilities::IsInClosedAngle(mp-o,lp-o,rp-o))	go = false;
				}
				if(go){
					vector3 r(to);
					r[iAxis] = constrainer.second[iAxis];
					return r;
				}
			}else if(from[iAxis]<=constrainer.first[iAxis]){
				bool go(true);
				for(int dAxis(1) ; go && dAxis<=2 ; ++dAxis){
					const int j((iAxis+dAxis)%3);
					vector2 lp(constrainer.second[j],constrainer.first[iAxis]);
					vector2 mp(from[j],from[iAxis]);
					vector2 rp(constrainer.first[j],constrainer.first[iAxis]);
					vector2 o(to[j],to[iAxis]);
					if(!MyUtilities::IsInClosedAngle(mp-o,lp-o,rp-o))	go = false;
				}
				if(go){
					vector3 r(to);
					r[iAxis] = constrainer.first[iAxis];
					return r;
				}
			}
		}
		assert(0);
		return from;
	}
}


bool CConstraintOutOfAABB::Violated(const vector3 &x, const AABB &aabb) const{
	AABB constrainer(GetAABB());
	constrainer.first-=aabb.second;
	constrainer.second-=aabb.first;
	if(!IsValid(constrainer)){
		assert(0);
		return true;
	}
	return OpenAABBContains(constrainer,x);
}