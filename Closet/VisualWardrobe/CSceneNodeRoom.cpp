#include "CSceneNodeRoom.h"
#include "3d.h"
#include <string>
#include <sstream>
#include "CProgram.h"
#include "singleton.h"
#include "CControlPlaneSystem.h"
#include <stdlib.h>
#include "IScene.h"
#include "CConstraintComposite.h"
#include "CCoordSysAxis.h"
#include "IMaterial.h"

using namespace std;
void CSceneNodeRoom::Render() const{
	//glColor3f(1,1,1);
	if(!GetSingleton<CProgram>()->GetConfigVar<bool>(L"ShowRoom")) return;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
	//glDisable(GL_COLOR_MATERIAL);
	//GLenum err = glGetError();
	//{
	//	float r[4] = {1,0,0,0};
	//	glMaterialfv( GL_FRONT, GL_DIFFUSE ,r);
	//	
	//	float g[4] = {0,1,0,0};
	//	glMaterialfv( GL_BACK, GL_DIFFUSE ,g);
	//}
	{
		float w[4] = {1,1,1,0};
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,w );
	}
	glShadeModel(GL_FLAT);
	if(GetSingleton<CProgram>()->IsInWireFrameMode()){
		glDisable(GL_CULL_FACE);
	}else{
		glEnable(GL_CULL_FACE);
	}

	static const int d[5][2] = {
		{0,0},
		{1,0},
		{1,1},
		{0,1},
		{0,0}
	};
	static const int dc[13][2] = {
		{0,1},
		{1,1},
		{1,0},

		{-1,0},
		{-1,1},
		{0,1},

		{0,-1},
		{-1,-1},
		{-1,0},
		
		{1,0},
		{1,-1},
		{0,-1},

		{0,1}
	};
	//for(int i=0;i<13;++i){
	//	for(int j=0;j<2;++j){
	//		glVertex3i(
	//			m_sizeInMillimeters[0]*d[i/3][0] + m_sizeCornerInMillimeters[i/3%4][0]*dc[i][0],
	//			m_sizeInMillimeters[1]*d[i/3][1] + m_sizeCornerInMillimeters[i/3%4][1]*dc[i][1],
	//			j*m_sizeInMillimeters[2]
	//		);
	//	}
	//}
	static const int n[12][2] = {
		{0,1},
		{1,0},
		{0,1},
		{-1,0},
		{0,1},
		{-1,0},
		{0,-1},
		{-1,0},
		{0,-1},
		{1,0},
		{0,-1},
		{1,0}
	};

	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(m_pMtlWalls)		m_pMtlWalls->Apply();
	for(int i=0;i<12;++i){
		if(abs(n[i][0])>abs(n[i][1])){
			m_texCoordGenYZ.Apply();
		}else{			
			m_texCoordGenZX.Apply();
		}
		glBegin(GL_QUADS);
		//if(i&1) continue;
		glNormal3f(n[i][0],n[i][1],0);
		//glNormal3f(1.0,0,0);
		for(int j=0;j<2;++j){
			for(int k=0;k<2;++k){
				glVertex3f(
					m_sizeInMillimeters[0]*d[(i+j)/3][0] + m_sizeCornerInMillimeters[(i+j)/3%4][0]*dc[i+j][0],
					m_sizeInMillimeters[1]*d[(i+j)/3][1] + m_sizeCornerInMillimeters[(i+j)/3%4][1]*dc[i+j][1],
					m_sizeInMillimeters[2] * (j?(1-k):k)
					);
			}
		}
		glEnd();
	}
	glPopAttrib();

	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		m_texCoordGenXY.Apply();
		if(m_pMtlFloor)		m_pMtlFloor->Apply();
		const int (&sz)[3] (m_sizeInMillimeters);
		glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(sz[0],0,0);
		glVertex3f(sz[0],sz[1],0);
		glVertex3f(0,sz[1],0);
		glEnd();
		glPopAttrib();
	}

	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		m_texCoordGenXY.Apply();
		if(m_pMtlCeiling)	m_pMtlCeiling->Apply();
		const int (&sz)[3] (m_sizeInMillimeters);
		glBegin(GL_QUADS);
		glNormal3f(0,0,-1);
		glVertex3f(0,0,sz[2]);
		glVertex3f(0,sz[1],sz[2]);
		glVertex3f(sz[0],sz[1],sz[2]);
		glVertex3f(sz[0],0,sz[2]);
		glEnd();
		glPopAttrib();
	}
	//glColor3f(1,0,0);
	//glDisable(GL_LIGHTING);
	//if(rand()%2){
	//	for(int i=0; i<6; ++i){
	//		m_pCPs[1][i]->Render();
	//	}
	//}
	glPopAttrib();
}

