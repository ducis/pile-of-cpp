#include "CTexCoordGen.h"
#include "3d.h"


void CTexCoordGen::Apply() const{
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, &m_uCoeff.x);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, &m_vCoeff.x);
}



CTexCoordGen::CTexCoordGen(const vector4 &uCoeff,const vector4 &vCoeff):m_uCoeff(uCoeff),m_vCoeff(vCoeff){
}
