#pragma once
#include "mtxlib.h"

class CTexCoordGen{
public:
	//�Զ����������������(����glTexGen*�Ĳ��������е���)
	void Apply() const;//������Ӧ��OpenGL����,�趨OpenGL�������������ɵ�״̬
	CTexCoordGen(const vector4 &uCoeff,const vector4 &vCoeff);
	//uCoeff,vCoeff�������ֵ,����ݳ�����Ŀǰʹ�õ�ֵ����ʾ��Ч�����е���
private:
	vector4 m_uCoeff;
	vector4 m_vCoeff;
};
