#pragma once


#include "wx/wx.h"
#include <memory>

class ShellFrame: public wxFrame
{
public:
    ShellFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnDynamicBoundEvents(wxCommandEvent &e);

//	virtual bool ProcessCommand(int id);
	
//    virtual bool HandleCommand(WXWORD id, WXWORD cmd, WXHWND control);
private:

	std::auto_ptr<class ShellGLCanvas> m_pGLCanvas;

    DECLARE_EVENT_TABLE()
};
