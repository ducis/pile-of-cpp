#include <algorithm>
#include "3d.h"
#include "CAnnotation.h"
#include "IControlPlane.h"
#include "CProgram.h"
#include "singleton.h"
#include <sstream>



float CAnnotation::GetUPos() const{
	return m_u+m_uDelta;
}


void CAnnotation::ResetUPos(){
	m_uDelta = 0;
}


void CAnnotation::SetUPos(float u){
	m_uDelta = u-m_u;
}


void CAnnotation::Update(const vector3 &mainVec){
	m_dRange.first = m_planePair.first->CastRange(mainVec).first;
	m_dRange.second = m_planePair.second->CastRange(mainVec).first;
	if(m_dRange.first>m_dRange.second) swap(m_dRange.first,m_dRange.second);
	m_dRange.first+=0.0001;
	m_dRange.second-=0.0001;
	if(m_dRange.first>m_dRange.second) swap(m_dRange.first,m_dRange.second);
	m_length = m_dRange.second-m_dRange.first;
}


void CAnnotation::Render(const vector3 &mainVec,const vector3 &uBase,const vector3 &vBase,const float &uOffset){
	if(!GetSingleton<CProgram>()->GetConfigVar<bool>(L"ShowAnnotations")){
		return;
	}
	glBegin(GL_LINES);
	vector3 base = uBase*(GetUPos()+uOffset)+vBase*m_v;
	float dFirst = m_planePair.first->CastRange(mainVec).first;
	float dSecond = m_planePair.second->CastRange(mainVec).first;
	vector3 head = dFirst*mainVec + base;
	vector3 tail = dSecond*mainVec + base;
	glVertex3fv(&head.x);
	glVertex3fv(&tail.x);
	std::pair<float,float> uRangeFirst = m_planePair.first->CastRange(uBase);
	std::pair<float,float> uRangeSecond = m_planePair.second->CastRange(uBase);
	//if(m_u>uRangeFirst.second){
	//	glVertex3fv(&((dFirst*mainVec + uRangeFirst.first*uBase + m_v*vBase).x));
	//	glVertex3fv(&((head + 20.0f*uBase).x));
	//}else if(m_u<uRangeFirst.first){
	//	glVertex3fv(&((dFirst*mainVec + uRangeFirst.second*uBase + m_v*vBase).x));
	//	glVertex3fv(&((head - 20.0f*uBase).x));
	//}
	//if(m_u>uRangeSecond.second){
	//	glVertex3fv(&((dSecond*mainVec + uRangeSecond.first*uBase + m_v*vBase).x));
	//	glVertex3fv(&((tail + 20.0f*uBase).x));
	//}else if(m_u<uRangeSecond.first){
	//	glVertex3fv(&((dSecond*mainVec + uRangeSecond.second*uBase + m_v*vBase).x));
	//	glVertex3fv(&((tail - 20.0f*uBase).x));
	//}
	//vector3
	glVertex3fv(&((dFirst*mainVec + GetCPCenter(*m_planePair.first,uBase) * uBase + GetCPCenter(*m_planePair.first,vBase) * vBase).x));
	glVertex3fv(&head.x);
	glVertex3fv(&((dSecond*mainVec + GetCPCenter(*m_planePair.second,uBase) * uBase + GetCPCenter(*m_planePair.second,vBase) * vBase).x));
	glVertex3fv(&tail.x);
	glEnd();
	vector3 mid = 0.5f*(head+tail);
	glRasterPos3fv(&mid.x);
	//glRasterPos2f(0,0);
	char s[32];
	_itoa_s(int(abs(dFirst-dSecond)),s,30,10);
	for(char *pc = s; *pc; ++pc){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
	}
}

//void CAnnotation::DrawText(){
//}
