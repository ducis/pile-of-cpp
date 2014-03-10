#include "CBoxyDraggableContainerAxialAnnotator.h"
#include "CSceneNodeBoxyDraggableContainer.h"
#include "3d.h"
#include "CAxialAABBAnnotation.h"
#include "singleton.h"
#include "CProgram.h"


CSceneNodeBoxyDraggableContainer::CAxialAnnotator::CAxialAnnotator(const CSceneNodeBoxyDraggableContainer &bdc):
m_bdc(bdc),
m_facing(1,0,0),
m_pts( 0, 1, 0, 1, 0, 1 ),
m_pAnnotation( NULL , NULL , NULL )
//m_pts(
//m_pts(0,0,0)
//m_isDead(false)
{
	OnSpatialChange();
}



void CSceneNodeBoxyDraggableContainer::CAxialAnnotator::OnSpatialChange()
{
	matrix44 world(m_bdc.GetTransform());
	vector3 newFacing((world*vector4(1,0,0,0)).xyz());
	if(DotProduct(newFacing,m_facing)<0.99f){
		m_facing = newFacing;
		for(int i=0; i<3; ++i){
			if(m_pAnnotation[i]){
				for(int j=0;j<2;++j)	m_pAnnotation[i]->UnregisterPoint(&m_pts.As2D<2>(i,j));
			}
			m_pAnnotation[i] = GetSingleton<CProgram>()->GetWallAnnotation(m_facing,i);
			if(m_pAnnotation[i]){
				for(int j=0;j<2;++j)	m_pAnnotation[i]->RegisterPoint(&m_pts.As2D<2>(i,j));
			}
		}
	}
	AABB aabb(world*m_bdc.GetAABB());
	for(int i=0; i<3; ++i){
		m_pts.As2D<2>(i,0) = aabb.first[i];
		m_pts.As2D<2>(i,1) = aabb.second[i];
	}
}



