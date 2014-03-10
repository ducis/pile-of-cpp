#pragma once
#include "CSceneNodeBoxyRepositioner.h"
#include "per_element.h"
#include "lerp.h"

using namespace per_element;
using namespace MyUtilities;
using namespace std;

void CSceneNodeBoxyRepositioner::SetAABB(const AABB &aabb){ 
	AABB newAABB(aabb);
	if(m_flags[Enclosing]){
		vector3 dim(GetDim(CSceneNodeBoxyDecorator::GetAABB()));
		for(int iAxis(0); iAxis<3; ++iAxis){
			ClampL(newAABB.second[iAxis],min(m_aabb.second[iAxis],m_aabb.first[iAxis]+dim[iAxis]));
			ClampR(newAABB.first[iAxis],max(m_aabb.first[iAxis],m_aabb.second[iAxis]-dim[iAxis]));
		}
	}
	m_aabb = newAABB; 
	CSceneNodeBoxyDecorator::SetAABB( AABB(
		Lerp(m_aabb.first,m_aabb.second,m_remapping.first.Scale())+m_remapping.first.Offset(),
		Lerp(m_aabb.first,m_aabb.second,m_remapping.second.Scale())+m_remapping.second.Offset() ) ); 
}


void CSceneNodeBoxyRepositioner::Set(Flags flag){
	SetEnabled(flag,true);
}


void CSceneNodeBoxyRepositioner::Clear(Flags flag){
	SetEnabled(flag,false);
}


void CSceneNodeBoxyRepositioner::SetEnabled(Flags flag, bool enabled){
	m_flags.set(flag,enabled);
}


bool CSceneNodeBoxyRepositioner::Get(Flags flag){
	return m_flags[flag];
}
