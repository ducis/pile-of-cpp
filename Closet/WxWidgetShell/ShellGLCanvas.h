#pragma once


#include "wx/glcanvas.h"
#include "../VisualWardrobe/Common.h"

class ShellGLCanvas: public wxGLCanvas
{
public:
    ShellGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("ShellGLCanvas"));

    ~ShellGLCanvas();

    void LoadDXF(const wxString& filename);

protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);

private:
    void InitGL();
    void ResetProjectionMode();

	//struct GLData
	//{
	//	bool initialized;           // have OpenGL been initialized?
	//	float beginx, beginy;       // position of mouse
	//	float quat[4];              // orientation of object
	//	float zoom;                 // field of view in degrees
	//};

 //   GLData m_gldata;

	bool m_firstPaint;
    DECLARE_EVENT_TABLE()
};
