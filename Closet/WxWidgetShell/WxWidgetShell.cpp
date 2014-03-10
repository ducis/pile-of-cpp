
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "ShellFrame.h"
#include "globals.h"
#include <fstream>
#include <sstream>
#include <locale>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
class MyApp: public wxApp
{
    virtual bool OnInit();
};
IMPLEMENT_APP(MyApp)
bool MyApp::OnInit()
{
	using namespace std;
	{
		locale loc( "chs" , locale::ctype );
		locale::global(loc);
	}
	g_hDLL = LoadLibrary(L"VisualWardrobe.dll");
	assert(g_hDLL);

	g_pCommandEntryFunc = (PCOMMANDENTRYFUNC)GetProcAddress(g_hDLL,"CommandEntry");
	assert(g_pCommandEntryFunc);

	g_pUseCurrentDCFunc = (PUSECURRENTDCFUNC)GetProcAddress(g_hDLL,"UseCurrentDC");
	assert(g_pUseCurrentDCFunc);

    ShellFrame *frame = new ShellFrame( L"ÒÂ¹ñÉè¼Æ", wxPoint(50,50), wxSize(800,600) );
    frame->Show( true );
    SetTopWindow( frame );
    return true;
}
