#include "ShellFrame.h"
#include "ShellGLCanvas.h"
#include "globals.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>

enum
{
    ID_Quit = 1,
    ID_About,
	OneOverLastShellFrameEvent
};
BEGIN_EVENT_TABLE(ShellFrame, wxFrame)
    EVT_MENU(ID_Quit,  ShellFrame::OnQuit)
    EVT_MENU(ID_About, ShellFrame::OnAbout)
	//EVT_MENU(ID_RotateView, NULL)//ShellFrame::RotateView)
	//EVT_
END_EVENT_TABLE()

namespace ShellFrame_impl{
	struct COMMANDITEM{
		char m;
		const wchar_t *menuStr;
		const wchar_t *cmdStrs[32];
	};
	const COMMANDITEM c_cmdList[] = 
	{
		{'v',L"显示/隐藏房间",{L"Toggle ShowRoom",NULL}},
		//{'v',L"显示/隐藏标注",{L"Toggle ShowAnnotations",NULL}},
		{'v',L"显示/隐藏板材名称",{L"Toggle ShowBoardNames",NULL}},
		{'v',L"线框模式",{L"WireFrameMode",NULL}},
		{'v',L"着色模式",{L"ShadedMode",NULL}},
		{'v',L"设置门板显示方式1",{L"Set DoorStyle 0",NULL}},
		{'v',L"设置门板显示方式2",{L"Set DoorStyle 1",NULL}},
		{'v',L"设置门板显示方式3",{L"Set DoorStyle 2",NULL}},
		{'v',L"左视图",{L"SetCameraOrtho 0",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"右视图",{L"SetCameraOrtho 3",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"前视图",{L"SetCameraOrtho 1",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"后视图",{L"SetCameraOrtho 4",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"顶视图",{L"SetCameraOrtho 2",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"自动缩放",{L"CenterCameraView",L"FitView",NULL}},
		{'v',L"设置地板材质",{L"SetRoomMaterial Floor %m",NULL}},
		{'v',L"设置墙体材质",{L"SetRoomMaterial Walls %m",NULL}},
		{'v',L"设置天花板材质",{L"SetRoomMaterial Ceiling %m",NULL}},

		//{'e',L"",{L"DivideCell 0",NULL}},
		{'e',L"竖直分割",{L"DivideCell 1",NULL}},
		{'e',L"水平分割",{L"DivideCell 2",NULL}},
		{'e',L"删除所选板",{L"RemovePicked",NULL}},
		{'e',L"旋转所选柜体",{L"RotatePicked",NULL}},
		{'e',L"锁定/解锁选定板",{L"ToggleBoardRemovable",NULL}},
		{'e',L"设置板材材质",{L"SetPickedMaterial %m",NULL}},

		{'i',L"无型号配件",{L"CreateComponent %c",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"配件",{L"CreateComponentVariant %v",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"抽屉",{L"Generate %gD %p",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"柜体",{L"CreateWardrobe %gW %p",L"DeployNewBornAsBoxyDraggableContainer 0 3000 3000",NULL}},

		{'o',L"导出选定板材DXF",{L"FormatPickedBoardToDXF %f",NULL}},
		{'o',L"导出板材列表",{L"FormatBoardList %f",NULL}},
		
		{' ',NULL,{NULL}}
	};
}
using namespace ShellFrame_impl;

ShellFrame::ShellFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size),
	   m_pGLCanvas( new ShellGLCanvas(this, wxID_ANY, wxDefaultPosition,wxSize(800, 600), wxSUNKEN_BORDER) )
{
    wxMenu *menuFile = new wxMenu;

    //menuFile->Append( ID_About, L"&About..." );
    //menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, L"退出(&X)" );

	wxMenu *menuEdit( new wxMenu );

	wxMenu *menuView( new wxMenu );

	wxMenu *menuOutput( new wxMenu );

	wxMenu *menuInsert( new wxMenu );

	for(const COMMANDITEM *pC( c_cmdList ); pC->menuStr; ++pC){
		const wchar_t *pMS(pC->menuStr);
		wxMenu *pMenu(menuFile);
		if(pC->m == 'v'){
			pMenu = menuView;
		}else if(pC->m == 'e'){
			pMenu = menuEdit;
		}else if(pC->m == 'o'){
			pMenu = menuOutput;
		}else if(pC->m == 'i'){
			pMenu = menuInsert;
		}
		int id( pC-c_cmdList+OneOverLastShellFrameEvent );
		Connect(id,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(ShellFrame::OnDynamicBoundEvents));
		pMenu->Append(id,pMS);
	}

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, L"文件(&F)" );
	menuBar->Append( menuEdit, L"编辑(&E)" );
	menuBar->Append( menuView, L"视图(&V)" );
	menuBar->Append( menuOutput, L"输出(&O)" );
	menuBar->Append( menuInsert, L"插入(&I)" );



    SetMenuBar( menuBar );

    CreateStatusBar();
}
void ShellFrame::OnQuit(wxCommandEvent& e)//WXUNUSED(event))
{
    Close( true );
}
void ShellFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void ShellFrame::OnDynamicBoundEvents(wxCommandEvent &e){
	int id(e.GetId()-OneOverLastShellFrameEvent);
	for(const wchar_t * const *t=c_cmdList[id].cmdStrs; *t; ++t){
		std::wstring s(*t);
		std::wstring::size_type c(0);
		while(1){
			std::wstring::size_type o(std::wstring::npos);
			if( (o=s.find(L"%f",c))!=std::wstring::npos ){
				wxString filename( wxFileSelector(L"选择路径") );
				if(filename.empty())	return;
				s.replace(o,wcslen(L"%f"),filename.c_str());
				c = o+filename.size();
			}else if( (o=s.find(L"%m",c))!=std::wstring::npos ){
				wxArrayString ms;
				std::wstringstream ss;
				(*g_pCommandEntryFunc)(L"QueryMaterials",ss);
				for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i){
					ms.Add(*i);
				}
				wxString ret( wxGetSingleChoice(L"选择材质",L"材质",ms) );
				if(ret.empty())  return;
				s.replace(o,wcslen(L"%m"),ret.c_str());
				c = o+ret.size();
			}else if( (o=s.find(L"%gD",c))!=std::wstring::npos ){
				wxArrayString ms;
				std::wstringstream ss;
				(*g_pCommandEntryFunc)(L"QueryGenerators D",ss);
				for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i){
					ms.Add(*i);
				}
				wxString ret( wxGetSingleChoice(L"选择抽屉构造",L"抽屉构造",ms) );
				if(ret.empty())  return;
				s.replace(o,wcslen(L"%gD"),std::wstring(L"D.")+ret.c_str());
				c = o+ret.size();
			}else if( (o=s.find(L"%gW",c))!=std::wstring::npos ){
				wxArrayString ms;
				std::wstringstream ss;
				(*g_pCommandEntryFunc)(L"QueryGenerators W",ss);
				for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i){
					ms.Add(*i);
				}
				wxString ret( wxGetSingleChoice(L"选择柜体构造",L"柜体构造",ms) );
				if(ret.empty())  return;
				s.replace(o,wcslen(L"%gW"),std::wstring(L"W.")+ret.c_str());
				c = o+ret.size();
			}else if( (o=s.find(L"%p",c))!=std::wstring::npos ){
				wxArrayString ms;
				std::wstringstream ss;
				(*g_pCommandEntryFunc)(L"QueryManufactureProperties",ss);
				for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i){
					ms.Add(*i);
				}
				wxString ret( wxGetSingleChoice(L"选择厂商配置",L"厂商配置",ms) );
				if(ret.empty())  return;
				s.replace(o,wcslen(L"%p"),ret.c_str());
				c = o+ret.size();
			}else if( (o=s.find(L"%c",c))!=std::wstring::npos ){
				wxArrayString ms;
				std::wstringstream ss;
				(*g_pCommandEntryFunc)(L"QueryComponents",ss);
				for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i){
					ms.Add(*i);
				}
				wxString ret( wxGetSingleChoice(L"选择配件",L"配件",ms) );
				if(ret.empty())  return;
				s.replace(o,wcslen(L"%c"),ret.c_str());
				c = o+ret.size();
			}else if( (o=s.find(L"%v",c)) != std::wstring::npos ){
				wxArrayString ms;

				{
					std::wstringstream ss;
					(*g_pCommandEntryFunc)(L"QueryComponents",ss);
					for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i ){
						ms.Add(*i);
					}
				}
				wxString retcmp( wxGetSingleChoice(L"选择配件",L"配件",ms) );
				if(retcmp.empty())  return;


				{	
					std::wstringstream ss;
					(*g_pCommandEntryFunc)(std::wstring(L"QueryVariants ") + retcmp.c_str(), ss);
					ms.clear();
					for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i ){
						ms.Add(*i);
					}
				}
				wxString retvar( wxGetSingleChoice(L"选择型号",L"型号",ms) );
				if(retvar.empty())	return;

				ms.clear();
				for(int i=0;i<8;++i){
					wchar_t s[16];
					ms.Add(_itow(i,s,10));
				}
				wxString retpos( wxGetSingleChoice(L"选择位置",L"位置",ms) );
				if(retpos.empty())	return;

				wxString ret(retcmp+L" "+retvar+L" "+retpos);
				s.replace(o,wcslen(L"%v"),ret.c_str());
				c = o+ret.size();
			}else{
				break;
			}
		}
		(*g_pCommandEntryFunc)(s,std::wcout);
	}
}

//
//bool ShellFrame::ProcessCommand(int id)
//{
//	return false;
//}
//
//
//
//
//bool ShellFrame::HandleCommand(WXWORD id, WXWORD cmd, WXHWND control){
//	return false;
//}
