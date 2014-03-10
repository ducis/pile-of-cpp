// DLL�����
#include "windowsnominmax.h"
#include <gl/gl.h>
#include "Common.h"
#include <string>
#include "CProgram.h"
#include "singleton.h"
#include <cassert>
#include <sstream>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;
extern "C" {
	__declspec(dllexport) int CommandEntry(const wstring &cmd, wostream &out){
		//������������ַ�������ʽͨ�������������
		//cmd������Ͳ���,out�������Ϣ
		//����GDI+
		Status s(Ok);
		GdiplusStartupInput input;
		ULONG_PTR token;
		s = GdiplusStartup(&token,&input,NULL);
		assert(Ok == s);
		if( !cmd.size() ){
			GetSingleton<CProgram>()->Redraw();
			return 0;
		}
		//��'\t'Ϊ�磬�����������Ͳ���
		int off = cmd.find_first_of( L" \t" );
		if(off < 0){
			off = cmd.size();
		}
		wstring::const_iterator d = cmd.begin() + off;
		int ret = GetSingleton<CProgram>()->AcceptCommand( wstring(cmd.begin(),d), wstring(d,cmd.end()), out );
		GdiplusShutdown(token);
		return ret;
	}
	//
	__declspec(dllexport) int BindDC(HDC hDC){//�ڵ���dll�ĳ�������OpenGL���������������,�����������ʼ��OpenGL�Ļ���
		return GetSingleton<CProgram>()->BindDC(hDC);
	}
	__declspec(dllexport) void UseCurrentDC(){//�ڵ���dll�ĳ����Ѿ����ú�OpenGL�����������������ֻ�������
		GetSingleton<CProgram>()->UseCurrentDC();
	}
}
