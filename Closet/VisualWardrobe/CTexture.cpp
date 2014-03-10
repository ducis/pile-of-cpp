#include "CTexture.h"

#include "windowsnominmax.h"
//#include <windows.h>
#include <algorithm>
using std::min;
using std::max;
#include <gdiplus.h>
#include "util.h"

template<> bool Failed(Gdiplus::Status ret) { return Gdiplus::Ok!=ret; }

CTexture::CTexture(const std::wstring &name):m_name(name),m_glTexname(0xFFFFFFFF){
}

GLuint CTexture::GetTexName(){
	EnsureLoaded();
	return m_glTexname;
}

void CTexture::EnsureLoaded(){
	if(0xFFFFFFFF!=m_glTexname){
		return;
	}
	//下面是用GDI+读图片
	using namespace Gdiplus;
	Bitmap bm((L"Textures\\"+m_name).c_str());
	Status s = bm.GetLastStatus();
	RETURN_IF_FAIL(bm.GetLastStatus());
	Rect rect(0,0,bm.GetWidth(),bm.GetHeight());
	BitmapData bmd;
	RETURN_IF_FAIL(bm.LockBits(&rect,ImageLockModeRead,PixelFormat24bppRGB,&bmd));

	//装入OpenGL
	glGenTextures(1,&m_glTexname);
	glPushAttrib(GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_glTexname);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	while(GL_NO_ERROR!=glGetError())
		;
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,bmd.Width,bmd.Height,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmd.Scan0);
	GLenum err = glGetError();
	assert(GL_NO_ERROR == err);
	glPopAttrib();
	bm.UnlockBits(&bmd);
	
}


namespace NTexture{
	void Bind(CTexture &texture){
		glBindTexture(GL_TEXTURE_2D,texture.GetTexName());
		GLenum err = glGetError();
		assert(GL_NO_ERROR == err);
	}
};
