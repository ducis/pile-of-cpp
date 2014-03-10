#pragma once

#include "ISimplyNotified.h"
#include "CObjectSourceCached.h"
#include "CSceneNodeBoxyDraggableContainer.h"
#include "Array.h"

class CSceneNodeBoxyDraggableContainer::CAxialAnnotator{
public:
	void OnSpatialChange();
	CAxialAnnotator(const class CSceneNodeBoxyDraggableContainer &bdc);
	//void HasOutlivedUsefulness() const { return m_isDead; }
private:
	vector3 m_facing;
	const class CSceneNodeBoxyDraggableContainer &m_bdc;
	//typedef FixedArray< CObjectSourceCached<float> , 2 > T1;
	//typedef FixedArray< T1 , 3 > T2;
	FixedArray< CObjectSourceCached<float> , 3*2 > m_pts;
	FixedArray< class CAxialAABBAnnotation *, 3 > m_pAnnotation;
	//FixedArray2< float , 3 , 2 > m_pts;
};
