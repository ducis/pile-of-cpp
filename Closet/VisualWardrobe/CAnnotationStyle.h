#pragma once

#include "3d.h"

class CAnnotationStyle{//��ע��ʽ
public:
	enum{
		DIGITS_BIT = 1,
		LEFT_END_BIT = 2,
		RIGHT_END_BIT = 4,
		LINE_BIT = 8,
		ENDS_BIT = LEFT_END_BIT|RIGHT_END_BIT
	};
	void DrawAnnotationEnd(const vector3 &posWS) const;//����ע����ֹ��
	void DrawAnnotation(const vector3 &begin, const vector3 &end, int num, unsigned int mask) const;
	//������֮�仭��ע,num��Ҫ��ʾ�ĳ���,mask�ǿ����Ƿ���ʾĳЩ����,��ö������
	void DrawAABBSelection(const AABB &aabb) const;//��aabb�Ŀ�
	//void SetCamUpWS(const vector3 &u){ m_camUpWS = u; }
	//void SetCamRightWS(const vector3 &r){ m_camRightWS = r; }
private:
	const matrix44 GetViewInv() const;
	const matrix44 GetView() const;
	//vector3 m_camUpWS;
	//vector3 m_camRightWS;
};
