#include "CSceneNodeRotationCoordSysAxis.h"
#include "3d.h"

template<unsigned int _iAxis>
void CSceneNodeRotationCoordSysAxis<_iAxis>::Render() const{
	glPushMatrix();
	glRotatef(m_rotInDegree,s_axis.x,s_axis.y,s_axis.z);
	CSceneNodeDecorator::Render();
	glPopMatrix();
}


template<unsigned int _iAxis>
void CSceneNodeRotationCoordSysAxis<_iAxis>::UpdateTransform(const matrix44 &parentTransform){
	CSceneNodeDecorator::UpdateTransform(
		parentTransform
		*RotateRadMatrix44(s_axisChar,m_rotInDegree*(PI/180.0))
		);
}


vector3 CSceneNodeRotationCoordSysAxis<0>::s_axis(1,0,0);
vector3 CSceneNodeRotationCoordSysAxis<1>::s_axis(0,1,0);
vector3 CSceneNodeRotationCoordSysAxis<2>::s_axis(0,0,1);

char CSceneNodeRotationCoordSysAxis<0>::s_axisChar('x');
char CSceneNodeRotationCoordSysAxis<1>::s_axisChar('y');
char CSceneNodeRotationCoordSysAxis<2>::s_axisChar('z');


template class CSceneNodeRotationCoordSysAxis<0>;
template class CSceneNodeRotationCoordSysAxis<1>;
template class CSceneNodeRotationCoordSysAxis<2>;
