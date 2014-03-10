#include "CTargetCamera.h"
#include "3d.h"
#include "singleton.h"
#include "CProgram.h"
#include "MyUtilities.h"

CTargetCamera::CTargetCamera():m_target(0,0,0),m_azimuth(PI/4),m_elevation(PI/4),m_distance(30000)
{
	m_pPanner.reset(new panner_type(this));
	m_pOrbiter.reset(new orbiter_type(this));
}

CTargetCamera::~CTargetCamera(){
	Deactivate();
}

void CTargetCamera::Activate(){
	CProgram *pProgram(GetSingleton<CProgram>());
	pProgram->RegisterPickable(4,m_pPanner.get());
	pProgram->RegisterPickable(5,m_pOrbiter.get());
	Apply();
}

void CTargetCamera::Deactivate(){
	CProgram *pProgram(GetSingleton<CProgram>());
	pProgram->UnregisterPickable(4,m_pPanner.get());
	pProgram->UnregisterPickable(5,m_pOrbiter.get());
}

void CTargetCamera::Apply() const
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLenum err(glGetError());
	assert(GL_NO_ERROR==err);
	glMatrixMode(GL_MODELVIEW);
	assert(GL_NO_ERROR==glGetError());
	glLoadIdentity();
	assert(GL_NO_ERROR==glGetError());
	const vector2 h( cosf(m_azimuth),sinf(m_azimuth) );
	const float hl( cosf(m_elevation) );
	const float v( sinf(m_elevation) );
	vector3 eye( m_target + vector3(h*hl,v).normalized()* m_distance );
	vector3 up( vector3(-v*h,hl).normalized() );
	float epsilon = 0.01f;
	using namespace MyUtilities;
	for(int i=0; i<3; ++i){
		SnapTo(up[i], 0.0f, epsilon);
		SnapTo(up[i], 1.0f, epsilon);
		SnapTo(eye[i],m_target[i],epsilon);
	}
	gluLookAt( eye.x, eye.y, eye.z, m_target.x, m_target.y, m_target.z, up.x, up.y, up.z );
	assert(GL_NO_ERROR==glGetError());
	glPopAttrib();
	assert(GL_NO_ERROR==glGetError());
	//pProgram->SetInOrthoView((eyex==centerx?1:0)+(eyey==centery?1:0)+(eyez==centerz?1:0) == 2);
	CProgram *pProgram(GetSingleton<CProgram>());
	if((eye.x==m_target.x?1:0)+(eye.y==m_target.y?1:0)+(eye.z==m_target.z?1:0) == 2){
		if(eye.x != m_target.x){
			pProgram->SetOrthoViewAxis(0);
		}else if(eye.y != m_target.y){
			pProgram->SetOrthoViewAxis(1);
		}else if(eye.z != m_target.z){
			pProgram->SetOrthoViewAxis(2);
		}else{
			pProgram->SetOrthoViewAxis(-1);
		}
	}else{
		pProgram->SetOrthoViewAxis(-1);
	}
}

void CTargetCamera::MakeOrthogonal(int iView){
	if(0 == iView){
		m_azimuth=0;
		m_elevation=0;
	}else if(1 == iView){
		m_azimuth=-0.5*PI;
		m_elevation=0;
	}else if(2 == iView){
		m_azimuth=-PI/2;
		m_elevation=PI/2;
	}else if(3 == iView){
		m_azimuth = -PI;
		m_elevation = 0;
	}else if(4 == iView){
		m_azimuth = 0.5*PI;
		m_elevation = 0;
	}
	Apply();
}

const vector2 CTargetCamera::Pan(const vector2 &displacementHS, const matrix44 &viewProj){
	vector3 delta( (viewProj.inverted()*vector4(displacementHS,0,0)).xyz() );
	m_target-=delta;
	Apply();
	return displacementHS;
}


const vector2 CTargetCamera::Orbit(const vector2 &displacementHS, const matrix44 &viewProj){
	m_azimuth -= displacementHS.x;
	m_elevation -= displacementHS.y;
	Apply();
	return displacementHS;
}



void CTargetCamera::SetTarget(const vector3 &p){
	m_target = p;
	Apply();
}