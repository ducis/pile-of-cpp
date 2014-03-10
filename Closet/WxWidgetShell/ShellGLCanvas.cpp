#include "ShellGLCanvas.h"
#include "wx/wx.h"
#include "../VisualWardrobe/Common.h"
#include "windows.h"
#include "globals.h"
#include <sstream>
#include <iostream>
#include <fstream>

BEGIN_EVENT_TABLE(ShellGLCanvas, wxGLCanvas)
    EVT_SIZE(ShellGLCanvas::OnSize)
    EVT_PAINT(ShellGLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND(ShellGLCanvas::OnEraseBackground)
    EVT_MOUSE_EVENTS(ShellGLCanvas::OnMouse)
END_EVENT_TABLE()

ShellGLCanvas::ShellGLCanvas(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : wxGLCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name),
	m_firstPaint(true)
{
	//SetCursor(wxCursor(wxCURSOR_BLANK));
}

ShellGLCanvas::~ShellGLCanvas()
{
}

void ShellGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // must always be here
    wxPaintDC dc(this);

    SetCurrent();

	if(m_firstPaint){
		m_firstPaint = false;
		(*g_pUseCurrentDCFunc)();
		using namespace std;
		wstring cmd;
		wofstream os(L"sshellautoexec.log");

		{
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
	}else{
		std::wostringstream os;
		(*g_pUseCurrentDCFunc)();
		(*g_pCommandEntryFunc)(L"",os);
	}

}

void ShellGLCanvas::OnSize(wxSizeEvent& event)
{
    wxGLCanvas::OnSize(event);
	std::wostringstream os;
	os<<L"OnResize "<<GetSize().GetWidth()<<L' '<<GetSize().GetHeight();
	(*g_pCommandEntryFunc)(os.str(),std::wcout);
}

void ShellGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
}


void ShellGLCanvas::OnMouse(wxMouseEvent& event)
{
	using namespace std;
	if(event.Dragging()||event.Moving()){
		wostringstream os;
//		wxSetCursor(wxCursor(wxCURSOR_BLANK));
		os  << L"SetCursorHS "
			<< float( float(event.GetX()) / GetSize().GetWidth() * 2.0f - 1.0f )
			<< L' '
			<< float( -float(event.GetY()) / GetSize().GetHeight() * 2.0f + 1.0f );

		(*g_pCommandEntryFunc)(os.str(),wcout);
	}
	if(event.LeftDown()){
		if(event.ShiftDown()){
			(*g_pCommandEntryFunc)(L"AlignPickedWithPointedAt 0",wcout);
		}else if(event.ControlDown()){
			(*g_pCommandEntryFunc)(L"AlignPickedWithPointedAt 0",wcout);
			(*g_pCommandEntryFunc)(L"ConnectPickedToPointedAt 0",wcout);
		}else if(event.AltDown()){
			(*g_pCommandEntryFunc)(L"PickWardrobeCell",wcout);
		}else{
			(*g_pCommandEntryFunc)(L"Pick 0",wcout);
			(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
		}
	}else if(event.MiddleDown()){		
		if(event.ShiftDown()){
			(*g_pCommandEntryFunc)(L"Pick 5",wcout);
		}else{
			(*g_pCommandEntryFunc)(L"Pick 4",wcout);
		}
		(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
	}else if(event.RightDown()){
		if(event.ShiftDown()){
			(*g_pCommandEntryFunc)(L"Pick 3",wcout);
		}else if(event.ControlDown()){
			(*g_pCommandEntryFunc)(L"DisconnectPickedFromPointedAt 0",wcout);
		}else if(event.AltDown()){
			(*g_pCommandEntryFunc)(L"UnpickWardrobeCell",wcout);
		}else{
			(*g_pCommandEntryFunc)(L"Pick 1",wcout);
			(*g_pCommandEntryFunc)(L"BeginDrag",wcout);
		}
	}else if(event.ButtonUp()){
		(*g_pCommandEntryFunc)(L"EndDrag",wcout);
	}else if(event.GetWheelRotation()){
		(*g_pCommandEntryFunc)(event.GetWheelRotation()>0 ? L"Zoom 1.1" : L"Zoom 0.9", wcout);
	}
}
