#pragma once
#include <memory>
#include <string>
#include "3d.h"
class CTexture{
public:
	//基本的纹理类,用了GDI+来读写图片文件
	CTexture(const std::wstring &name);//name是图片的文件名,图片必须在Textures目录下,name中不包括Textures目录
	GLuint GetTexName();//返回这个纹理在 OpenGL 中的 texture name
	void EnsureLoaded();//若纹理没有装入内存则装入
private:
	std::wstring m_name;
	GLuint m_glTexname;
};

namespace NTexture{
	void Bind(CTexture &texture);//将texture中的纹理设为OpenGL的当前纹理
};

