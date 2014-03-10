// DLL的入口
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
		//所有命令都是以字符串的形式通过这个函数调用
		//cmd是命令和参数,out是输出信息
		//启用GDI+
		Status s(Ok);
		GdiplusStartupInput input;
		ULONG_PTR token;
		s = GdiplusStartup(&token,&input,NULL);
		assert(Ok == s);
		if( !cmd.size() ){
			GetSingleton<CProgram>()->Redraw();
			return 0;
		}
		//以'\t'为界，区分命令名和参数
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
	__declspec(dllexport) int BindDC(HDC hDC){//在调用dll的程序不配置OpenGL工作环境的情况下,调用这个来初始化OpenGL的环境
		return GetSingleton<CProgram>()->BindDC(hDC);
	}
	__declspec(dllexport) void UseCurrentDC(){//在调用dll的程序已经配置好OpenGL工作环境的情况下请只调用这个
		GetSingleton<CProgram>()->UseCurrentDC();
	}
}
