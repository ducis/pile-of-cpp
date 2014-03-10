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
		{'v',L"��ʾ/���ط���",{L"Toggle ShowRoom",NULL}},
		//{'v',L"��ʾ/���ر�ע",{L"Toggle ShowAnnotations",NULL}},
		{'v',L"��ʾ/���ذ������",{L"Toggle ShowBoardNames",NULL}},
		{'v',L"�߿�ģʽ",{L"WireFrameMode",NULL}},
		{'v',L"��ɫģʽ",{L"ShadedMode",NULL}},
		{'v',L"�����Ű���ʾ��ʽ1",{L"Set DoorStyle 0",NULL}},
		{'v',L"�����Ű���ʾ��ʽ2",{L"Set DoorStyle 1",NULL}},
		{'v',L"�����Ű���ʾ��ʽ3",{L"Set DoorStyle 2",NULL}},
		{'v',L"����ͼ",{L"SetCameraOrtho 0",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"����ͼ",{L"SetCameraOrtho 3",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"ǰ��ͼ",{L"SetCameraOrtho 1",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"����ͼ",{L"SetCameraOrtho 4",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"����ͼ",{L"SetCameraOrtho 2",L"CenterCameraView",L"FitView",NULL}},
		{'v',L"�Զ�����",{L"CenterCameraView",L"FitView",NULL}},
		{'v',L"���õذ����",{L"SetRoomMaterial Floor %m",NULL}},
		{'v',L"����ǽ�����",{L"SetRoomMaterial Walls %m",NULL}},
		{'v',L"�����컨�����",{L"SetRoomMaterial Ceiling %m",NULL}},

		//{'e',L"",{L"DivideCell 0",NULL}},
		{'e',L"��ֱ�ָ�",{L"DivideCell 1",NULL}},
		{'e',L"ˮƽ�ָ�",{L"DivideCell 2",NULL}},
		{'e',L"ɾ����ѡ��",{L"RemovePicked",NULL}},
		{'e',L"��ת��ѡ����",{L"RotatePicked",NULL}},
		{'e',L"����/����ѡ����",{L"ToggleBoardRemovable",NULL}},
		{'e',L"���ð�Ĳ���",{L"SetPickedMaterial %m",NULL}},

		{'i',L"���ͺ����",{L"CreateComponent %c",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"���",{L"CreateComponentVariant %v",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"����",{L"Generate %gD %p",L"DeployNewBornIntoPickedCell",NULL}},
		{'i',L"����",{L"CreateWardrobe %gW %p",L"DeployNewBornAsBoxyDraggableContainer 0 3000 3000",NULL}},

		{'o',L"����ѡ�����DXF",{L"FormatPickedBoardToDXF %f",NULL}},
		{'o',L"��������б�",{L"FormatBoardList %f",NULL}},
		
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
    menuFile->Append( ID_Quit, L"�˳�(&X)" );

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
    menuBar->Append( menuFile, L"�ļ�(&F)" );
	menuBar->Append( menuEdit, L"�༭(&E)" );
	menuBar->Append( menuView, L"��ͼ(&V)" );
	menuBar->Append( menuOutput, L"���(&O)" );
	menuBar->Append( menuInsert, L"����(&I)" );



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
				wxString filename( wxFileSelector(L"ѡ��·��") );
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
				wxString ret( wxGetSingleChoice(L"ѡ�����",L"����",ms) );
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
				wxString ret( wxGetSingleChoice(L"ѡ����빹��",L"���빹��",ms) );
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
				wxString ret( wxGetSingleChoice(L"ѡ����幹��",L"���幹��",ms) );
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
				wxString ret( wxGetSingleChoice(L"ѡ��������",L"��������",ms) );
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
				wxString ret( wxGetSingleChoice(L"ѡ�����",L"���",ms) );
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
				wxString retcmp( wxGetSingleChoice(L"ѡ�����",L"���",ms) );
				if(retcmp.empty())  return;


				{	
					std::wstringstream ss;
					(*g_pCommandEntryFunc)(std::wstring(L"QueryVariants ") + retcmp.c_str(), ss);
					ms.clear();
					for( std::istream_iterator<std::wstring,wchar_t> i(ss); i!=std::istream_iterator<std::wstring,wchar_t>();++i ){
						ms.Add(*i);
					}
				}
				wxString retvar( wxGetSingleChoice(L"ѡ���ͺ�",L"�ͺ�",ms) );
				if(retvar.empty())	return;

				ms.clear();
				for(int i=0;i<8;++i){
					wchar_t s[16];
					ms.Add(_itow(i,s,10));
				}
				wxString retpos( wxGetSingleChoice(L"ѡ��λ��",L"λ��",ms) );
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
