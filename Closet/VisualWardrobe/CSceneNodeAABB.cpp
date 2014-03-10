#include "CSceneNodeAABB.h"
#include "CProgram.h"
#include "singleton.h"



void CSceneNodeAABB::DrawFace(const FixedArray<vector3,4> &r,unsigned int divA,unsigned int divB) const{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_AUTO_NORMAL);
	glMap2f( GL_MAP2_VERTEX_3,0.0f,1.0f,3,2,0.0f,1.0f,6,2,(GLfloat*)r.PtrBegin());
	GLenum err(glGetError());
	glMapGrid2f(divA,0,1,divB,0,1);
	err = glGetError();
	glEvalMesh2(GL_FILL,0,divA,0,divB);
	err = glGetError();
	glPopAttrib();
}


void CSceneNodeAABB::DrawFace(const vector3 &from, const vector3 &d0,const vector3 &d1, int div0, int div1) const{
	DrawFace( FixedArray<vector3,4>( from,from+d0,from+d1,from+d0+d1 ),div0,div1 );
}


void CSceneNodeAABB::Render() const{
	if(!GetSingleton<CProgram>()->IsInWireFrameMode())
	{
		//glPushAttrib(GL_ALL_ATTRIB_BITS);
		////glBegin();
		//glEnable(GL_MAP2_VERTEX_3);
		//vector3 r[4] = { 
		//	m_aabb.first , 
		//	vector3(m_aabb.first.x,m_aabb.second.y,m_aabb.first.z) ,
		//	vector3(m_aabb.second.x,m_aabb.first.y,m_aabb.first.z) , 
		//	vector3(m_aabb.second.x,m_aabb.second.y,m_aabb.first.z)
		//};
		//glMap2f( GL_MAP2_VERTEX_3,0.0f,1.0f,6,2,0.0f,1.0f,3,2,(GLfloat*)r);
		//glMapGrid2f(10,0,1,20,0,1);
		//glEvalMesh2(GL_FILL,0,10,0,20);
		////glEnd();
		//glPopAttrib();
		vector3 dim(GetDim(m_aabb));
		vector3 dx(dim.x,0,0);
		vector3 dy(0,dim.y,0);
		vector3 dz(0,0,dim.z);
		unsigned int nx(dim.x*0.02+1);
		unsigned int ny(dim.y*0.02+1);
		unsigned int nz(dim.z*0.02+1);
		DrawFace(m_aabb.first,dy,dx,ny,nx);
		DrawFace(m_aabb.first,dx,dz,nx,nz);
		DrawFace(m_aabb.first,dz,dy,nz,ny);
		DrawFace(m_aabb.second,-dx,-dy,nx,ny);
		DrawFace(m_aabb.second,-dy,-dz,ny,nz);
		DrawFace(m_aabb.second,-dz,-dx,nz,nx);
		return;
	}
	glPushAttrib(GL_TEXTURE_BIT);
	glPushMatrix();
	vector3 center(GetCenter(m_aabb)),dim(GetDim(m_aabb));
	glTranslatef(center.x,center.y,center.z);
	{
		GLenum coord[2]={GL_S,GL_T};
		for(int iCoord(0); iCoord<2; ++iCoord)
		{
			float f[4];
			glGetTexGenfv(coord[iCoord],GL_OBJECT_PLANE,f);
			f[0]*=dim.x*0.001f;
			f[1]*=dim.y*0.001f;
			f[2]*=dim.z*0.001f;
			//f[2]*=1000;
			glTexGenfv(coord[iCoord],GL_OBJECT_PLANE,f);
		}
	}
	glScalef(dim.x,dim.y,dim.z);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopAttrib();
}



void CSceneNodeAABB::SetAABB(const AABB &aabb){
	m_aabb = aabb;
}


const AABB CSceneNodeAABB::GetAABB() const{
	return m_aabb;
}


CSceneNodeAABB::CSceneNodeAABB(const AABB &aabb):m_aabb(aabb){
}
