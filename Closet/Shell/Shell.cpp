// Shell.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include <windowsx.h>
#include "../VisualWardrobe/Common.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <locale>
#include <sstream>
using namespace std;
HDC InitializeCanvas();
PCOMMANDENTRYFUNC g_pCommandEntryFunc;
bool g_stopAutoMode = false;
std::wstring g_keyBinding[256];
void InitKeyBinding();
float g_windowClientWidth,g_windowClientHeight;
//bool g_gui(true);
bool g_console(true);
int _tmain(int argc, _TCHAR* argv[])
{
	{
		locale loc( "chs" , locale::ctype );
		//loc.combine<num_put<wchar_t> >( locale("C") );
		locale::global(loc);
		//locale::global(locale( "C" , locale::numeric ));
//		setlocale(LC_ALL,"chs");
		//locale::global.combine<codecvt
	}
	InitKeyBinding();
	HMODULE hDll = LoadLibrary(L"VisualWardrobe.dll");
	DWORD err(GetLastError());
	assert(hDll);

	PBINDDCFUNC pBindDCFunc = (PBINDDCFUNC)GetProcAddress(hDll,"BindDC");
	assert(pBindDCFunc);
	HDC hDC = InitializeCanvas();
	(*pBindDCFunc)(hDC);
	SelectFont(hDC,CreateFont(12,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FIXED_PITCH |FF_DONTCARE,L"宋体"));

	g_pCommandEntryFunc = (PCOMMANDENTRYFUNC)GetProcAddress(hDll,"CommandEntry");
	assert(g_pCommandEntryFunc);
	std::wstring cmd;

	//wofstream os(L"shellautoexec.log");
	//wostringstream os(L"");
	wostream &os(wcout);
	{
		//先把shellautoexec.txt里面的命令都调用一遍
		os << L"autoexec...\n";
		wifstream f(L"shellautoexec.txt");
		while(1){
			cmd.resize(4096);
			f.getline(&cmd[0],4096);
			int n = f.gcount();
			if(n<1) break;
			cmd.resize(f.gcount()-1);
			os << cmd << L"\n";
			int r = (*g_pCommandEntryFunc)(cmd,os);
			if(r){
				os << L"Error code: " << r << L"\n";
			}
		}
		os << L"...autoexec\n";
	}
	if(g_console){
		while(1){
			//读取一行命令,如果是\auto就切到OpenGL窗口操作
			//如果是\exit就退出,其它就传给DLL处理
			cmd.resize(4096);
			wcin.getline(&cmd[0],4096);
			int n = wcin.gcount();
			//wcin.ignore(n);
			cmd.resize(n-1);
			if(cmd == L"\\exit"){
				break;
			}
			if(cmd == L"\\auto"){
				MSG msg;
				while (GetMessage(&msg, NULL, 0, 0) && !g_stopAutoMode)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				g_stopAutoMode = false;
			}else{
				//use_facet<ctype<wchar_t>>(locale()).tolower(&cmd[0],&cmd[0]+cmd.size());
				//std::os << cmd << L"\n";
				int r = (*g_pCommandEntryFunc)(cmd,os);
				if(r){
					os << L"Error code: " << r << L"\n";
				}
			}
		}
	}else{
		//这一段没有写好,避免进入
		FreeConsole();
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0) && !g_stopAutoMode)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

