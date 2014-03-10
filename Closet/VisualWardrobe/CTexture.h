#pragma once
#include <memory>
#include <string>
#include "3d.h"
class CTexture{
public:
	//������������,����GDI+����дͼƬ�ļ�
	CTexture(const std::wstring &name);//name��ͼƬ���ļ���,ͼƬ������TexturesĿ¼��,name�в�����TexturesĿ¼
	GLuint GetTexName();//������������� OpenGL �е� texture name
	void EnsureLoaded();//������û��װ���ڴ���װ��
private:
	std::wstring m_name;
	GLuint m_glTexname;
};

namespace NTexture{
	void Bind(CTexture &texture);//��texture�е�������ΪOpenGL�ĵ�ǰ����
};