CSceneNodeRoom::CSceneNodeRoom(const boost::property_tree::wptree &tr):
m_constraint(AABB(vector3(0,0,0),vector3(1,1,1))),
m_texCoordGenXY(vector4(0.002,0,0,0), vector4(0,0.002,0,0)),
m_texCoordGenYZ(vector4(0,0.002,0,0), vector4(0,0,0.002,0)),
m_texCoordGenZX(vector4(0,0,0.002,0), vector4(0.002,0,0,0)),
m_pMtlFloor(NULL),m_pMtlWalls(NULL),m_pMtlCeiling(NULL)
{
	using namespace boost::property_tree;
	wistringstream(tr.get_child(L"Size").data()) >> m_sizeInMillimeters;
	wistringstream(tr.get_child(L"CornerSize").data()) >> m_sizeCornerInMillimeters;
	int x = m_sizeInMillimeters[0];
	int y = m_sizeInMillimeters[1];
#define SC m_sizeCornerInMillimeters
	int vxy[2][6][2][2]={
		{
			{{SC[0][0] ,	0},				{x-SC[1][0] ,	0}},
			{{0 ,			SC[0][1]},		{SC[0][0] ,		SC[0][1]}},
			{{x-SC[1][0] ,	SC[1][1]},		{x ,			SC[1][1]}},
			{{0 ,			y-SC[3][1]},	{SC[3][0] ,		y-SC[3][1]}},
			{{x-SC[2][0] ,	y-SC[2][1]},	{x ,			y-SC[2][1]}},
			{{SC[3][0] ,	y},				{x-SC[2][0] ,	y}}
		},
		{
			{{0 ,			SC[0][1]},		{0 ,			y-SC[3][1]}},
			{{SC[0][0] ,	0},				{SC[0][0] ,		SC[0][1]}},
			{{SC[3][0] ,	y-SC[3][1]},	{SC[3][0] ,		y}},
			{{x-SC[1][0] ,	0},				{x-SC[1][0] ,	SC[1][1]}},
			{{x-SC[2][0] ,	y-SC[2][1]},	{x-SC[2][0] ,	y}},
			{{x ,			SC[1][1]},		{x ,			y-SC[2][1]}}
		}
	};
	m_cutters.Ctor(0,vector2(-10,-10),vector2(SC[0][0],SC[0][1]));
	m_cutters.Ctor(1,vector2(x+10,-10),vector2(x-SC[1][0],SC[1][1]));
	m_cutters.Ctor(2,vector2(x+10,y+10),vector2(x-SC[2][0],y-SC[2][1]));
	m_cutters.Ctor(3,vector2(-10,y+10),vector2(SC[3][0],y-SC[3][1]));
	for(CCutter *p( m_cutters.PtrBegin() ); p<m_cutters.PtrEnd(); ++p ){
		GetSingleton<CProgram>()->GetScene()->RegisterCutter(p);
	}
#undef SC
	for(int iAxis=0; iAxis<2; ++iAxis){
		for(int iCP=0; iCP<6; ++iCP){
			GetSingleton<CProgram>()->GetAxisAlignedCPSystem((iAxis+1)%2)->Insert(m_pCPs[iAxis][iCP] = new CControlPlane(*this,
				vector3(vxy[iAxis][iCP][0][0],vxy[iAxis][iCP][0][1],0),
				vector3(vxy[iAxis][iCP][1][0],vxy[iAxis][iCP][1][1],m_sizeInMillimeters[2]) ));
		}
	}
	m_constraint.SetAABB(AABB(vector3(-0.01f,-0.01f,-100.01f),vector3(m_sizeInMillimeters[0]+0.01f,m_sizeInMillimeters[1]+0.01f,m_sizeInMillimeters[2]+0.01f)));
	GetSingleton<CProgram>()->GetScene()->GetConstraintComposite()->AddConstraint(&m_constraint);
	float d(50.0f);
	vector3 offset[4] = {
		vector3(-d,-d,-d),
		vector3(-d,d+m_sizeInMillimeters[1],-d),
		vector3(d+m_sizeInMillimeters[0],-d,-d),
		vector3(d+m_sizeInMillimeters[0],d+m_sizeInMillimeters[1],-d)
	};
	vector3 facing[4] = {
		vector3(1,0,0),
		vector3(0,-1,0),
		vector3(0,1,0),
		vector3(-1,0,0)
	};
	//std::pair<float,float> range[4][3] = {
	//	{
	//		make_pair(
	//	},
	//	{
	//	},
	//	{
	//	},
	//	{
	//	}
	//};
	for(int i=0;i<4;++i){
		for(int j=0;j<3;++j){
			vector3 off(offset[i]);
			off[j] = 0;
			m_axialAABBAnnotations.Ctor(i*3+j,CCoordSysAxis(j).Vector(),make_pair(0,float(m_sizeInMillimeters[j])),off);
			GetSingleton<CProgram>()->RegisterWallAnnotation(&m_axialAABBAnnotations[i*3+j], facing[i], j);
		}
	}
	GetSingleton<CProgram>()->RegisterOrderedObject(*this);
}