#include <sstream>
LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg){
		case WM_PAINT:
			(*g_pCommandEntryFunc)(L"",wcout);
			return 0;
		case WM_KEYDOWN:
			switch( wParam ){
				case VK_ESCAPE:
					g_stopAutoMode = true;
					return 0;
				case 'W':
					(*g_pCommandEntryFunc)(L"Generate D.DefaultDrawer 默认",wcout);
					(*g_pCommandEntryFunc)(L"DeployNewBornIntoPickedCell",wcout);
					return 0;
				case 'T':
					(*g_pCommandEntryFunc)(L"CreateComponentVariant 领带盒 LDH300 7",wcout);
					(*g_pCommandEntryFunc)(L"DeployNewBornIntoPickedCell",wcout);
					return 0;
				case 'I':
					(*g_pCommandEntryFunc)(L"SetCameraOrtho 0",wcout);
					(*g_pCommandEntryFunc)(L"CenterCameraView",wcout);
					(*g_pCommandEntryFunc)(L"FitView",wcout);
					return 0;
				case 'O':
					(*g_pCommandEntryFunc)(L"SetCameraOrtho 1",wcout);
					(*g_pCommandEntryFunc)(L"CenterCameraView",wcout);
					(*g_pCommandEntryFunc)(L"FitView",wcout);
					return 0;
				case 'P':
					(*g_pCommandEntryFunc)(L"SetCameraOrtho 2",wcout);
					(*g_pCommandEntryFunc)(L"CenterCameraView",wcout);
					(*g_pCommandEntryFunc)(L"FitView",wcout);
					return 0;
				case 'U':
					(*g_pCommandEntryFunc)(L"CenterCameraView",wcout);
					(*g_pCommandEntryFunc)(L"FitView",wcout);
					return 0;
				default:
					{
						int r = (*g_pCommandEntryFunc)(g_keyBinding[wParam],wcout);
						if(r){
							wcout << L"Error code: " << r << L"\n";
						}
					}
					return 0;
			}
			return 0;
		case WM_MOUSEMOVE:
			{
				wostringstream os;
				os  << "SetCursorHS "
					<< float( float(GET_X_LPARAM(lParam)) / g_windowClientWidth * 2.0f - 1.0f )
					<< L' '
					<< float( -float(GET_Y_LPARAM(lParam)) / g_windowClientHeight * 2.0f + 1.0f );
 
				(*g_pCommandEntryFunc)(os.str(),wcout);
			}
			return 0;
		case WM_LBUTTONDOWN:
			if(wParam & MK_SHIFT){
				(*g_pCommandEntryFunc)(L"AlignPickedWithPointedAt 0",wcout);
			}else if(wParam & MK_CONTROL){
				(*g_pCommandEntryFunc)(L"AlignPickedWithPointedAt 0",wcout);
				(*g_pCommandEntryFunc)(L"ConnectPickedToPointedAt 0",wcout);
			}else{
				(*g_pCommandEntryFunc)(L"Pick 0",wcout);
				(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
			}
			return 0;
		case WM_MBUTTONDOWN:
			if(wParam & MK_SHIFT){
				(*g_pCommandEntryFunc)(L"Pick 5",wcout);
			}else{
				(*g_pCommandEntryFunc)(L"Pick 4",wcout);
			}
			(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
			return 0;
		case WM_RBUTTONDOWN:
			if(wParam & MK_SHIFT){
				(*g_pCommandEntryFunc)(L"Pick 3",wcout);
			}else if(wParam & MK_CONTROL){
				(*g_pCommandEntryFunc)(L"DisconnectPickedFromPointedAt 0",wcout);
			}else{
				(*g_pCommandEntryFunc)(L"Pick 1",wcout);
				(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
			}
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
			(*g_pCommandEntryFunc)(L"EndDrag",wcout);
			//(*g_pCommandEntryFunc)(L"PickWithCursor",wcout);
			return 0;
		case WM_MOUSEWHEEL:
			(*g_pCommandEntryFunc)(GET_WHEEL_DELTA_WPARAM(wParam)>0 ? L"Zoom 1.1" : L"Zoom 0.9", wcout);
			//(*g_pCommandEntryFunc)(HIWORD(wParam)>0 ? L"NextCP" : L"PrevCP", wcout);
			return 0;
	}
    return DefWindowProc( hWnd, msg, wParam, lParam );
}


HDC InitializeCanvas(){
	WNDCLASSEX wc;
	
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW ;
	wc.lpfnWndProc = &WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle( NULL );
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Canvas";
    RegisterClassEx( &wc );

	HWND hWnd = CreateWindow( L"Canvas" ,L"衣柜设计",0 , 0, 0, 1024, 768,
                              NULL, NULL, NULL, NULL );
	assert(hWnd);
	ShowWindow(hWnd,SW_SHOW);
	HDC hDC = GetDC(hWnd);
	assert(hDC);
	{
		WINDOWINFO wi;
		GetWindowInfo(hWnd,&wi);
		g_windowClientWidth = float(wi.rcClient.right - wi.rcClient.left);
		g_windowClientHeight = float(wi.rcClient.bottom - wi.rcClient.top);
	}
	return hDC;
}

void InitKeyBinding(){
	g_keyBinding[VK_F1] = L"Toggle ShowRoom";
	g_keyBinding[VK_F2] = L"Toggle ShowAnnotations";
	g_keyBinding[VK_F3] = L"Toggle FixWallThickness";
	g_keyBinding[VK_F4] = L"Toggle ShowInteriorDefBoundingBox";
	g_keyBinding[VK_F5] = L"Toggle ShowBoardNames";
	g_keyBinding[VK_F7] = L"WireFrameMode";
	g_keyBinding[VK_F8] = L"ShadedMode";
	g_keyBinding['1']   = L"Set DoorStyle 0";
	g_keyBinding['2']   = L"Set DoorStyle 1";
	g_keyBinding['3']   = L"Set DoorStyle 2";
	//g_keyBinding['Q'] = L"RememberCurCP";
	//g_keyBinding['A'] = L"AnnotateCP";
	//g_keyBinding['C'] = L"ClearTheOtherCP";
	//g_keyBinding['1'] = L"MoveCP 1";
	//g_keyBinding['2'] = L"MoveCP -1";
	//g_keyBinding['3'] = L"MoveCP 10";
	//g_keyBinding['4'] = L"MoveCP -10";
	//g_keyBinding['5'] = L"MoveCP 100";
	//g_keyBinding['6'] = L"MoveCP -100";
	//g_keyBinding[VK_OEM_4] = L"PrevInteriorDef";
	//g_keyBinding[VK_OEM_6] = L"NextInteriorDef";
	//g_keyBinding[VK_OEM_5] = L"CycleInteriorDefAxis";
	//g_keyBinding['J'] = L"SetInteriorDefAxis 0";
	//g_keyBinding['K'] = L"SetInteriorDefAxis 1";
	//g_keyBinding['L'] = L"SetInteriorDefAxis 2";
	//g_keyBinding[VK_OEM_COMMA] = L"IncInteriorDefBoardNum -1";
	//g_keyBinding[VK_OEM_PERIOD] = L"IncInteriorDefBoardNum 1";
	//g_keyBinding['7'] = L"IncInteriorDefBoardThickness -1";
	//g_keyBinding['8'] = L"IncInteriorDefBoardThickness 1";
	//g_keyBinding['9'] = L"IncInteriorDefBoardThickness -10";
	//g_keyBinding['0'] = L"IncInteriorDefBoardThickness 10";
	g_keyBinding['Z'] = L"PickWardrobeCell";
	g_keyBinding['X'] = L"UnpickWardrobeCell";
	g_keyBinding['V'] = L"DivideCell 0";
	g_keyBinding['B'] = L"DivideCell 1";
	g_keyBinding['N'] = L"DivideCell 2";
	g_keyBinding['D'] = L"RemovePicked";
	g_keyBinding['R'] = L"RotatePicked";
	g_keyBinding['M'] = L"LookAt 10000 10000 10000 1000 1000 1000 0 0 1";
	g_keyBinding['J'] = L"LookAt 10000 2000 1000 0 2000 1000 0 0 1";
	g_keyBinding['K'] = L"LookAt 2000 2000 10000  2000 2000 0  0 1 0";
	g_keyBinding['L'] = L"LookAt 2000 10000 1000 2000 0 1000 0 0 1";
	g_keyBinding['F'] = L"FormatPickedBoardToDXF zzzz.dxf";
	g_keyBinding['S'] = L"Set DragSpeedModifier 0.02";
	g_keyBinding['A'] = L"Set DragSpeedModifier 1.0";
	g_keyBinding['Y'] = L"ToggleBoardRemovable";
//	g_keyBinding[VK_LBUTTON] = L"PickWithCursor";
}
