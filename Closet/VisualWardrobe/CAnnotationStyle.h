#pragma once

#include "3d.h"

class CAnnotationStyle{//标注样式
public:
	enum{
		DIGITS_BIT = 1,
		LEFT_END_BIT = 2,
		RIGHT_END_BIT = 4,
		LINE_BIT = 8,
		ENDS_BIT = LEFT_END_BIT|RIGHT_END_BIT
	};
	void DrawAnnotationEnd(const vector3 &posWS) const;//画标注的起止线
	void DrawAnnotation(const vector3 &begin, const vector3 &end, int num, unsigned int mask) const;
	//在两点之间画标注,num是要显示的长度,mask是控制是否显示某些东西,见枚举类型
	void DrawAABBSelection(const AABB &aabb) const;//画aabb的框
	//void SetCamUpWS(const vector3 &u){ m_camUpWS = u; }
	//void SetCamRightWS(const vector3 &r){ m_camRightWS = r; }
private:
	const matrix44 GetViewInv() const;
	const matrix44 GetView() const;
	//vector3 m_camUpWS;
	//vector3 m_camRightWS;
};
