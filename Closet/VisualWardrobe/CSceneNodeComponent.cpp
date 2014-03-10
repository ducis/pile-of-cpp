#include "CSceneNodeComponent.h"
#include "3d.h"
void CSceneNodeComponent::Render() const{
	glPushMatrix();
	glTranslatef(m_posInMillimeter[0],m_posInMillimeter[1],m_posInMillimeter[2]);
	glRotatef(m_rotInDegree,0,0,1);
	CSceneNodeDecorator::Render();
	glPopMatrix();
}

CSceneNodeComponent::CSceneNodeComponent(ISceneNode *pObj, std::wistream &arg):CSceneNodeDecorator(pObj){
	arg>>m_posInMillimeter>>m_rotInDegree;
}


void CSceneNodeComponent::UpdateTransform(const matrix44 &parentTransform){
	CSceneNodeDecorator::UpdateTransform(
		parentTransform
		*TranslateMatrix44(m_posInMillimeter[0],m_posInMillimeter[1],m_posInMillimeter[2])
		*RotateRadMatrix44('z',m_rotInDegree*(PI/180.0))
		);
}