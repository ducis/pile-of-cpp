#include "CSceneNodeBoxyScalar.h"


namespace CSceneNodeBoxyScalar_impl{
	const matrix44 GetTransform(const AABB &aabb){
		return TranslateMatrix44( GetCenter(aabb) )*ScaleMatrix44( GetDim(aabb) );
	}
}
using namespace CSceneNodeBoxyScalar_impl;


CSceneNodeBoxyScalar::CSceneNodeBoxyScalar( ISceneNode *pSN, const AABB &aabb ):m_pSN(pSN),m_aabb(aabb){
}



void CSceneNodeBoxyScalar::Render() const{
	glPushMatrix();
	const vector3 center(GetCenter(m_aabb)), dim(GetDim(m_aabb));
	glTranslatef(center.x,center.y,center.z);
	glScalef(dim.x,dim.y,dim.z);
	m_pSN->Render();
	//glutSolidCube(0.8f);
	glPopMatrix();
}



void CSceneNodeBoxyScalar::OnInsertion(){
	m_pSN->OnInsertion();
}



void CSceneNodeBoxyScalar::UpdateTransform(const matrix44 &parentTransform){
	m_parentTransform = parentTransform;
	m_pSN->UpdateTransform(m_parentTransform*GetTransform(m_aabb));
}



void CSceneNodeBoxyScalar::SetAABB(const AABB &aabb){
	m_aabb = aabb;
	UpdateTransform(m_parentTransform);
}



const AABB CSceneNodeBoxyScalar::GetAABB() const{
	return m_aabb;
}

