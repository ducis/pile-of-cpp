#pragma once

#include "CSceneNodeBoxyDecorator.h"
#include "3d.h"

class CSceneNodeBoxyAutoTex: public CSceneNodeBoxyDecorator{
public://�洢��ͼ���Զ���ͼ�������
	virtual void Render() const;
	CSceneNodeBoxyAutoTex(class CTexture *pTex,const vector4 &uCoeff,const vector4 &vCoeff,ISceneNodeBoxy *pDecorated);
private:
	class CTexture *m_pTex;
	vector4 m_uCoeff;
	vector4 m_vCoeff;
};
