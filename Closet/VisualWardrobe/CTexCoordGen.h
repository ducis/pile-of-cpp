#pragma once
#include "mtxlib.h"

class CTexCoordGen{
public:
	//自动生成纹理坐标的类(储存glTexGen*的参数并进行调用)
	void Apply() const;//调用相应的OpenGL函数,设定OpenGL中纹理坐标生成的状态
	CTexCoordGen(const vector4 &uCoeff,const vector4 &vCoeff);
	//uCoeff,vCoeff都是相对值,请根据程序中目前使用的值和显示的效果进行调整
private:
	vector4 m_uCoeff;
	vector4 m_vCoeff;
};
