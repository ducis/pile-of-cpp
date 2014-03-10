#pragma once

#include "CSceneNodeBoxyDecorator.h"
#include "3d.h"

class CSceneNodeBoxyAutoTex: public CSceneNodeBoxyDecorator{
public://存储贴图和自动贴图坐标的类
	virtual void Render() const;
	CSceneNodeBoxyAutoTex(class CTexture *pTex,const vector4 &uCoeff,const vector4 &vCoeff,ISceneNodeBoxy *pDecorated);
private:
	class CTexture *m_pTex;
	vector4 m_uCoeff;
	vector4 m_vCoeff;
};
