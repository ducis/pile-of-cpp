#include "CSceneNodeDoor.h"
#include "singleton.h"
#include "CProgram.h"


void CSceneNodeDoor::SetAABB(const AABB &aabb){
	m_aabb = aabb;
}



const AABB CSceneNodeDoor::GetAABB() const{
	return m_aabb;
}



void CSceneNodeDoor::Render() const{
	int style( GetSingleton<CProgram>()->GetConfigVar<int>(L"DoorStyle") );
	if(0==style) return;
	
	vector3 dim( GetDim(m_aabb) );
	vector3 center( GetCenter(m_aabb) );
	int n(int(dim.y/m_maxSingleDoorWidth)+2);
	float deltaY = dim.y/n;
	AABB aabb(m_aabb);
	aabb.second.y = aabb.first.y;
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for(int i=0;i<n;++i){
		aabb.first.y = aabb.second.y;
		aabb.second.y += deltaY;
		if(i&1){
			aabb.first.x = center.x;
			aabb.second.x = m_aabb.second.x;
		}else{
			aabb.first.x = m_aabb.first.x;
			aabb.second.x = center.x;
		}
		vector3 dim( GetDim(aabb) );
		vector3 center( GetCenter(aabb) );
		glEnable(GL_COLOR_MATERIAL);
		if(1==style){
			glPushMatrix();
			glTranslatef(center.x,center.y,center.z);
			glScalef(dim.x,dim.y,dim.z);
			glColor3f(0.8,0.7,0.5);
			glutWireCube(0.8);
			glutWireCube(0.1);
			glColor3f(0.7,0.6,0.4);
			glutWireCube(0.99);
			glutWireCube(0.5);
			glPopMatrix();
		}else{
			glColor3f(0.8,0.7,0.5);

			glPushMatrix();
			glTranslatef(center.x,center.y+dim.y*0.45,center.z);
			glScalef(dim.x,dim.y*0.1,dim.z);
			glutSolidCube(0.99);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(center.x,center.y-dim.y*0.45,center.z);
			glScalef(dim.x,dim.y*0.1,dim.z);
			glutSolidCube(0.99);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(center.x,center.y,center.z+dim.z*0.475);
			glScalef(dim.x,dim.y,dim.z*0.05);
			glutSolidCube(0.99);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(center.x,center.y,center.z-dim.z*0.475);
			glScalef(dim.x,dim.y,dim.z*0.05);
			glutSolidCube(0.99);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(center.x,center.y,center.z);
			glScalef(dim.x,dim.y*0.15,dim.z*0.1);
			glutSolidCube(1.0f);
			glPopMatrix();
			
			glColor3f(0.7,0.6,0.4);
			glPushMatrix();
			glTranslatef(center.x,center.y,center.z);
			glScalef(dim.x*0.5,dim.y,dim.z);
			glutSolidCube(1.0f);
			glPopMatrix();

						
		}
	}
	glPopAttrib();
}



CSceneNodeDoor::CSceneNodeDoor(const boost::property_tree::wptree &):m_maxSingleDoorWidth(1000.0f){
}


