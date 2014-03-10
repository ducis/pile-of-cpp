#include "CDraggableAABBSurface.h"
#include "CObjectConstRef.h"
#include "CCoordSysAxis.h"
#include "CProgram.h"
#include "singleton.h"
#include "IScene.h"
#include "CConstraintComposite.h"
#include "CAnnotationStyle.h"

template<class Transform>
CDraggableAABBSurface<Transform>::CDraggableAABBSurface( IBoxy *pBoxy, Type type, const Transform &transform, IConstraint *pSelfConstraint)
:m_pBoxy(pBoxy),m_type(type),m_transform(transform),m_pSelfConstraint(pSelfConstraint)
{
}



template<class Transform>
float CDraggableAABBSurface<Transform>::Pick(const vector2 & posHS, const matrix44 &viewProj) const{
	matrix44 fullTransform( viewProj*m_transform );
	AABB aabb(GetPickingAABB());
	for(int iAxis(0); iAxis<3; ++iAxis){
		if(GetAxisId()==iAxis) continue;
		CCoordSysAxis axis(iAxis);
		if( abs((fullTransform*axis.Vector4()).xyz().normalized().z)>0.99f ){
			AABB aabbHS(AABBFromTwoPoints( (fullTransform*vector4(aabb.first,1.0f)).xyz() , (fullTransform*vector4(aabb.second,1.0f)).xyz() ));
			if(1+aabbHS.first.z>0 && MyUtilities::IsInClosedRect(posHS,aabbHS.first.xy(),aabbHS.second.xy())){
				return 1+aabbHS.first.z;
			}
			return 2000000.0f;
		}
	}
	return 2000000.0f;
}


template<class Transform>
const AABB CDraggableAABBSurface<Transform>::GetPickingAABB() const{
	AABB aabb(m_pBoxy->GetAABB());
	const float dA(18.0f),dB(0.0f);
	if(m_type%2){
		aabb.first[GetAxisId()] = aabb.second[GetAxisId()]-dA;
		aabb.second[GetAxisId()]+=dB;
	}else{
		aabb.second[GetAxisId()] = aabb.first[GetAxisId()]+dA;
		aabb.first[GetAxisId()]-=dB;
	}
	return aabb;
}


template<class Transform>
void CDraggableAABBSurface<Transform>::VisualizePick() const{
	//GetSingleton<CAnnotationStyle>()->DrawAABBSelection(m_transform*GetPickingAABB());
}


template<class Transform>
const vector2 CDraggableAABBSurface<Transform>::Drag(const vector2 &displacementHS, const matrix44 &viewProj){
	AABB aabb(m_pBoxy->GetAABB());
	matrix44 fullTransform( viewProj*m_transform );
	matrix44 fullTransformInv( fullTransform );
	fullTransformInv.invert();
	vector3 delta( (fullTransformInv*vector4(displacementHS.x,displacementHS.y,0,0)).xyz() );
	{
		CConstraintComposite *pComposite(GetSingleton<CProgram>()->GetScene()->GetConstraintComposite());
		if(m_pSelfConstraint) pComposite->RemoveConstraint(m_pSelfConstraint);
		std::for_each( m_pConstraintsToDisable.begin(),m_pConstraintsToDisable.end(),std::bind1st(std::mem_fun(&CConstraintComposite::RemoveConstraint),pComposite) );
		float l(0),r(delta[GetAxisId()]);
		AABB newAabb(aabb);
		while(abs(r-l)>0.00001f){
			float m(0.5f*(l+r));
			if(m_type%2){
				newAabb.second[GetAxisId()] = max(aabb.first[GetAxisId()] , aabb.second[GetAxisId()]+m);
			}else{
				newAabb.first[GetAxisId()] = min(aabb.second[GetAxisId()] , aabb.first[GetAxisId()]+m);
			}
			if(GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->Violated(vector3(0,0,0) , m_transform*newAabb)){
				r = m;
			}else{
				l = m;
			}
		}
		if(m_type%2){
			newAabb.second[GetAxisId()] = max(aabb.first[GetAxisId()] , aabb.second[GetAxisId()]+int(l));
		}else{
			newAabb.first[GetAxisId()] = min(aabb.second[GetAxisId()] , aabb.first[GetAxisId()]+int(l));
		}
		std::for_each( m_pConstraintsToDisable.begin(),m_pConstraintsToDisable.end(),std::bind1st(std::mem_fun(&CConstraintComposite::AddConstraint),pComposite) );
		if(m_pSelfConstraint) pComposite->AddConstraint(m_pSelfConstraint);
		m_pBoxy->SetAABB(newAabb);
	}
	vector3 r(0,0,0);
	if(m_type%2){
		r[GetAxisId()] = m_pBoxy->GetAABB().second[GetAxisId()] - aabb.second[GetAxisId()];
	}else{
		r[GetAxisId()] = m_pBoxy->GetAABB().first[GetAxisId()] - aabb.first[GetAxisId()];
	}
	return (fullTransform * vector4(r,0.0f)).xy();
}



template CDraggableAABBSurface< matrix44 >;
template CDraggableAABBSurface< CObjectConstRef<matrix44> >;
