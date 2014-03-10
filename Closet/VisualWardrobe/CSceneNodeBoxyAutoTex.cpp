#pragma once

#include "CSceneNodeBoxyAutoTex.h"
#include "CTexture.h"

void CSceneNodeBoxyAutoTex::Render() const{
	glPushAttrib(GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	NTexture::Bind(*m_pTex);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, &m_uCoeff.x);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, &m_vCoeff.x);
	CSceneNodeBoxyDecorator::Render();
	glPopAttrib();
}

CSceneNodeBoxyAutoTex::CSceneNodeBoxyAutoTex(CTexture *pTex, const vector4 &uCoeff, const vector4 &vCoeff, ISceneNodeBoxy *pDecorated)
:CSceneNodeBoxyDecorator(pDecorated),m_pTex(pTex),m_uCoeff(uCoeff),m_vCoeff(vCoeff)
{
}
