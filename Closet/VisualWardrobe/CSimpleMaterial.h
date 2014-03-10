#pragma once

#include "IMaterial.h"
#include "CIdentifier.h"
#include <boost/property_tree/ptree_fwd.hpp> 

class CSimpleMaterial: public IMaterial{
public:
	//��򵥵Ĳ���,����һ����ͼ,��ӦMaterials�ļ�����xml�ļ��е�SimpleMaterial���
	virtual void Apply() const;//�趨OpenGL״̬׼����Ⱦ
	virtual const CIdentifier &GetIdentifier() const;//���ر�ʶ��
	CSimpleMaterial(boost::property_tree::wptree &tr);//ͨ��xml�ļ��е�<SimpleMaterial></SimpleMaterial>�ڵ���д���
private:
	class CTexture *m_pTex;
	CIdentifier m_identifier;
};
