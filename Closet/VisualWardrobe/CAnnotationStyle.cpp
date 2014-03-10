
#include "CAnnotationStyle.h"
#include "3d.h"
#include "singleton.impl.h"
#include "CProgram.h"

void CAnnotationStyle::DrawAnnotationEnd(const vector3 &posWS) const{
	matrix44 viewInv(GetViewInv());
	const vector3 offset((viewInv*vector4(-20,20,0,0)).xyz());
	vector3 a(posWS+offset),b(posWS-offset);
	glBegin(GL_LINES);
	glVertex3f(a.x,a.y,a.z);
	glVertex3f(b.x,b.y,b.z);
	glEnd();
}

template CAnnotationStyle *GetSingleton();

void CAnnotationStyle::DrawAnnotation(const vector3 &begin, const vector3 &end, int num, unsigned int mask) const{
	if(mask & LEFT_END_BIT){
		DrawAnnotationEnd(begin);
	}
	if(mask & RIGHT_END_BIT){
		DrawAnnotationEnd(end);
	}
	if(mask & DIGITS_BIT){

		matrix44 m(GetSingleton<CProgram>()->GetViewProjTransform());
		matrix44 mi(m);
		mi.invert();

		char s[32];
		_itoa_s(num,s,30,10);
		int nDigits(strlen(s));

		vector3 mid(0.5*(begin+end));
		vector3 dirCS( m*(end-begin) );
		vector2 deltaCS( abs(dirCS.x)>abs(dirCS.y) ? vector2(-0.007*nDigits,-0.03) : vector2(-0.01-0.014*nDigits,-0.01) );//-80,20) );

		mid += (mi*vector4(deltaCS)).xyz();
		//mid+=up*50.0f;
		glRasterPos3f(mid.x,mid.y,mid.z);
		for(char *pc = s; *pc; ++pc){
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*pc);
		}
	}
	if(mask & LINE_BIT){
		glBegin(GL_LINES);
		glVertex3f(begin.x,begin.y,begin.z);
		glVertex3f(end.x,end.y,end.z);
		glEnd();
	}
}


const matrix44 CAnnotationStyle::GetViewInv() const{
	matrix44 view;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	return view.invert();
}


const matrix44 CAnnotationStyle::GetView() const{
	matrix44 view;
	glGetFloatv(GL_MODELVIEW_MATRIX,(GLfloat*)&view);
	return view;
}


void CAnnotationStyle::DrawAABBSelection(const AABB &aabb) const{
	glPushAttrib(GL_POINT_BIT);
	glPointSize(8.0f);
	vector3 p[2]= {
		aabb.first,
		aabb.second
	};
	glPushMatrix();
	vector3 center(GetCenter(aabb)),dim(GetDim(aabb));
	glTranslatef(center.x,center.y,center.z);
	glScalef(dim.x,dim.y,dim.z);
	glutWireCube(1.0);
	glPopMatrix();
	glBegin(GL_POINTS);
	for(int i(0);i<2;++i){
		for(int j(0);j<2;++j){
			for(int k(0);k<2;++k){
				glVertex3f(p[i].x,p[j].y,p[k].z);
			}
		}
	}
	glEnd();
	glPopAttrib();
}