CSceneNodeRoom::CControlPlane::CControlPlane(const CSceneNodeRoom &room, vector3 a, vector3 b):m_room(room){
	m_vs[0] = a;
	m_vs[2] = b;
	for(int iAxis = 0; iAxis<3; ++iAxis){
		if(a[iAxis] == b[iAxis]){
			m_vs[1][iAxis] = m_vs[3][iAxis] = a[iAxis];
			int u = (iAxis+1)%3;
			int v = (iAxis+2)%3;
			m_vs[1][u] = a[u];
			m_vs[1][v] = b[v];
			m_vs[3][u] = b[u];
			m_vs[3][v] = a[v];
			return;
		}
	}
	assert(0);
	return;
}

void CSceneNodeRoom::CControlPlane::Render() const{
	glBegin(GL_POLYGON);
	for(int i=0;i<4;++i){
		glVertex3fv(&m_vs[i].x);
	}
	glEnd();
}

float CSceneNodeRoom::CControlPlane::Pick(const vector2 &posHS, const matrix44 &viewProj) const{
	if(!GetSingleton<CProgram>()->GetConfigVar<bool>(L"ShowRoom")) return -1.0f;
	bool strictLeft = false;
	bool strictRight = false;
	float zmax = -1.0f;
	for(int i=0; i<4; ++i){
		vector4 head4 = viewProj*vector4(m_vs[i],1.0f);
		vector4 tail4 = viewProj*vector4(m_vs[(i+1)%4],1.0f);
		vector2 head2(head4.x,head4.y);
		vector2 tail2(tail4.x,tail4.y);
		float c = CrossProduct(tail2-head2,posHS-head2);
		const static float epsilon = 0.001;
		strictLeft|=(c>epsilon);
		strictRight|=(c<-epsilon);
		//head4.z = -head4.z;
		if(head4.z>zmax){
			zmax = head4.z;
		}
	}
	if(GetSingleton<CProgram>()->IsInWireFrameMode()){
	//if(GetSingleton<CProgram>()->IsInWireFrameMode() && (strictLeft||strictRight) ){
		return -1.0f;
	}
	if(strictLeft && strictRight){
		return -1.0f;
	}else{
		return 1.0f;//zmax;
	}
}


std::pair<float,float> CSceneNodeRoom::CControlPlane::CastRange( const class vector3 &dir ) const{
	float d = DotProduct(dir,m_vs[0]);
	std::pair<float,float> ret(d,d);
	for(int i=1; i<4; ++i){
		d = DotProduct(dir,m_vs[i]);
		if(d<ret.first){
			ret.first = d;
		}else if(d>ret.second){
			ret.second = d;
		}
	}
	return ret;
}




void CSceneNodeRoom::OnInsertion(){

}


CSceneNodeRoom::~CSceneNodeRoom(){
	GetSingleton<CProgram>()->UnregisterOrderedObject(*this);
	for(int i=0;i<4;++i){
		for(int j=0;j<3;++j){
			GetSingleton<CProgram>()->UnregisterWallAnnotation(&m_axialAABBAnnotations[i*3+j], j);
		}
	}
	for(CCutter *p( m_cutters.PtrBegin()); p<m_cutters.PtrEnd(); ++p ){
		GetSingleton<CProgram>()->GetScene()->UnregisterCutter(p);
	}
	m_axialAABBAnnotations.DtorAll();
	m_cutters.DtorAll();
}




void CSceneNodeRoom::EnlargeAABB(AABB &aabb)const{
	Enlarge(aabb,AABB(vector3(0,0,0),vector3(m_sizeInMillimeters[0],m_sizeInMillimeters[1],m_sizeInMillimeters[2])));
}