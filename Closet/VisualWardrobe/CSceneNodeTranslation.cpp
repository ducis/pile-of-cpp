#include "CSceneNodeTranslation.h"
#include "3d.h"

void CSceneNodeTranslation::Render() const{
	glPushMatrix();
	glTranslatef(m_translation.x,m_translation.y,m_translation.z);
	CSceneNodeDecorator::Render();
	glPopMatrix();
}


const vector3 CSceneNodeTranslation::GetTranslation(){
	return m_translation;
}


void CSceneNodeTranslation::SetTranslation(const vector3 &translation){
	m_translation = translation;
	CSceneNodeDecorator::UpdateTransform(m_parentTransform*TranslateMatrix44(m_translation.x,m_translation.y,m_translation.z));
}


void CSceneNodeTranslation::UpdateTransform(const matrix44 &parentTransform){
	m_parentTransform = parentTransform;
	CSceneNodeDecorator::UpdateTransform(m_parentTransform*TranslateMatrix44(m_translation.x,m_translation.y,m_translation.z));
}
