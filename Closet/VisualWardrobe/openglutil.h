#pragma once

#include "3d.h"
#include <string>

namespace OpenGLUtil{
	inline void DrawText(const std::wstring &str){
		//在OpenGL中显示中文
		HDC hDC( wglGetCurrentDC() );
		GLuint list( glGenLists(1) );
		for(std::wstring::const_iterator i(str.begin()); i!=str.end(); ++i){
			wglUseFontBitmapsW(hDC, *i, 1, list);
			glCallList(list);
		}
		glDeleteLists(list, 1);
	}

};