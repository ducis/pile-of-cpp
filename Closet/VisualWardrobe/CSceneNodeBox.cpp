#include "CSceneNodeBox.h"

#include "3d.h"

CSceneNodeBox::CSceneNodeBox(const vector3 &size):m_size(size){}

void CSceneNodeBox::Render() const{
	glPushMatrix();
	glScalef(m_size.x, m_size.y, m_size.z);
	glutSolidCube(1.0);
	glPopMatrix();
}
